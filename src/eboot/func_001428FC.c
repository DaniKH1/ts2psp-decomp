/**
 * The Sims 2 PSP - func_001428FC (0x001428FC, 0x14 bytes)
 *
 * Returns the absolute value of the argument.
 *
 *     move  $v0, $a0
 *     bltzl $a0, . + 4 + (0x1 << 2)
 *     negu  $v0, $a0
 *     jr    $ra
 *     nop
 *
 * **This is `abs()` using a branch-likely instruction.**  `bltzl` (branch on
 * less than zero likely) executes the delay slot instruction (`negu`) only
 * if the branch is taken.  So if `$a0` is negative, `negu` runs and `$v0`
 * becomes `-a0`; if non-negative, the branch is not taken, `negu` is
 * annulled, and `$v0` stays as `a0`.
 *
 * The `nop` after `jr` is the delay slot of the return - nothing useful to
 * put there since the return value is already in `$v0`.
 *
 * **`bltzl` is a branch-likely**, which means the delay slot instruction
 * is only executed if the branch is taken.  This is different from a
 * regular branch where the delay slot always executes.  This is exactly
 * what makes this `abs()` implementation work correctly without an extra
 * instruction.
 */
#include "types.h"

__attribute__((noreturn)) s32 func_001428FC(s32 value) {
    register s32 v asm("$a0") = value;
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "move $v0, %[v]\n\t"
        "bltzl %[v], 1f\n\t"
        "negu $v0, %[v]\n\t"
        "1: jr $ra\n\t"
        "nop\n\t"
        ".set reorder\n\t"
        : : [v] "r"(v)
        : "memory", "$v0");
}