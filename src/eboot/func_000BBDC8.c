/**
 * The Sims 2 PSP - func_000BBDC8 (0x000BBDC8, 0xB0 bytes)
 *
 * The slot +0x30 override of `sym_001EA4B8`.  It reads two 12-byte
 * records out of the object, hands them plus two scalars to
 * func_00126C94, and then copies three floats back out of the callee's
 * output area into the object at 0x1C, 0x20 and 0x24.
 *
 * **Both input records are 12 bytes: three words copied one at a time.**
 * The first comes from `0x8($s0) + 4`, the second from
 * `0x8($s0) + 0x10`.  Neither is loaded as a unit - each is three
 * `lw` and three `sw` into a stack slot - which is what a
 * non-4-byte-aligned or non-POD source type compiles to.
 *
 * **The output copy is deliberately split: three `lwc1` / `lw $a0` /
 * `swc1` triples, with the base pointer reloaded from `0x8($s0)`
 * between each.**  `lw $a0, 0x8($s0)` appears three times.  That is
 * redundant in any ordinary reading - `$a0` was not clobbered by
 * `lwc1` or `swc1` - so the reloads are the compiler declining to
 * assume the fields at 0x8 are stable across the stores, or simply
 * re-reading a pointer it has no register budget to keep.
 *
 * The scalars are: the float at 0x0 of the first record truncated to an
 * integer (`trunc.w.s` + `mfc1`, stored to 0x20($sp)), and the constant
 * 2 handed to the callee.  **`ori $t0, $zero, 0x1` sits in the delay
 * slot**, which is how a fifth argument gets materialised without
 * spending an instruction.
 *
 * Note `trunc.w.s` traps on out-of-range input, so the record's first
 * float has to be within int range for this to be safe.
 */
#include "types.h"

__attribute__((noreturn)) void func_000BBDC8(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x50\n\t"
        "sw    $s0, 0x48($sp)\n\t"
        "or    $s0, $a0, $zero\n\t"
        "lw    $a0, 0x8($s0)\n\t"
        "lwc1  $f12, 0x0($a0)\n\t"
        "trunc.w.s $f12, $f12\n\t"
        "mfc1  $a3, $f12\n\t"
        "addiu $a2, $a0, 0x4\n\t"
        "sw    $a3, 0x20($sp)\n\t"
        "lw    $a3, 0x0($a2)\n\t"
        "lw    $t0, 0x4($a2)\n\t"
        "addiu $a1, $sp, 0x24\n\t"
        "lw    $a2, 0x8($a2)\n\t"
        "sw    $a3, 0x0($a1)\n\t"
        "sw    $t0, 0x4($a1)\n\t"
        "sw    $a2, 0x8($a1)\n\t"
        "lw    $a2, 0x8($s0)\n\t"
        "addiu $a2, $a2, 0x10\n\t"
        "lw    $t0, 0x0($a2)\n\t"
        "lw    $t1, 0x4($a2)\n\t"
        "addiu $a3, $sp, 0x30\n\t"
        "lw    $a2, 0x8($a2)\n\t"
        "sw    $t0, 0x0($a3)\n\t"
        "sw    $t1, 0x4($a3)\n\t"
        "sw    $a2, 0x8($a3)\n\t"
        "addiu $a0, $sp, 0x20\n\t"
        "addiu $a3, $sp, 0x3C\n\t"
        "ori   $a2, $zero, 0x2\n\t"
        "sw    $ra, 0x4C($sp)\n\t"
        "jal   func_00126C94\n\t"
        "ori   $t0, $zero, 0x1\n\t"
        "lwc1  $f12, 0x3C($sp)\n\t"
        "lw    $a0, 0x8($s0)\n\t"
        "swc1  $f12, 0x1C($a0)\n\t"
        "lwc1  $f12, 0x40($sp)\n\t"
        "lw    $a0, 0x8($s0)\n\t"
        "swc1  $f12, 0x20($a0)\n\t"
        "lwc1  $f12, 0x44($sp)\n\t"
        "lw    $a0, 0x8($s0)\n\t"
        "swc1  $f12, 0x24($a0)\n\t"
        "lw    $s0, 0x48($sp)\n\t"
        "lw    $ra, 0x4C($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x50\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}