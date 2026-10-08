/**
 * The Sims 2 PSP - func_001674FC (0x001674FC, 0x0C bytes)
 *
 * Loads the address of string "StartDecisionMaker" and returns it.
 *
 *     lui  $v0, %hi(str_StartDecisionMaker)
 *     jr   $ra
 *     addiu $v0, $v0, %lo(str_StartDecisionMaker)
 *
 * **Returns a string address.**
 */
#include "types.h"

__attribute__((noreturn)) char *func_001674FC(void) {
    __asm__ __volatile__(
        "lui  $v0, %%hi(str_StartDecisionMaker)\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "addiu $v0, $v0, %%lo(str_StartDecisionMaker)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$v0");
}