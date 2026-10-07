/**
 * The Sims 2 PSP - func_00128F50 (0x00128F50, 0x10 bytes)
 *
 * Returns `later - earlier`, the fields at offset 0x18 and 0x8.  The same
 * elapsed-time comparator as func_000E4B94 and func_000F8908, with the two
 * fields at different offsets; the pins are explained there.
 */
#include "types.h"

typedef struct Elapsed {
    u8 pad[0x8];
    s32 start;   /* 0x08 */
    u8 pad2[0x10];
    s32 now;     /* 0x18 */
} Elapsed;

s32 func_00128F50(Elapsed *self) {
    register Elapsed *ptr asm("$a0") = self;
    register s32 out asm("$v0");

    __asm__ __volatile__(
        "lw   %[out], 0x08(%[ptr])\n\t"
        "lw   %[ptr], 0x18(%[ptr])\n\t"
        "subu %[out], %[out], %[ptr]\n\t"
        : [out] "+r"(out), [ptr] "+r"(ptr)
        :
        : "memory");
    return out;
}