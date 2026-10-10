/* Store the two float arguments at offsets 0x2C and 0x30. */
typedef struct FloatPairAt2C {
    unsigned char padding[0x2C];
    float first_at_0x2C;
    float second_at_0x30;
} FloatPairAt2C;

void set_float_pair_at_0x2C_000CD744(
    FloatPairAt2C *object, float first, float second)
{
    object->first_at_0x2C = first;
    object->second_at_0x30 = second;
}
