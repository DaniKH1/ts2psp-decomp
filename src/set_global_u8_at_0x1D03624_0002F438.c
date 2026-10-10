/* Store a byte at the global retail address 0x1D03624. */
typedef struct GlobalByteRegion {
    unsigned char padding[0x3624];
    volatile unsigned char value;
} GlobalByteRegion;

void set_global_u8_at_0x1D03624_0002F438(unsigned value)
{
    register GlobalByteRegion *base __asm__("$5") =
        (GlobalByteRegion *)0x001D0000;
    base->value = value;
}
