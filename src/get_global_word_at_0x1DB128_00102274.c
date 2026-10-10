/* Read the global word at 0x001E0000 - 0x4ED8. */
unsigned get_global_word_at_0x1DB128_00102274(void)
{
    register volatile unsigned *region_base __asm__("$4") =
        (volatile unsigned *)0x001E0000;
    return region_base[-0x13B6];
}
