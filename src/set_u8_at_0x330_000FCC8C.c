/* set_u8_at_0x330_000FCC8C: observed byte-level access at offset 0x330. */
typedef struct ByteAt0x330 {
    unsigned char padding[0x330];
    unsigned char value;
} ByteAt0x330;

void set_u8_at_0x330_000FCC8C(ByteAt0x330 *object, unsigned char value)
{
    object->value = value;
}
