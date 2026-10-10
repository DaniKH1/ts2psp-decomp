/* Follow the pointer stored at offset 0x40 and read its first word. */
typedef struct WordPointerAt40 {
    unsigned char padding[0x40];
    const unsigned *value_pointer;
} WordPointerAt40;

unsigned get_word_through_pointer_at_0x40_0009DBE0(
    const WordPointerAt40 *object)
{
    register const unsigned *value_pointer __asm__("$4") =
        object->value_pointer;
    return *value_pointer;
}
