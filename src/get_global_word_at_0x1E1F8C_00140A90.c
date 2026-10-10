/* Read the global word at 0x001E0000 + 0x1F8C. */
typedef struct GlobalWordsAt1E1F8C {
    unsigned char padding[0x1F8C];
    volatile unsigned value;
} GlobalWordsAt1E1F8C;

unsigned get_global_word_at_0x1E1F8C_00140A90(void)
{
    register GlobalWordsAt1E1F8C *region_base __asm__("$4") =
        (GlobalWordsAt1E1F8C *)0x001E0000;
    return region_base->value;
}
