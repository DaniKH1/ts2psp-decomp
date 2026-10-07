/**
 * The Sims 2 PSP - func_00101AE8 (0x00101AE8, 0x10 bytes)
 *
 * Returns the address of the vtable at 0x001DB050.  This is a C++ type
 * accessor: the engine's controllers register `Start` and `ActiveController`
 * from their static constructors, and each of those translation units gets one
 * of these, so the group at 0x001DB050..0x001DB0F8 is the controller class
 * hierarchy.
 *
 * The address is materialised in three instructions rather than a single
 * `lui`/`addiu` pair: `lui $a0, 0x1E` carries the high half, then two `addiu`s
 * add the rest, and the last one goes in the return's delay slot.  The pin is
 * that the high half has to be computed *before* it is added into, in `$a0`,
 * which is not what GCC does - it materialises the whole address in `$v0` and
 * never emits the second `addiu`.
 *
 * The siblings func_00101AF8 and func_00101D24 are the same shape at the other
 * two vtables in the group.
 */
#include "types.h"

extern char vtable_001DB050[];

char *func_00101AE8(void) {
    register char *result asm("$v0");
    register u32 high asm("$a0");

    __asm__ __volatile__(
        "lui   %[a0], 0x1E\n\t"
        "addiu %[v0], %[a0], -0x4FEC\n\t"
        "addiu %[v0], %[v0], 0x3C\n\t"
        : [a0] "+r"(high), [v0] "=&r"(result)
        :
        : "memory");
    return result;
}