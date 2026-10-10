/* get_u8_at_0x6A_00088B6C: observed byte-level access at offset 0x6A. */
typedef struct ByteAt0x6A {
    unsigned char padding[0x6A];
    unsigned char value;
} ByteAt0x6A;

unsigned char get_u8_at_0x6A_00088B6C(const ByteAt0x6A *object)
{
    return object->value;
}
