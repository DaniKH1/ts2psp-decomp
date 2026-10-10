/* Return the word at byte offset 0x38 of this object. */
typedef struct WordAt0x38 {
    unsigned char padding[0x38];
    unsigned value;
} WordAt0x38;

unsigned get_word_at_0x38_00056E74(const WordAt0x38 *object)
{
    return object->value;
}
