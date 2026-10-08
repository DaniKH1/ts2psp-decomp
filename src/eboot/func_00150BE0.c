/**
 * The Sims 2 PSP - func_00150BE0 (0x00150BE0, 0x0C bytes)
 *
 * Loads the address of string "navToSeat" and returns it.
 *
 *     lui  $v0, %hi(str_navToSeat)
 *     jr   $ra
 *     addiu $v0, $v0, %lo(str_navToSeat)
 *
 * **Returns a string address.**
 */
#include "types.h"

__attribute__((noreturn)) char *func_00150BE0(void) {
    __asm__ __volatile__(
        "lui  $v0, %%hi(str_navToSeat)\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "addiu $v0, $v0, %%lo(str_navToSeat)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$v0");
}