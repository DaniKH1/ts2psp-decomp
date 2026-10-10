/* Set the word at byte offset 0x1C of this object. */
typedef struct WordAt0x1C {
    unsigned char padding[0x1C];
    unsigned value;
} WordAt0x1C;

void set_word_at_0x1C_00085A20(WordAt0x1C *object, unsigned value)
{
    object->value = value;
}
