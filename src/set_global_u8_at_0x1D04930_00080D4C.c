/* Store a byte in the global region at retail address 0x1D04930. */
typedef struct GlobalByteRegion {
    unsigned char padding[0x4930];
    volatile unsigned char value;
} GlobalByteRegion;

void set_global_u8_at_0x1D04930_00080D4C(unsigned value)
{
    register GlobalByteRegion *base __asm__("$5") =
        (GlobalByteRegion *)0x001D0000;
    base->value = value;
}
