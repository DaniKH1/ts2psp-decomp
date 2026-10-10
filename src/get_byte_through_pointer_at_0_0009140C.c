/* Follow the word at offset 0 and read a byte from the pointed-to object. */
typedef struct BytePointerAt0 {
    const unsigned char *value_pointer;
} BytePointerAt0;

unsigned get_byte_through_pointer_at_0_0009140C(const BytePointerAt0 *object)
{
    register const unsigned char *value_pointer __asm__("$4") =
        object->value_pointer;
    return *value_pointer;
}
