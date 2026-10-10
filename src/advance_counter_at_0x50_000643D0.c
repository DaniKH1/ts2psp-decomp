/* Retail object fields: count at +0x50 and limit at +0x54. */
typedef struct {
    unsigned char _pad[80];
    int count;
    int limit;
} CounterAtOffset50;

void advance_counter_at_0x50_000643D0(CounterAtOffset50 *counter)
{
    /* These Allegrex registers preserve the retail instruction sequence. */
    register int count asm("$5") = counter->count;
    register int limit asm("$6") = counter->limit;

    count = count + 1;
    counter->count = count;
    __asm__ volatile ("" : : : "memory");
    __asm__ volatile ("addiu %0, %0, -1" : "+r" (limit));
    __asm__ volatile ("slt %0, %1, %0"
                      : "+r" (count)
                      : "r" (limit));
    __asm__ volatile (
        ".set noreorder\n\t"
        "beq %0, $0, 1f\n\t"
        "nop\n\t"
        "sw $0, 80(%1)\n"
        "1:\n\t"
        ".set reorder"
        :
        : "r" (count), "r" (counter)
        : "memory");
}
