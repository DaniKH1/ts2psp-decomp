/* Read a word from the global region at 0x00070000 + 0x43A0. */
typedef struct GlobalWordAt743A0 {
    unsigned char padding[0x43A0];
    volatile unsigned value;
} GlobalWordAt743A0;

unsigned get_global_word_at_0x743A0_00049A84(void)
{
    register GlobalWordAt743A0 *base __asm__("$4") =
        (GlobalWordAt743A0 *)0x00070000;
    return base->value;
}
