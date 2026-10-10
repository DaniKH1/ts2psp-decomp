/* Follow the word at offset 0 and store a byte through that pointer. */
typedef struct BytePointerAt0 {
    unsigned char *value_pointer;
} BytePointerAt0;

void set_byte_through_pointer_at_0_00091418(
    const BytePointerAt0 *object, unsigned value)
{
    register unsigned char *value_pointer __asm__("$4") =
        object->value_pointer;
    *value_pointer = value;
}
