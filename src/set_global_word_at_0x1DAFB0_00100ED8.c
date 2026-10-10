/* Store the first argument at global address 0x001E0000 - 0x5050. */
void set_global_word_at_0x1DAFB0_00100ED8(unsigned value)
{
    register volatile unsigned *region_base __asm__("$5") =
        (volatile unsigned *)0x001E0000;
    region_base[-0x1414] = value;
}
