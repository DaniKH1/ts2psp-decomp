/**
 * The Sims 2 PSP - func_0004E5CC (0x0004E5CC, 0x8 bytes)
 *
 *     jr    $ra
 *       lw    $v0, 0x0($a0)
 *
 * A field getter returning the word at offset 0 of the object.
 *
 * **There are eight identical getters at offset 0, and three of them sit
 * in adjacent address pairs**: func_0004E5CC/func_0004E5D4,
 * func_00058078/func_00058080 and func_00080288/func_00080290.  **An
 * adjacent pair 8 bytes apart is the same getter emitted twice** - the
 * compiler inlining it into one function that needs the value in two
 * places, where the inlined copy had to be a real function body
 * because the address was taken.
 *
 * Offset 0 is the object's first field, so these return a primary
 * pointer or handle rather than a scalar.
 */
#include "types.h"

__attribute__((noreturn)) void func_0004E5CC(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "lw    $v0, 0x0($a0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}