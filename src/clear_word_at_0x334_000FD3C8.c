/* Clear the word at byte offset 0x334 of this object. */
typedef struct WordAt0x334 {
    unsigned char padding[0x334];
    unsigned value;
} WordAt0x334;

void clear_word_at_0x334_000FD3C8(WordAt0x334 *object)
{
    object->value = 0;
}
