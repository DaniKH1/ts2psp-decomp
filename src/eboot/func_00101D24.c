/**
 * The Sims 2 PSP - func_00101D24 (0x00101D24, 0x10 bytes)
 *
 * Returns the address of the vtable at 0x001DB014.
 *
 * The same C++ type accessor shape as func_00101AE8, for the first vtable in the
 * controller class group; see it for the pins.  The final `addiu` lands in the
 * return's delay slot.
 */
#include "types.h"

extern char vtable_001DB014[];

char *func_00101D24(void) {
    register char *result asm("$v0");
    register u32 high asm("$a0");

    __asm__ __volatile__(
        "lui   %[a0], 0x1E\n\t"
        "addiu %[v0], %[a0], -0x4FEC\n\t"
        "addiu %[v0], %[v0], 0x1C\n\t"
        : [a0] "+r"(high), [v0] "=&r"(result)
        :
        : "memory");
    return result;
}