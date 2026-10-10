/* Store a float in the field at byte offset 0x10. */
typedef struct FloatSlotAt_10_000F96C0 {
    unsigned char padding[0x10];
    float value;
} FloatSlotAt_10_000F96C0;

void set_float_at_0x10_000F96C0(FloatSlotAt_10_000F96C0 *object, float value)
{
    object->value = value;
}
