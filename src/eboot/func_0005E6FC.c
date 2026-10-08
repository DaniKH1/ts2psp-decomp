/**
 * The Sims 2 PSP - func_0005E6FC (0x0005E6FC, 0x0C bytes)
 *
 * Loads a word from offset 0x140, returns 1 if it's negative.
 *
 *     lw   $v0, 0x140($a0)
 *     jr   $ra
 *     sltu $v0, $zero, $v0
 *
 * **Branchless negative check.**  `sltu $v0, $zero, $v0` returns 1 if
 * $v0 is negative (since negative numbers have MSB=1, so as unsigned
 * they are >= 2^31, which is not < 1, wait - sltu compares unsigned).
 * Actually: sltu $v0, $zero, $v0 returns 1 if 0 < $v0 (unsigned), i.e.
 * if $v0 != 0. That's not a negative check.
 *
 * Wait, sltu with $zero as first arg: sltu $v0, $zero, $v0 means
 * $v0 = (0 < $v0_unsigned) ? 1 : 0. So it returns 1 if the value is
 * non-zero (as unsigned), 0 if zero. That's a "not zero" check.
 */
#include "types.h"

typedef struct Target {
    u8 pad[0x140];
    u32 value;  /* 0x140 - checked for non-zero */
} Target;

__attribute__((noreturn)) u32 func_0005E6FC(Target *self) {
    register Target *t asm("$a0") = self;
    __asm__ __volatile__(
        "lw   $v0, 0x140(%[t])\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "sltu $v0, $zero, $v0\n\t"
        ".set reorder\n\t"
        : : [t] "r"(t)
        : "memory", "$v0");
}