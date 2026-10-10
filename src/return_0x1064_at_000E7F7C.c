/* Return the retail constant 0x1064 using Allegrex $zero. */
unsigned return_0x1064_at_000E7F7C(void)
{
    register unsigned int zero asm("$0");
    __asm__ volatile ("" : "=r"(zero));
    return zero | 0x1064u;
}
