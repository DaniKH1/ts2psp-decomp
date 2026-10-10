/* Read a word from the global region at 0x001D0000 + 0x4D9C. */
typedef struct GlobalWordAt1D4D9C {
    unsigned char padding[0x4D9C];
    volatile unsigned value;
} GlobalWordAt1D4D9C;

unsigned get_global_word_at_0x1D4D9C_00097778(void)
{
    register GlobalWordAt1D4D9C *base __asm__("$4") =
        (GlobalWordAt1D4D9C *)0x001D0000;
    return base->value;
}
