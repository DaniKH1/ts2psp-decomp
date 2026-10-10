/* Store the two float arguments at offsets 0x24 and 0x28. */
typedef struct FloatPairAt24 {
    unsigned char padding[0x24];
    float first_at_0x24;
    float second_at_0x28;
} FloatPairAt24;

void set_float_pair_at_0x24_000CD770(
    FloatPairAt24 *object, float first, float second)
{
    object->first_at_0x24 = first;
    object->second_at_0x28 = second;
}
