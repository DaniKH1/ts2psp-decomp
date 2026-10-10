/* Return the word at byte offset 0xC of this object. */
typedef struct WordAt0xC {
    unsigned char padding[0xC];
    unsigned value;
} WordAt0xC;

unsigned get_word_at_0xC_0009C2F8(const WordAt0xC *object)
{
    return object->value;
}
