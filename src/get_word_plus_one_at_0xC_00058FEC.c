/* Return the word at offset 0x0C increased by one. */
typedef struct WordAt0C {
    unsigned char padding[0x0C];
    unsigned value_at_0x0C;
} WordAt0C;

unsigned get_word_plus_one_at_0xC_00058FEC(const WordAt0C *object)
{
    return object->value_at_0x0C + 1;
}
