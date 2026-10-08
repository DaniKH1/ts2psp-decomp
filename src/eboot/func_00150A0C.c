/**
 * The Sims 2 PSP - func_00150A0C (0x00150A0C, 0x0C bytes)
 *
 * Loads the address of string "WendToPoint" and returns it.
 *
 *     lui  $v0, %hi(str_WendToPoint)
 *     jr   $ra
 *     addiu $v0, $v0, %lo(str_WendToPoint)
 *
 * **Returns a string address.**
 */
#include "types.h"

__attribute__((noreturn)) char *func_00150A0C(void) {
    __asm__ __volatile__(
        "lui  $v0, %%hi(str_WendToPoint)\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "addiu $v0, $v0, %%lo(str_WendToPoint)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$v0");
}