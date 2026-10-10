/* Return the word at byte offset 0x34 of this object. */
typedef struct WordAt0x34 {
    unsigned char padding[0x34];
    unsigned value;
} WordAt0x34;

unsigned get_word_at_0x34_001114A0(const WordAt0x34 *object)
{
    return object->value;
}
