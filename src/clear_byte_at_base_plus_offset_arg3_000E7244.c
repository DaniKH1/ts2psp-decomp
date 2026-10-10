/* Clear the byte at base + offset, where those are arguments 1 and 3. */
void clear_byte_at_base_plus_offset_arg3_000E7244(
    int unused0, unsigned char *base, int unused2, unsigned offset)
{
    register unsigned char *address __asm__("$4") = base + offset;
    *address = 0;
}
