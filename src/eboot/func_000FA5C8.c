/**
 * The Sims 2 PSP - func_000FA5C8 (0x000FA5C8, 0x1C bytes)
 *
 * Float arithmetic: loads f13 from 0x8(a1), f14 from 0x3C(a0),
 * computes f13 = f12 - f13, then f13 = f14 + f13, stores result
 * to 0x3C(a0), and stores f12 to 0x8(a1) in delay slot.
 *
 *     lwc1 $f13, 0x8($a1)
 *     lwc1 $f14, 0x3C($a0)
 *     sub.s $f13, $f12, $f13
 *     add.s $f13, $f14, $f13
 *     swc1 $f13, 0x3C($a0)
 *     jr   $ra
 *     swc1 $f12, 0x8($a1)    delay slot
 *
 * **Computes: `*a0_3C = *a0_3C + (f12 - *a1_08)` and `*a1_08 = f12`.**
 * The incoming f12 is both used in the computation and stored back
 * to a1+8. The result is stored back to a0+0x3C.
 *
 * Arguments: a0 = destination structure, a1 = source structure,
 * f12 = incoming float value.
 */
#include "types.h"

__attribute__((noreturn)) void func_000FA5C8(void *dst, void *src) {
    register void *d asm("$a0") = dst;
    register void *s asm("$a1") = src;
    (void)d; (void)s;
    __asm__ __volatile__(
        "lwc1 $f13, 0x8(%[s])\n\t"
        "lwc1 $f14, 0x3C(%[d])\n\t"
        "sub.s $f13, $f12, $f13\n\t"
        "add.s $f13, $f14, $f13\n\t"
        "swc1 $f13, 0x3C(%[d])\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "swc1 $f12, 0x8(%[s])\n\t"
        ".set reorder\n\t"
        : : [d] "r"(d), [s] "r"(s)
        : "memory", "$f12", "$f13", "$f14");
}