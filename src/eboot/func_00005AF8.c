/**
 * The Sims 2 PSP - func_00005AF8 (0x00005AF8, 0x8 bytes)
 *
 *     jr    $ra
 *       ori   $v0, $zero, 0x4
 *
 * Returns the constant value 4 in $v0.
 * (Note: this is a duplicate of func_00005AF0 - both return 4.)
 */
#include "types.h"

__attribute__((noreturn)) void func_00005AF8(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "ori   $v0, $zero, 0x4\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}