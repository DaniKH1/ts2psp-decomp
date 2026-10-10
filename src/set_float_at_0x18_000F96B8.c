/* Store a float in the field at byte offset 0x18. */
typedef struct FloatSlotAt_18_000F96B8 {
    unsigned char padding[0x18];
    float value;
} FloatSlotAt_18_000F96B8;

void set_float_at_0x18_000F96B8(FloatSlotAt_18_000F96B8 *object, float value)
{
    object->value = value;
}
