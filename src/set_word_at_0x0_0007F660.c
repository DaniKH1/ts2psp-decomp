/* Set the word at byte offset 0x0 of this object. */
typedef struct WordAt0x0 {
    unsigned value;
} WordAt0x0;

void set_word_at_0x0_0007F660(WordAt0x0 *object, unsigned value)
{
    object->value = value;
}
