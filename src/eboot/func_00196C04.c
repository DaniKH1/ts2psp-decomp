/**
 * The Sims 2 PSP - func_00196C04 (0x00196C04, 0x2C bytes)
 *
 *     addiu $sp, $sp, -0x20
 *     lw    $a2, 0x78($a0)
 *     addiu $a2, $a2, 0x78
 *     lh    $a3, 0x0($a2)
 *     lw    $a2, 0x4($a2)
 *     sw    $ra, 0x10($sp)
 *     jalr  $a2
 *       addu  $a0, $a0, $a3
 *     lw    $ra, 0x10($sp)
 *     jr    $ra
 *       addiu $sp, $sp, 0x20
 *
 * The fifth instance of the multiple-inheritance thunk in the module,
 * and **the first whose table is reached through two hops rather than
 * one.**
 *
 *     lw    $a2, 0x78($a0)      a pointer stored in this object
 *     addiu $a2, $a2, 0x78       the thunk array sits at +0x78 of it
 *     lh    $a3, 0x0($a2)        signed half-word adjustment
 *     lw    $a2, 0x4($a2)        function pointer
 *     jalr  $a2
 *       addu  $a0, $a0, $a3      this + adjustment
 *
 * **The other four read the thunk array out of `0x18($this)` at +0xD0.**
 * Here the path is `this -> 0x78 -> +0x78`, so the effective offset
 * within the reached object is **0xF0**, not 0xD0.  The idiom is
 * identical - a signed half-word at +0 and a pointer at +4, eight bytes
 * per entry, `$a0` formed in the delay slot - **so the thunk layout is
 * confirmed at a third base address and the two hops are what differ.**
 *
 * The `lh` is a **signed** load, which is what makes the adjustment able
 * to be negative; the same instruction appears in `func_0019D11C`,
 * `func_000BE138` and `func_000BD3A4`, and all four use `lh` rather
 * than `lhu`.
 *
 * **This is entry 3 of the three-table family at `0x1EC528`, `0x1ECD88`
 * and `0x1EDB78`, whose first three entries are `func_00196BE4` (returns
 * `FLT_MAX`), `func_00171268` (returns 0) and `func_00171280` (returns
 * 1).**  So that interface is a limit, a negative predicate, a positive
 * one, and a call-through - **and the call-through is the only one of the
 * four that costs more than eight bytes.**
 *
 * `$a1` is never set, so the thunk is called with one argument and this
 * function returns whatever it returns, in `$v0`.
 */
#include "types.h"

__attribute__((noreturn)) void func_00196C04(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "lw    $a2, 0x78($a0)\n\t"
        "addiu $a2, $a2, 0x78\n\t"
        "lh    $a3, 0x0($a2)\n\t"
        "lw    $a2, 0x4($a2)\n\t"
        "sw    $ra, 0x10($sp)\n\t"
        "jalr  $a2\n\t"
        "addu  $a0, $a0, $a3\n\t"
        "lw    $ra, 0x10($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}