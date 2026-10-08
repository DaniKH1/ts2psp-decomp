/**
 * The Sims 2 PSP - func_00150B40 (0x00150B40, 0x0C bytes)
 *
 * Loads the address of string "NavToCritter" and returns it.
 *
 *     lui  $v0, %hi(str_NavToCritter)
 *     jr   $ra
 *     addiu $v0, $v0, %lo(str_NavToCritter)
 *
 * **Returns a string address.**
 */
#include "types.h"

__attribute__((noreturn)) char *func_00150B40(void) {
    __asm__ __volatile__(
        "lui  $v0, %%hi(str_NavToCritter)\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "addiu $v0, $v0, %%lo(str_NavToCritter)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$v0");
}