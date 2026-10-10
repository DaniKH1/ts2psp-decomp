/* Clear the global byte at 0x001E0000 - 0x6054. */
void clear_global_u8_at_0x1D9FAC_000DD524(void)
{
    register volatile unsigned char *region_base __asm__("$4") =
        (volatile unsigned char *)0x001E0000;
    region_base[-0x6054] = 0;
}
