/* Return the retail constant 0x1 using Allegrex $zero. */
unsigned return_0x1_at_000B9D34(void)
{
    register unsigned int zero asm("$0");
    __asm__ volatile ("" : "=r"(zero));
    return zero | 0x1u;
}
