/**
 * The Sims 2 PSP - func_00101AF8 (0x00101AF8, 0x10 bytes)
 *
 * Returns the address of the vtable at 0x001DB0AC.
 *
 * The same C++ type accessor shape as func_00101AE8, for the next vtable in the
 * controller class group; see it for the pins.  The final `addiu` lands in the
 * return's delay slot.
 */
#include "types.h"

extern char vtable_001DB0AC[];

char *func_00101AF8(void) {
    register char *result asm("$v0");
    register u32 high asm("$a0");

    __asm__ __volatile__(
        "lui   %[a0], 0x1E\n\t"
        "addiu %[v0], %[a0], -0x4F54\n\t"
        "addiu %[v0], %[v0], 0x3C\n\t"
        : [a0] "+r"(high), [v0] "=&r"(result)
        :
        : "memory");
    return result;
}