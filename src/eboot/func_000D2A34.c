/**
 * The Sims 2 PSP - func_000D2A34 (0x000D2A34, 0x20 bytes)
 *
 * Sets a flag byte, copies a word from the second argument, and writes three
 * consecutive floats from the FPU argument registers.
 *
 *     ori   $a3, $zero, 0x1
 *     sb    $a3, 0x84($a0)
 *     sw    $a2, 0xBC($a0)
 *     swc1  $f13, 0xA4($a0)
 *     swc1  $f14, 0xA8($a0)
 *     swc1  $f12, 0xB0($a0)
 *     jr    $ra
 *     sw    $a1, 0x180($a0)
 *
 * **Six fields written in seven instructions.**  The byte at 0x84 is a flag; the
 * word at 0xBC comes from `$a2` (the third integer argument); three floats at
 * 0xA4, 0xA8, 0xB0 come from `$f13`, `$f14`, `$f12` - note the order: the first
 * two are the second and third float arguments, and the third is the first float
 * argument.  That order is unusual and suggests the source passed them that way
 * explicitly, or the compiler reordered them.
 *
 * **The delay slot holds an integer store from `$a1`.**  The second integer
 * argument is written at 0x180 *after* the return.  So this function takes
 * `(buf, u32, u32, float, float, float)` and writes all six fields.
 *
 * **Float register order is the tell.**  `$f12` is the first float argument,
 * `$f13` the second, `$f14` the third.  Writing them as `f13, f14, f12` means the
 * source passed the floats in a different order than the buffer layout, or the
 * compiler did a deliberate permutation.  Either way, the transcription must
 * preserve the exact register-to-offset mapping.
 */
#include "types.h"

typedef struct Target {
    u8  pad_84[0x84];
    u8  flag;      /* 0x84 - set to 1 */
    u8  pad_88[0x34];
    u32 word_bc;   /* 0xBC - from arg 2 */
    u8  pad_c0[0x24];
    float f_a4;    /* 0xA4 - from $f13 (2nd float arg) */
    float f_a8;    /* 0xA8 - from $f14 (3rd float arg) */
    float f_b0;    /* 0xB0 - from $f12 (1st float arg) */
    u8  pad_b4[0xCC];
    u32 word_180;  /* 0x180 - from arg 1, in delay slot */
} Target;

__attribute__((noreturn)) void func_000D2A34(Target *self, u32 arg1, u32 arg2,
                                             float f1, float f2, float f3) {
    register Target *t asm("$a0") = self;
    (void)arg1; (void)arg2; (void)f1; (void)f2; (void)f3;

    __asm__ __volatile__(
        "ori  $a3, $zero, 0x1\n\t"
        "sb   $a3, 0x84(%[t])\n\t"
        "sw   $a2, 0xBC(%[t])\n\t"
        "swc1 $f13, 0xA4(%[t])\n\t"
        "swc1 $f14, 0xA8(%[t])\n\t"
        "swc1 $f12, 0xB0(%[t])\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "sw   $a1, 0x180(%[t])\n\t"
        ".set reorder\n\t"
        : : [t] "r"(t)
        : "memory", "$a1", "$a2", "$a3", "$f12", "$f13", "$f14");
}