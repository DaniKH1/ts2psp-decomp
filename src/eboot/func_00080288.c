/**
 * The Sims 2 PSP - func_00080288 (0x00080288, 0x8 bytes)
 *
 *     jr    $ra
 *       lw    $v0, 0x0($a0)
 *
 * The fifth of the eight offset-0 getters and the first of the third
 * adjacent pair; see func_0004E5CC.
 */
#include "types.h"

__attribute__((noreturn)) void func_00080288(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "lw    $v0, 0x0($a0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}