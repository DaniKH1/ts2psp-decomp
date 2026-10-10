/* Set the word at byte offset 0xCC of this object. */
typedef struct WordAt0xCC {
    unsigned char padding[0xCC];
    unsigned value;
} WordAt0xCC;

void set_word_at_0xCC_000CDEEC(WordAt0xCC *object, unsigned value)
{
    object->value = value;
}
