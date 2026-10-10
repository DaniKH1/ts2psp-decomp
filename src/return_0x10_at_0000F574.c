/* Allegrex register $zero always reads as 0.  The empty GNU asm output
 * constraint makes GCC expose that hardware register as a C value. */
int return_0x10_at_0000F574(void)
{
    register unsigned int zero asm("$0");
    __asm__ volatile ("" : "=r"(zero));
    return zero | 0x10;
}
