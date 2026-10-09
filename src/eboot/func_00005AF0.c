/**
 * The Sims 2 PSP - func_00005AF0 (0x00005AF0, 0x8 bytes)
 *
 *     jr    $ra
 *       ori   $v0, $zero, 0x4
 *
 * Returns the constant value 4 in $v0.
 */
#include "types.h"

__attribute__((noreturn)) void func_00005AF0(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "ori   $v0, $zero, 0x4\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}