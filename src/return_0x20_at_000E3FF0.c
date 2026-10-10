/* Return the retail constant 0x20 using Allegrex $zero. */
unsigned return_0x20_at_000E3FF0(void)
{
    register unsigned int zero asm("$0");
    __asm__ volatile ("" : "=r"(zero));
    return zero | 0x20u;
}
