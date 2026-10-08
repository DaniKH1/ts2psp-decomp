/**
 * The Sims 2 PSP - func_0000BEDC (0x0000BEDC, 0x10 bytes)
 *
 * Sets a float to 0.0f at offset 0 of the third argument, returns 1.
 *
 *     mtc1 $zero, $f12
 *     ori  $v0, $zero, 0x1
 *     jr   $ra
 *     swc1 $f12, 0x0($t0)
 *
 * **Stores 0.0f through the third argument ($t0), returns 1.**  The third
 * argument is passed in $t0 (not a standard argument register), suggesting
 * a non-standard calling convention or a specialized helper.
 */
#include "types.h"

__attribute__((noreturn)) u32 func_0000BEDC(void *a0, void *a1, void *t0) {
    (void)a0; (void)a1;
    register void *t asm("$t0");
    __asm__ __volatile__(
        "mtc1 $zero, $f12\n\t"
        "ori  $v0, $zero, 0x1\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "swc1 $f12, 0x0(%[t])\n\t"
        ".set reorder\n\t"
        : : [t] "r"(t)
        : "memory", "$f12", "$v0");
}