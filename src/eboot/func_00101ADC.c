/**
 * The Sims 2 PSP - func_00101ADC (0x00101ADC, 0x0C bytes)
 *
 * Loads the address of function 0x00063FF8 and returns it.
 *
 *     lui  $v0, %hi(func_00063FF8)
 *     jr   $ra
 *     addiu $v0, $v0, %lo(func_00063FF8)
 *
 * **Returns a function address.**  func_00063FF8 is the address.
 */
#include "types.h"

__attribute__((noreturn)) void *func_00101ADC(void) {
    __asm__ __volatile__(
        "lui  $v0, %%hi(func_00063FF8)\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "addiu $v0, $v0, %%lo(func_00063FF8)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$v0");
}