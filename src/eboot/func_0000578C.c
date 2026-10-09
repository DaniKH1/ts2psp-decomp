/**
 * The Sims 2 PSP - func_0000578C (0x0000578C, 0x8 bytes)
 *
 *     jr    $ra
 *       lwc1  $f0, 0x14($a0)
 *
 * Loads a float from offset 0x14 of the object pointed to by $a0
 * into floating-point register $f0, then returns.
 * The load is placed in the delay slot of the return instruction.
 */
#include "types.h"

__attribute__((noreturn)) void func_0000578C(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "lwc1  $f0, 0x14($a0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}