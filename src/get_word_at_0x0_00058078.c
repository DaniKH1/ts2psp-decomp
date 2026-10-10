/* Return the word at byte offset 0x0 of this object. */
typedef struct WordAt0x0 {
    unsigned value;
} WordAt0x0;

unsigned get_word_at_0x0_00058078(const WordAt0x0 *object)
{
    return object->value;
}
