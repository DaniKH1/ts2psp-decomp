/* get_u8_at_0x30_00111498: observed byte-level access at offset 0x30. */
typedef struct ByteAt0x30 {
    unsigned char padding[0x30];
    unsigned char value;
} ByteAt0x30;

unsigned char get_u8_at_0x30_00111498(const ByteAt0x30 *object)
{
    return object->value;
}
