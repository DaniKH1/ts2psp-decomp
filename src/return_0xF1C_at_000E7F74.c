/* Return the retail constant 0xF1C using Allegrex $zero. */
unsigned return_0xF1C_at_000E7F74(void)
{
    register unsigned int zero asm("$0");
    __asm__ volatile ("" : "=r"(zero));
    return zero | 0xF1Cu;
}
