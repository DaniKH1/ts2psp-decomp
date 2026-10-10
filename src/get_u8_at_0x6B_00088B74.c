/* get_u8_at_0x6B_00088B74: observed byte-level access at offset 0x6B. */
typedef struct ByteAt0x6B {
    unsigned char padding[0x6B];
    unsigned char value;
} ByteAt0x6B;

unsigned char get_u8_at_0x6B_00088B74(const ByteAt0x6B *object)
{
    return object->value;
}
