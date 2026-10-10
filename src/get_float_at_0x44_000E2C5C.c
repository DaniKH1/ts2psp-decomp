/* Read the float field at byte offset 0x44. */
typedef struct FloatAt_44_000E2C5C {
    unsigned char padding[0x44];
    float value;
} FloatAt_44_000E2C5C;

float get_float_at_0x44_000E2C5C(const FloatAt_44_000E2C5C *object)
{
    return object->value;
}
