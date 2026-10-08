/**
 * The Sims 2 PSP - func_0008ABEC (0x0008ABEC, 0x10 bytes)
 *
 * Stores 0.0f through the third argument, returns 0.
 *
 *     mtc1 $zero, $f12
 *     or   $v0, $zero, $zero
 *     jr   $ra
 *     swc1 $f12, 0x0($a2)
 *
 * **Stores 0.0f through the third argument ($a2), returns 0.**
 * Similar to func_00088E40 but uses $a2 instead of $t0 and returns 0.
 */
#include "types.h"

__attribute__((noreturn)) u32 func_0008ABEC(void *a0, void *a1, void *a2) {
    (void)a0; (void)a1; (void)a2;
    __asm__ __volatile__(
        "mtc1 $zero, $f12\n\t"
        "or   $v0, $zero, $zero\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "swc1 $f12, 0x0($a2)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$f12", "$v0");
}