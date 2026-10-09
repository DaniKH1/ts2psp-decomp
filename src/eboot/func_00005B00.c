/**
 * The Sims 2 PSP - func_00005B00 (0x00005B00, 0x8 bytes)
 *
 *     jr    $ra
 *       or    $v0, $zero, $zero
 *
 * Returns 0 in $v0.
 */
#include "types.h"

__attribute__((noreturn)) void func_00005B00(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "or    $v0, $zero, $zero\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}