/* Return positive infinity using the retail IEEE-754 bit pattern. */
float return_positive_infinity_at_00140A4C(void)
{
    union FloatBits {
        unsigned int bits;
        float value;
    } result;
    register volatile unsigned int float_bits __asm__("$4") = 0x7F800000;
    /* Constrain the constant to $a0; this empty asm emits no instruction. */
    __asm__ volatile ("" : : "r"(float_bits));
    result.bits = float_bits;
    return result.value;
}
