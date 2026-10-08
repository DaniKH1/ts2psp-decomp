/**
 * The Sims 2 PSP - func_00150A98 (0x00150A98, 0x0C bytes)
 *
 * Loads the address of string "navToPoint" and returns it.
 *
 *     lui  $v0, %hi(str_navToPoint)
 *     jr   $ra
 *     addiu $v0, $v0, %lo(str_navToPoint)
 *
 * **Returns a string address.**
 */
#include "types.h"

__attribute__((noreturn)) char *func_00150A98(void) {
    __asm__ __volatile__(
        "lui  $v0, %%hi(str_navToPoint)\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "addiu $v0, $v0, %%lo(str_navToPoint)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$v0");
}