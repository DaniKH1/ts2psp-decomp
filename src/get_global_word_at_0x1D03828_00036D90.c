/* Read the global word at retail address 0x1D03828. */
typedef struct GlobalWordRegion {
    unsigned char padding[0x3828];
    volatile unsigned value;
} GlobalWordRegion;

unsigned get_global_word_at_0x1D03828_00036D90(void)
{
    register GlobalWordRegion *base __asm__("$4") =
        (GlobalWordRegion *)0x001D0000;
    return base->value;
}
