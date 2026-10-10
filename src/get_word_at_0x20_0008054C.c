/* Return the word at byte offset 0x20 of this object. */
typedef struct WordAt0x20 {
    unsigned char padding[0x20];
    unsigned value;
} WordAt0x20;

unsigned get_word_at_0x20_0008054C(const WordAt0x20 *object)
{
    return object->value;
}
