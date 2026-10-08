/**
 * The Sims 2 PSP - func_0009C9A0 (0x0009C9A0, 0x14 bytes)
 *
 * Clears a word at offset 0x8, then stores three floats (all 0.0f)
 * at offsets 0, 4, 8. Returns the destination.
 *
 *     mtc1 $zero, $f12
 *     sw   $zero, 0x8($a0)
 *     swc1 $f12, 0x0($a0)
 *     jr   $ra
 *     or   $v0, $a0, $zero
 *
 * **Clears a word, stores three floats.**  Returns the destination.
 * The delay slot does `or $v0, $a0, $zero` (move self to return).
 */
#include "types.h"

typedef struct Target {
    float f1;    /* 0x00 - 0.0f */
    float f2;    /* 0x04 - 0.0f */
    float f3;    /* 0x08 - 0.0f */
    u32 cleared; /* 0x0C - cleared to 0 */
} Target;

__attribute__((noreturn)) Target *func_0009C9A0(Target *self) {
    register Target *t asm("$a0") = self;
    __asm__ __volatile__(
        "mtc1 $zero, $f12\n\t"
        "sw   $zero, 0x8(%[t])\n\t"
        "swc1 $f12, 0x0(%[t])\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "or   $v0, %[t], $zero\n\t"
        ".set reorder\n\t"
        : [t] "+r"(t)
        :
        : "memory", "$f12", "$v0");
}