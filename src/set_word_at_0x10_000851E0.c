/* Set the word at byte offset 0x10 of this object. */
typedef struct WordAt0x10 {
    unsigned char padding[0x10];
    unsigned value;
} WordAt0x10;

void set_word_at_0x10_000851E0(WordAt0x10 *object, unsigned value)
{
    object->value = value;
}
