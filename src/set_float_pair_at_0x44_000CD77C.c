/* Store the two float arguments at offsets 0x44 and 0x48. */
typedef struct FloatPairAt44 {
    unsigned char padding[0x44];
    float first_at_0x44;
    float second_at_0x48;
} FloatPairAt44;

void set_float_pair_at_0x44_000CD77C(
    FloatPairAt44 *object, float first, float second)
{
    object->first_at_0x44 = first;
    object->second_at_0x48 = second;
}
