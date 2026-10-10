/* Store a float in the field at byte offset 0x60. */
typedef struct FloatSlotAt_60_000CD788 {
    unsigned char padding[0x60];
    float value;
} FloatSlotAt_60_000CD788;

void set_float_at_0x60_000CD788(FloatSlotAt_60_000CD788 *object, float value)
{
    object->value = value;
}
