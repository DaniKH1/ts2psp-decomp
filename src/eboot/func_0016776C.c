/**
 * The Sims 2 PSP - func_0016776C (0x0016776C, 0x0C bytes)
 *
 * Loads the address of string "TargetPlayer" and returns it.
 *
 *     lui  $v0, %hi(str_TargetPlayer)
 *     jr   $ra
 *     addiu $v0, $v0, %lo(str_TargetPlayer)
 *
 * **Returns a string address.**
 */
#include "types.h"

__attribute__((noreturn)) char *func_0016776C(void) {
    __asm__ __volatile__(
        "lui  $v0, %%hi(str_TargetPlayer)\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "addiu $v0, $v0, %%lo(str_TargetPlayer)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$v0");
}