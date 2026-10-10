/* Allegrex register $zero always reads as 0.  GCC 3.3's hard-register
 * variable support lets this C expression preserve the retail `ori`. */
int return_0x10_at_0000F574(void)
{
    register unsigned int zero asm("$0");
    return zero | 0x10;
}
