/**
 * The Sims 2 PSP - func_0000537C (0x0000537C, 0xC bytes)
 *
 *     ori   $a1, $zero, 0x1
 *     jr    $ra
 *       sb    $a1, 0x28($a0)
 *
 * Sets $a1 to 1, returns, and stores the byte value 1 to offset 0x28
 * of the object pointed to by $a0 in the delay slot of the return.
 */
#include "types.h"

__attribute__((noreturn)) void func_0000537C(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "ori   $a1, $zero, 0x1\n\t"
        "jr    $ra\n\t"
        "sb    $a1, 0x28($a0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}