/* Return the word at byte offset 0x14 of this object. */
typedef struct WordAt0x14 {
    unsigned char padding[0x14];
    unsigned value;
} WordAt0x14;

unsigned get_word_at_0x14_000E4BA4(const WordAt0x14 *object)
{
    return object->value;
}
