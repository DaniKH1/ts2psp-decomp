/* get_byte_address_at_0x80_000C3004: observed byte-level access at offset 0x80. */
unsigned char *get_byte_address_at_0x80_000C3004(void *object)
{
    return (unsigned char *)object + 0x80;
}
