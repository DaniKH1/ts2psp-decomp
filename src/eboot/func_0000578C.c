/**
 * The Sims 2 PSP - func_0000578C (0x0000578C, 0x8 bytes)
 *
 *     jr    $ra
 *     lwc1  $f0, 0x14($a0)
 *
 * Returns immediately. The delay slot loads a float from `a0+0x14` into `$f0`.
 * This is likely a "getter" for a float field that the caller expects in `$f0`.
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