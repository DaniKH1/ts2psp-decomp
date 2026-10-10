/* Return the word at byte offset 0x3C of this object. */
typedef struct WordAt0x3C {
    unsigned char padding[0x3C];
    unsigned value;
} WordAt0x3C;

unsigned get_word_at_0x3C_00111490(const WordAt0x3C *object)
{
    return object->value;
}
