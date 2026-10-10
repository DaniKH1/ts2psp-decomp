/* get_u8_at_0x1_00125B0C: observed byte-level access at offset 0x1. */
typedef struct ByteAt0x1 {
    unsigned char padding[0x1];
    unsigned char value;
} ByteAt0x1;

unsigned char get_u8_at_0x1_00125B0C(const ByteAt0x1 *object)
{
    return object->value;
}
