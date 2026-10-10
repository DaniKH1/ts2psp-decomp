/* Store the argument at offset 0x28, then return zero. */
typedef struct WordAt28 {
    unsigned char padding[0x28];
    unsigned value_at_0x28;
} WordAt28;

int set_word_at_0x28_and_return_zero_000F89B0(
    WordAt28 *object, unsigned value)
{
    object->value_at_0x28 = value;
    return 0;
}
