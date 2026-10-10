/* Read the global float at 0x001D0000 + 0x53C0. */
typedef struct GlobalFloatAt1D53C0 {
    unsigned char padding[0x53C0];
    volatile float value;
} GlobalFloatAt1D53C0;

float get_global_float_at_0x1D53C0_000ACB14(void)
{
    register GlobalFloatAt1D53C0 *region_base __asm__("$4") =
        (GlobalFloatAt1D53C0 *)0x001D0000;
    return region_base->value;
}
