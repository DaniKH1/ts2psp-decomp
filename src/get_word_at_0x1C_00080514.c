/* Return the word at byte offset 0x1C of this object. */
typedef struct WordAt0x1C {
    unsigned char padding[0x1C];
    unsigned value;
} WordAt0x1C;

unsigned get_word_at_0x1C_00080514(const WordAt0x1C *object)
{
    return object->value;
}
