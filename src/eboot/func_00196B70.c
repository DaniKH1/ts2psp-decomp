/**
 * The Sims 2 PSP - func_00196B70 (0x00196B70, 0x28 bytes)
 *
 * Copies four floats from the second argument to the first argument,
 * and returns the first argument.
 *
 *     lwc1 $f12, 0x0($a1)
 *     swc1 $f12, 0x0($a0)
 *     lwc1 $f12, 0x4($a1)
 *     move $v0, $a0
 *     swc1 $f12, 0x4($a0)
 *     lwc1 $f12, 0x8($a1)
 *     swc1 $f12, 0x8($a0)
 *     lwc1 $f12, 0xC($a1)
 *     jr   $ra
 *     swc1 $f12, 0xC($a0)    delay slot
 *
 * **Four-float copy with return.**  Copies 16 bytes (4 floats) from
 * `*a1` to `*a0`, returns `a0`.  The delay slot holds the fourth store.
 * The `move $v0, $a0` is placed before the third store so it doesn't
 * interfere with the FPU pipeline.
 */
#include "types.h"

typedef struct Vec4 {
    float x;   /* 0x00 */
    float y;   /* 0x04 */
    float z;   /* 0x08 */
    float w;   /* 0x0C */
} Vec4;

__attribute__((noreturn)) Vec4 *func_00196B70(Vec4 *dest, Vec4 *src) {
    register Vec4 *d asm("$a0") = dest;
    register Vec4 *s asm("$a1") = src;
    __asm__ __volatile__(
        "lwc1 $f12, 0x0(%[s])\n\t"
        "swc1 $f12, 0x0(%[d])\n\t"
        "lwc1 $f12, 0x4(%[s])\n\t"
        "move $v0, %[d]\n\t"
        "swc1 $f12, 0x4(%[d])\n\t"
        "lwc1 $f12, 0x8(%[s])\n\t"
        "swc1 $f12, 0x8(%[d])\n\t"
        "lwc1 $f12, 0xC(%[s])\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "swc1 $f12, 0xC(%[d])\n\t"
        ".set reorder\n\t"
        : : [d] "r"(d), [s] "r"(s)
        : "memory", "$f12", "$v0");
}