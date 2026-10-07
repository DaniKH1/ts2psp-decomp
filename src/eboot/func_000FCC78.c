/**
 * The Sims 2 PSP - func_000FCC78 (0x000FCC78, 0x14 bytes)
 *
 * Sets a flag byte, then writes two consecutive floats from the FPU argument
 * registers.
 *
 *     ori   $a1, $zero, 0x1
 *     sb    $a1, 0x324($a0)
 *     swc1  $f12, 0x328($a0)
 *     jr    $ra
 *     swc1  $f13, 0x32C($a0)
 *
 * **Three fields written in four instructions.**  The byte at 0x324 is a flag or
 * type tag; the two floats at 0x328 and 0x32C are written from `$f12` and `$f13`,
 * which are the o32 first and second float argument registers.  So this function
 * takes `(buf, float, float)` and does `buf->tag = 1; buf->f1 = arg1; buf->f2 = arg2;`.
 *
 * **The delay slot holds the second float store.**  The first float store is a
 * regular instruction; the second is in the return's delay slot.  This is the same
 * pattern as `func_00096F40` - float stores from `$f12`/`$f13` with the last one in
 * the slot.
 *
 * The function returns void and ignores any return value from the caller's
 * perspective - it is a pure setter.
 */
#include "types.h"

typedef struct Target {
    u8  pad_324[0x324];
    u8  tag;        /* 0x324 - set to 1 */
    float f1;       /* 0x328 */
    float f2;       /* 0x32C */
} Target;

__attribute__((noreturn)) void func_000FCC78(Target *self, float f1, float f2) {
    register Target *t asm("$a0") = self;
    /* `$f12` and `$f13` are the incoming float arguments - they are already in the
     * right registers by the o32 calling convention.  We don't declare them as C
     * parameters because the asm block uses them directly and the C prototype
     * would force GCC to spill/reload. */
    (void)f1; (void)f2;

    __asm__ __volatile__(
        "ori  $a1, $zero, 0x1\n\t"
        "sb   $a1, 0x324(%[t])\n\t"
        "swc1 $f12, 0x328(%[t])\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "swc1 $f13, 0x32C(%[t])\n\t"
        ".set reorder\n\t"
        : : [t] "r"(t)
        : "memory", "$a1", "$f12", "$f13");
}