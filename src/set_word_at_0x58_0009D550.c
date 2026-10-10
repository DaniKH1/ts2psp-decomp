/* Set the word at byte offset 0x58 of this object. */
typedef struct WordAt0x58 {
    unsigned char padding[0x58];
    unsigned value;
} WordAt0x58;

void set_word_at_0x58_0009D550(WordAt0x58 *object, unsigned value)
{
    object->value = value;
}
