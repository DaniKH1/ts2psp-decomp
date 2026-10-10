/* Store a word at offset 0x48 and a byte at offset 0x204. */
typedef struct WordAndByteAt48 {
    unsigned char padding_to_word[0x48];
    unsigned word_at_0x48;
    unsigned char padding_to_byte[0x204 - 0x4C];
    unsigned char byte_at_0x204;
} WordAndByteAt48;

void set_word_at_0x48_and_u8_at_0x204_0009D8A0(
    WordAndByteAt48 *object, unsigned word, unsigned byte)
{
    object->word_at_0x48 = word;
    object->byte_at_0x204 = byte;
}
