/* Return the retail constant 0x1 using Allegrex $zero. */
unsigned renderCommon_001BD9CC(void)
{
    register unsigned int zero asm("$0");
    __asm__ volatile ("" : "=r"(zero));
    return zero | 0x1u;
}
