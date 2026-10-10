/* Return the word at byte offset 0x24 of this object. */
typedef struct WordAt0x24 {
    unsigned char padding[0x24];
    unsigned value;
} WordAt0x24;

unsigned get_word_at_0x24_000C1490(const WordAt0x24 *object)
{
    return object->value;
}
