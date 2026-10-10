/* Copy the word at source offset 0x04 to the destination pointer. */
typedef struct WordSourceAt4 {
    unsigned char padding[0x04];
    unsigned value_at_0x04;
} WordSourceAt4;

void copy_word_from_offset_4_to_pointer_000D67C8(
    const WordSourceAt4 *source, unsigned *destination)
{
    register unsigned value __asm__("$4") = source->value_at_0x04;
    *destination = value;
}
