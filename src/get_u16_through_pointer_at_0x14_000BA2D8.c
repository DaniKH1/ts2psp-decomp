/* Follow the pointer at offset 0x14 and read its 16-bit field at +0x14. */
typedef struct U16At14 {
    unsigned char padding[0x14];
    unsigned short value_at_0x14;
} U16At14;

typedef struct U16PointerAt14 {
    unsigned char padding[0x14];
    const U16At14 *value_pointer;
} U16PointerAt14;

unsigned get_u16_through_pointer_at_0x14_000BA2D8(
    const U16PointerAt14 *object)
{
    register const U16At14 *value_pointer __asm__("$4") =
        object->value_pointer;
    return value_pointer->value_at_0x14;
}
