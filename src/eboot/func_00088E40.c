/**
 * The Sims 2 PSP - func_00088E40 (0x00088E40, 0x10 bytes)
 *
 * Stores 0.0f through the third argument, returns 1.
 *
 *     mtc1 $zero, $f12
 *     ori  $v0, $zero, 0x1
 *     jr   $ra
 *     swc1 $f12, 0x0($t0)
 *
 * **Stores 0.0f through the third argument ($t0), returns 1.**
 * The third argument is passed in $t0 (non-standard register),
 * similar to func_0000BEDC.
 */
#include "types.h"

__attribute__((noreturn)) u32 func_00088E40(void *a0, void *a1, void *t0) {
    (void)a0; (void)a1; (void)t0;
    __asm__ __volatile__(
        "mtc1 $zero, $f12\n\t"
        "ori  $v0, $zero, 0x1\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "swc1 $f12, 0x0($t0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$f12", "$v0", "$t0");
}