/**
 * The Sims 2 PSP - func_001022B0 (0x001022B0, 0x8 bytes)
 *
 *     jr $ra
 *     nop
 *
 * **This function does nothing.**  Two instructions, eight bytes.
 *
 * The fourth of the byte-identical eight-byte stubs in this renderer region,
 * with `func_00102228`, `func_001029E0` and `func_001029E8`.  **Nothing in the
 * six-function group calls it**, which is the same position as
 * `func_00102228`: two copies that nothing reaches and two that the wrappers do
 * reach.
 *
 * That split is the one fact here that is worth having.  **A stub nothing
 * calls and a stub something calls compile identically**, so the linker
 * keeping both says nothing about intent - and it is exactly the pair that
 * makes "unreachable stub" and "feature compiled out on this platform" the two
 * readings that survive.  Neither is established.
 */
#include "types.h"

/** Returns immediately without doing anything. */
__attribute__((noreturn)) void func_001022B0(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "jr $ra\n\t"
        "nop\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}