/* Return the float whose retail bit pattern is 0x43880000 (272.0f). */
float return_float_272_at_000E0784(void)
{
    union FloatBits {
        unsigned int bits;
        float value;
    } result;
    register volatile unsigned int float_bits __asm__("$4") = 0x43880000;
    /* Constrain the constant to $a0; this empty asm emits no instruction. */
    __asm__ volatile ("" : : "r"(float_bits));
    result.bits = float_bits;
    return result.value;
}
