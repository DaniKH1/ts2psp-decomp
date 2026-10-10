/* Return the low 16 bits of the word at offset 0x04. */
typedef struct WordAt4 {
    unsigned char padding[0x04];
    /* Retail reads a full word before masking off the upper half. */
    volatile unsigned value_at_0x04;
} WordAt4;

unsigned get_low_u16_at_0x4_00054510(const WordAt4 *object)
{
    return object->value_at_0x04 & 0xFFFFu;
}
