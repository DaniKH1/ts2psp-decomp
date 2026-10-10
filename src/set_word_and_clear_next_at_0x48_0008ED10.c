/* Store a word at offset 0x48 and clear the following word at 0x4C. */
typedef struct WordPairAt48 {
    unsigned char padding[0x48];
    unsigned value_at_0x48;
    unsigned cleared_at_0x4C;
} WordPairAt48;

void set_word_and_clear_next_at_0x48_0008ED10(WordPairAt48 *object,
                                               unsigned value)
{
    object->value_at_0x48 = value;
    object->cleared_at_0x4C = 0;
}
