/* Set the word at byte offset 0x8 of this object. */
typedef struct WordAt0x8 {
    unsigned char padding[0x8];
    unsigned value;
} WordAt0x8;

void set_word_at_0x8_000CD700(WordAt0x8 *object, unsigned value)
{
    object->value = value;
}
