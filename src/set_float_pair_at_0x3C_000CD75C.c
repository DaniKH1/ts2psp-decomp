/* Store the two float arguments at offsets 0x3C and 0x40. */
typedef struct FloatPairAt3C {
    unsigned char padding[0x3C];
    float first_at_0x3C;
    float second_at_0x40;
} FloatPairAt3C;

void set_float_pair_at_0x3C_000CD75C(
    FloatPairAt3C *object, float first, float second)
{
    object->first_at_0x3C = first;
    object->second_at_0x40 = second;
}
