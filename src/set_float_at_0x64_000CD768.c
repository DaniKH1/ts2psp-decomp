/* Store a float in the field at byte offset 0x64. */
typedef struct FloatSlotAt_64_000CD768 {
    unsigned char padding[0x64];
    float value;
} FloatSlotAt_64_000CD768;

void set_float_at_0x64_000CD768(FloatSlotAt_64_000CD768 *object, float value)
{
    object->value = value;
}
