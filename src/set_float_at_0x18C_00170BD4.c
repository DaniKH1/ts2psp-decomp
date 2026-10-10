/* Store the float argument at offset 0x18C in an opaque object. */
typedef struct FloatAt18C {
    unsigned char padding[0x18C];
    float value_at_0x18C;
} FloatAt18C;

void set_float_at_0x18C_00170BD4(FloatAt18C *object, float value)
{
    object->value_at_0x18C = value;
}
