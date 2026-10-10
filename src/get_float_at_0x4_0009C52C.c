/* Read the float field at byte offset 0x4. */
typedef struct FloatAt_4_0009C52C {
    unsigned char padding[0x4];
    float value;
} FloatAt_4_0009C52C;

float get_float_at_0x4_0009C52C(const FloatAt_4_0009C52C *object)
{
    return object->value;
}
