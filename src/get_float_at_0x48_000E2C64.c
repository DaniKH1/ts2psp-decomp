/* Read the float field at byte offset 0x48. */
typedef struct FloatAt_48_000E2C64 {
    unsigned char padding[0x48];
    float value;
} FloatAt_48_000E2C64;

float get_float_at_0x48_000E2C64(const FloatAt_48_000E2C64 *object)
{
    return object->value;
}
