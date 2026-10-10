/* Store the first argument at global address 0x00060000 + 0x4428. */
typedef struct GlobalWordAt64428 {
    unsigned char padding[0x4428];
    volatile unsigned value;
} GlobalWordAt64428;

void set_global_word_at_0x64428_00101F84(unsigned value)
{
    register GlobalWordAt64428 *region_base __asm__("$5") =
        (GlobalWordAt64428 *)0x00060000;
    region_base->value = value;
}
