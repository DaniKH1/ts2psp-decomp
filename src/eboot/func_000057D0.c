/**
 * The Sims 2 PSP - func_000057D0 (0x000057D0, 0x8 bytes)
 *
 *     jr    $ra
 *     lwc1  $f0, 0x18($a0)
 *
 * Returns immediately. The delay slot loads a float from `a0+0x18` into `$f0`.
 * This is a "getter" for a float field at offset 0x18.
 */
#include "types.h"

__attribute__((noreturn)) void func_000057D0(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "lwc1  $f0, 0x18($a0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}