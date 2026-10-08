/**
 * The Sims 2 PSP - func_000E3C5C (0x000E3C5C, 0x14 bytes)
 *
 * Stores a float from $f12 to offset 0x54, sets a byte to 1 at offset 0x40.
 *
 *     swc1 $f12, 0x54($a0)
 *     ori  $a1, $zero, 0x1
 *     jr   $ra
 *     sb   $a1, 0x40($a0)
 *
 * **Stores a float and a flag byte.**  The float from $f12 goes to offset 0x54,
 * and a flag byte (value 1) goes to offset 0x40. The delay slot holds the byte store.
 */
#include "types.h"

__attribute__((noreturn)) void func_000E3C5C(void *self, float f) {
    (void)f;
    register void *p asm("$a0") = self;
    __asm__ __volatile__(
        "swc1 $f12, 0x54(%[p])\n\t"
        "ori  $a1, $zero, 0x1\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "sb   $a1, 0x40(%[p])\n\t"
        ".set reorder\n\t"
        : [p] "=r"(p)
        :
        : "memory", "$f12");
}