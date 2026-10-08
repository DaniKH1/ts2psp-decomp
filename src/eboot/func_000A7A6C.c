/**
 * The Sims 2 PSP - func_000A7A6C (0x000A7A6C, 0x18 bytes)
 *
 * Stores 1.0f to three consecutive float slots at offsets 0, 4, 8
 * of the second argument.
 *
 *     lui  $a0, 0x3F80  (1.0f in upper 16 bits)
 *     mtc1 $a0, $f12
 *     swc1 $f12, 0x0($a1)
 *     swc1 $f12, 0x4($a1)
 *     jr   $ra
 *     swc1 $f12, 0x8($a1)
 *
 * **Sets three floats to 1.0f.**  0x3F800000 is the IEEE 754
 * representation of 1.0f. The value is moved to $f12 via mtc1,
 * then stored three times. The delay slot holds the third store.
 */
#include "types.h"

typedef struct Target {
    u8 pad[0x0];
    float f1;  /* 0x00 - set to 1.0f */
    float f2;  /* 0x04 - set to 1.0f */
    float f3;  /* 0x08 - set to 1.0f */
} Target;

__attribute__((noreturn)) void func_000A7A6C(void *self, Target *target) {
    (void)self;
    register Target *t asm("$a1") = target;
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "lui  $a0, 0x3F80\n\t"
        "mtc1 $a0, $f12\n\t"
        "swc1 $f12, 0x0(%[t])\n\t"
        "swc1 $f12, 0x4(%[t])\n\t"
        "jr   $ra\n\t"
        "swc1 $f12, 0x8(%[t])\n\t"
        ".set reorder\n\t"
        : : [t] "r"(t)
        : "memory", "$a0", "$f12");
}