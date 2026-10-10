/* Copy the word at offset 0x18 into the adjacent word at offset 0x1C. */
typedef struct AdjacentWordsAt18 {
    unsigned char padding[0x18];
    unsigned source_at_0x18;
    unsigned destination_at_0x1C;
} AdjacentWordsAt18;

void copy_word_at_0x18_to_0x1C_0011F440(AdjacentWordsAt18 *object)
{
    object->destination_at_0x1C = object->source_at_0x18;
}
