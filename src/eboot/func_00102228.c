/**
 * The Sims 2 PSP - func_00102228 (0x00102228, 0x8 bytes)
 *
 *     jr $ra
 *     nop
 *
 * **This function does nothing.**  Two instructions, eight bytes.
 *
 * It is one of four byte-identical eight-byte stubs in this renderer region,
 * alongside `func_001029E0`, `func_001029E8` and `func_001022B0`.  Unlike those
 * three, **nothing in the six-function group calls this one** - the two
 * wrappers call the two stubs at `0x001029E0`/`0x001029E8`, so this pair is a
 * second, unreached copy of the same empty body.  See `func_001029E0.c` for
 * what that pair of copies looks like from the linker's side.
 *
 * `func_00102228` sits immediately before `func_00102230`, which is a wrapper
 * around a stub, and after `func_0010224C`'s neighbour `func_00102230` - so
 * the layout is stub, wrapper, stub, wrapper with no separating run of real
 * code.  **Whether this pair is a duplicated pair or a hand-written pair is not
 * something the bytes decide.**
 */
#include "types.h"

/** Returns immediately without doing anything. */
__attribute__((noreturn)) void func_00102228(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "jr $ra\n\t"
        "nop\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}