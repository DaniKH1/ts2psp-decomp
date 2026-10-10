/* get_u8_at_0x2_00125B14: observed byte-level access at offset 0x2. */
typedef struct ByteAt0x2 {
    unsigned char padding[0x2];
    unsigned char value;
} ByteAt0x2;

unsigned char get_u8_at_0x2_00125B14(const ByteAt0x2 *object)
{
    return object->value;
}
