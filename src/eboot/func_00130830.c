/**
 * The Sims 2 PSP - func_00130830 (0x00130830, 0x10 bytes)
 *
 * A software trap.  It is never meant to run, and nothing does.
 *
 *     break 768
 *     nop
 *     jr    $ra
 *     nop
 *
 * **`break` with a code is how a compiler marks a path it proved unreachable**, and
 * 768 is 0x300, inside the software-break range rather than a hardware one.  So this
 * is a body that was never written: a function whose implementation is a trap.
 *
 * **It is also the only such function in the module** - one out of 7,497 begins with
 * a `break`, which is itself worth knowing: a codebase this size would normally be full
 * of them if they were ordinary.  One is not ordinary.
 *
 * What makes it interesting is the single caller.  `func_00130AE8` does not *call*
 * it; it takes **its address** and passes it to `func_00142910`:
 *
 *     lui   $a0, %hi(func_00130830)
 *     jal   func_00142910
 *     addiu $a0, $a0, %lo(func_00130830)
 *
 * and, a few instructions later, does the same with `str_exit_FE94` and
 * `func_00130810`.  So `func_00142910` takes a function pointer and is being handed a
 * sequence of them, and this trap is one of the values in that sequence.  **It is a
 * placeholder in a callback table** - a slot that had to exist, at a fixed address,
 * because something registers a pointer to it, but whose body was never filled in.
 *
 * `func_00142910` is also worth noting for where it sits: it is four hundred bytes
 * below `func_001429F0`, which is the path `func_0011296C` falls through to when an
 * abort has no handler at either nesting level.  The registration function and the
 * last-resort error path are neighbours in the link order, which is what would be
 * expected if both are part of the same at-exit or shutdown machinery.
 *
 * The `jr $ra` is dead code and is transcribed anyway.  Leaving it out would give
 * three instructions and shift everything after it.
 */
#include "types.h"

/* `noreturn` is a lie: the function does return, past a `jr $ra` that is itself dead.
 * It is here for the reason it is in func_000A9A00 - the block contains the return, so
 * the compiler must not add one.  With the explicit `nop` in the block, because
 * `noreorder` is not in play here but there is still nothing for the compiler to
 * attribute a delay slot to. */
__attribute__((noreturn)) void func_00130830(void) {
    __asm__ __volatile__(
        "break 768\n\t"
        "nop\n\t"
        /* `noreorder`, or the assembler fills the delay slot by moving the preceding
         * `nop` across the branch and the pair comes out the other way round. */
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "nop\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}