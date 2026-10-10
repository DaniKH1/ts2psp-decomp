/* Store the two word arguments at offsets 0x08 and 0x0C. */
typedef struct WordPairAt8 {
    unsigned char padding[0x08];
    unsigned first_at_0x08;
    unsigned second_at_0x0C;
} WordPairAt8;

void set_word_pair_at_0x8_00127920(
    WordPairAt8 *object, unsigned first, unsigned second)
{
    object->first_at_0x08 = first;
    object->second_at_0x0C = second;
}
