/* Read the global word at 0x001E0000 - 0x5D14. */
unsigned get_global_word_at_0x1DA2EC_000E2BF0(void)
{
    register volatile unsigned *region_base __asm__("$4") =
        (volatile unsigned *)0x001E0000;
    return region_base[-0x1745];
}
