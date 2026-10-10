/* Store the two float arguments at offsets 0x34 and 0x38. */
typedef struct FloatPairAt34 {
    unsigned char padding[0x34];
    float first_at_0x34;
    float second_at_0x38;
} FloatPairAt34;

void set_float_pair_at_0x34_000CD750(
    FloatPairAt34 *object, float first, float second)
{
    object->first_at_0x34 = first;
    object->second_at_0x38 = second;
}
