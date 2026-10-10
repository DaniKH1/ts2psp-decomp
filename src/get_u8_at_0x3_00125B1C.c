/* get_u8_at_0x3_00125B1C: observed byte-level access at offset 0x3. */
typedef struct ByteAt0x3 {
    unsigned char padding[0x3];
    unsigned char value;
} ByteAt0x3;

unsigned char get_u8_at_0x3_00125B1C(const ByteAt0x3 *object)
{
    return object->value;
}
