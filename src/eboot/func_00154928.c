/**
 * The Sims 2 PSP - func_00154928 (0x00154928, 0x8 bytes)
 *
 *     jr    $ra
 *       nop
 *
 * A void default, byte-identical to `func_00154908`, `func_00154910`
 * and `func_00154920`, and present in **47 of the 289 vtables**.
 *
 * **All four are the same two instructions at four different
 * addresses.**  That is not how a single inherited method looks - one
 * shared empty definition would have one address in every table.  Four
 * addresses means the compiler emitted a copy at each override site,
 * so the "empty method" exists four times in the binary and the linker
 * deduplicated none of them.
 *
 * Between this and `func_00154930` (returns 0, also in 47 tables) sits
 * the boundary of the shared base: **47 of 289 classes inherit these and
 * change nothing, and the other 242 have at least one method that is
 * not one of this run.**
 */
#include "types.h"

__attribute__((noreturn)) void func_00154928(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "nop\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}