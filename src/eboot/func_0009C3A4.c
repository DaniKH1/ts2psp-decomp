/**
 * The Sims 2 PSP - func_0009C3A4 (0x0009C3A4, 0x18 bytes)
 *
 * Stores a word, then three floats to a structure, returns the structure.
 *
 *     mtc1 $zero, $f12
 *     sw   $a1, 0x0($a0)
 *     swc1 $f12, 0x4($a0)
 *     swc1 $f12, 0x8($a0)
 *     jr   $ra
 *     or   $v0, $a0, $zero
 *
 * **Writes a word then three floats.**  The first argument is the destination,
 * second is a word to store at offset 0, then three floats (all 0.0f) at
 * offsets 4, 8, and 0xC (delay slot). Returns the destination.
 */
#include "types.h"

typedef struct Target {
    u32 word0;   /* 0x00 - from $a1 */
    float f1;    /* 0x04 - 0.0f */
    float f2;    /* 0x08 - 0.0f */
    float f3;    /* 0x0C - 0.0f */
} Target;

__attribute__((noreturn)) Target *func_0009C3A4(Target *self, u32 word) {
    register Target *t asm("$a0") = self;
    register u32 w asm("$a1") = word;
    __asm__ __volatile__(
        "mtc1 $zero, $f12\n\t"
        "sw   %[w], 0x0(%[t])\n\t"
        "swc1 $f12, 0x4(%[t])\n\t"
        "swc1 $f12, 0x8(%[t])\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "or   $v0, %[t], $zero\n\t"
        ".set reorder\n\t"
        : [t] "+r"(t)
        : [w] "r"(w)
        : "memory", "$f12", "$v0");
}