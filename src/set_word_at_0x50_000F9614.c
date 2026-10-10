/* Set the word at byte offset 0x50 of this object. */
typedef struct WordAt0x50 {
    unsigned char padding[0x50];
    unsigned value;
} WordAt0x50;

void set_word_at_0x50_000F9614(WordAt0x50 *object, unsigned value)
{
    object->value = value;
}
