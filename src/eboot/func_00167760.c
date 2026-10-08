/**
 * The Sims 2 PSP - func_00167760 (0x00167760, 0x0C bytes)
 *
 * Loads the address of string "Chase" and returns it.
 *
 *     lui  $v0, %hi(str_Chase)
 *     jr   $ra
 *     addiu $v0, $v0, %lo(str_Chase)
 *
 * **Returns a string address.**
 */
#include "types.h"

__attribute__((noreturn)) char *func_00167760(void) {
    __asm__ __volatile__(
        "lui  $v0, %%hi(str_Chase)\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "addiu $v0, $v0, %%lo(str_Chase)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$v0");
}