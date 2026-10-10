/* Store the argument at offset 0x04, then return zero. */
typedef struct WordAt4 {
    unsigned char padding[0x04];
    unsigned value_at_0x04;
} WordAt4;

int set_word_at_0x4_and_return_zero_000F8FAC(
    WordAt4 *object, unsigned value)
{
    object->value_at_0x04 = value;
    return 0;
}
