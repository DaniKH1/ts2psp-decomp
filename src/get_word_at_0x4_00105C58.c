/* Return the word at byte offset 0x4 of this object. */
typedef struct WordAt0x4 {
    unsigned char padding[0x4];
    unsigned value;
} WordAt0x4;

unsigned get_word_at_0x4_00105C58(const WordAt0x4 *object)
{
    return object->value;
}
