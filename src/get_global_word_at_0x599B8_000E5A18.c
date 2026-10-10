/* Read the global word at 0x00060000 - 0x6648. */
unsigned get_global_word_at_0x599B8_000E5A18(void)
{
    register volatile unsigned *region_base __asm__("$4") =
        (volatile unsigned *)0x00060000;
    return region_base[-0x1992];
}
