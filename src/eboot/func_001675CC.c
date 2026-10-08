/**
 * The Sims 2 PSP - func_001675CC (0x001675CC, 0x0C bytes)
 *
 * Loads the address of string "StopDecisionMaker" and returns it.
 *
 *     lui  $v0, %hi(str_StopDecisionMaker)
 *     jr   $ra
 *     addiu $v0, $v0, %lo(str_StopDecisionMaker)
 *
 * **Returns a string address.**
 */
#include "types.h"

__attribute__((noreturn)) char *func_001675CC(void) {
    __asm__ __volatile__(
        "lui  $v0, %%hi(str_StopDecisionMaker)\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "addiu $v0, $v0, %%lo(str_StopDecisionMaker)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$v0");
}