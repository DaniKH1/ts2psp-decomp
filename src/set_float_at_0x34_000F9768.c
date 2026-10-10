/* Store a float in the field at byte offset 0x34. */
typedef struct FloatSlotAt_34_000F9768 {
    unsigned char padding[0x34];
    float value;
} FloatSlotAt_34_000F9768;

void set_float_at_0x34_000F9768(FloatSlotAt_34_000F9768 *object, float value)
{
    object->value = value;
}
