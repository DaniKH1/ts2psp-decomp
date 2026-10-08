/**
 * The Sims 2 PSP - func_001029E8 (0x001029E8, 0x8 bytes)
 *
 *     jr $ra
 *     nop
 *
 * **This function does nothing.**  Two instructions, eight bytes, no memory
 * touched, no register read.
 *
 * It is byte-identical to `func_001029E0`, `func_00102228` and
 * `func_001022B0`, and those four stubs are what the two wrapper functions
 * `func_00102230` and `func_0010224C` call - **so those wrappers also compute
 * nothing, and the six of them form a closed subgraph with no effect.**  See
 * `func_001029E0.c` for the two readings that are available and the reason the
 * module cannot choose between them.
 *
 * The fact that the linker kept two separate copies rather than folding them
 * is worth stating: **four identical bodies, none merged**, which is what a
 * translation unit boundary between each pair would produce, and also what
 * four independently-empty inline bodies would produce.  The bytes do not
 * separate those two.
 */
#include "types.h"

/** Returns immediately without doing anything. */
__attribute__((noreturn)) void func_001029E8(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "jr $ra\n\t"
        "nop\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}