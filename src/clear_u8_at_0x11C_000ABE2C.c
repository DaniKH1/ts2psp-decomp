/* clear_u8_at_0x11C_000ABE2C: observed byte-level access at offset 0x11C. */
typedef struct ByteAt0x11C {
    unsigned char padding[0x11C];
    unsigned char value;
} ByteAt0x11C;

void clear_u8_at_0x11C_000ABE2C(ByteAt0x11C *object)
{
    object->value = 0;
}
