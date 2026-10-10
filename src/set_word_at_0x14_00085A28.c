/* Set the word at byte offset 0x14 of this object. */
typedef struct WordAt0x14 {
    unsigned char padding[0x14];
    unsigned value;
} WordAt0x14;

void set_word_at_0x14_00085A28(WordAt0x14 *object, unsigned value)
{
    object->value = value;
}
