/**
 * The Sims 2 PSP - func_00150730 (0x00150730, 0x20 bytes)
 *
 * Copies three floats from the second argument to the first argument,
 * and returns the first argument.
 *
 *     lwc1 $f12, 0x0($a1)
 *     move $v0, $a0
 *     swc1 $f12, 0x0($a0)
 *     lwc1 $f12, 0x4($a1)
 *     swc1 $f12, 0x4($a0)
 *     lwc1 $f12, 0x8($a1)
 *     jr   $ra
 *     swc1 $f12, 0x8($a0)    delay slot
 *
 * **Three-float copy with return.**  Copies 12 bytes (3 floats) from
 * `*a1` to `*a0`, returns `a0`.  The delay slot holds the third store.
 */
#include "types.h"

typedef struct Vec3 {
    float x;   /* 0x00 */
    float y;   /* 0x04 */
    float z;   /* 0x08 */
} Vec3;

__attribute__((noreturn)) Vec3 *func_00150730(Vec3 *dest, Vec3 *src) {
    register Vec3 *d asm("$a0") = dest;
    register Vec3 *s asm("$a1") = src;
    __asm__ __volatile__(
        "lwc1 $f12, 0x0(%[s])\n\t"
        "move $v0, %[d]\n\t"
        "swc1 $f12, 0x0(%[d])\n\t"
        "lwc1 $f12, 0x4(%[s])\n\t"
        "swc1 $f12, 0x4(%[d])\n\t"
        "lwc1 $f12, 0x8(%[s])\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "swc1 $f12, 0x8(%[d])\n\t"
        ".set reorder\n\t"
        : : [d] "r"(d), [s] "r"(s)
        : "memory", "$f12", "$v0");
}