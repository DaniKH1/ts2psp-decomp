/* Return the word at byte offset 0x18 of this object. */
typedef struct WordAt0x18 {
    unsigned char padding[0x18];
    unsigned value;
} WordAt0x18;

unsigned get_word_at_0x18_00080584(const WordAt0x18 *object)
{
    return object->value;
}
