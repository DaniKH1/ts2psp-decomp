/**
 * The Sims 2 PSP - func_0018F668 (0x0018F668, 0x8 bytes)
 *
 *     jr    $ra
 *       nop
 *
 * A default virtual method that returns **nothing at all**.  `$v0` is
 * left exactly as the caller passed it.
 *
 * **This is the only method in the whole shared vtable that does not
 * write `$v0`.**  Of the 26 slots in `sym_001EA3E8`, twenty-three are
 * shared by all three sibling classes: two return 1, sixteen return 0,
 * four have real bodies, and this one is void.  So the table's return
 * convention is mixed, and a caller reaching this slot cannot expect a
 * result - which is itself worth knowing, because a caller that reads
 * `$v0` here is reading its own leftover register.
 */
#include "types.h"

__attribute__((noreturn)) void func_0018F668(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "nop\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}