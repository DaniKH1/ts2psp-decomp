/* get_u8_at_0x55_0008A578: observed byte-level access at offset 0x55. */
typedef struct ByteAt0x55 {
    unsigned char padding[0x55];
    unsigned char value;
} ByteAt0x55;

unsigned char get_u8_at_0x55_0008A578(const ByteAt0x55 *object)
{
    return object->value;
}
