/* Follow the pointer stored at offset 0x0C and read its first word. */
typedef struct WordPointerAt0C {
    unsigned char padding[0x0C];
    const unsigned *value_pointer;
} WordPointerAt0C;

unsigned get_word_through_pointer_at_0xC_00098714(
    const WordPointerAt0C *object)
{
    register const unsigned *value_pointer __asm__("$4") =
        object->value_pointer;
    return *value_pointer;
}
