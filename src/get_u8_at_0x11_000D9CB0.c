/* get_u8_at_0x11_000D9CB0: observed byte-level access at offset 0x11. */
typedef struct ByteAt0x11 {
    unsigned char padding[0x11];
    unsigned char value;
} ByteAt0x11;

unsigned char get_u8_at_0x11_000D9CB0(const ByteAt0x11 *object)
{
    return object->value;
}
