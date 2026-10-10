/* Store the first argument at global address 0x00060000 + 0x442C. */
typedef struct GlobalWordAt6442C {
    unsigned char padding[0x442C];
    volatile unsigned value;
} GlobalWordAt6442C;

void set_global_word_at_0x6442C_00100EE4(unsigned value)
{
    register GlobalWordAt6442C *region_base __asm__("$5") =
        (GlobalWordAt6442C *)0x00060000;
    region_base->value = value;
}
