/**
 * The Sims 2 PSP - func_00085B3C (0x00085B3C, 0x180 bytes)
 *
 * Advances two float fields by a time delta, then runs a fixed sequence
 * of five phases whose bodies live in the object's own vtable.
 *
 *     addiu $sp, $sp, -0x20
 *     sw    $s0, 0x10($sp)
 *     sw    $ra, 0x14($sp)
 *     jal   func_0008541C
 *       or    $s0, $a0, $zero
 *     lwc1  $f12, 0x1C($v0)
 *     lwc1  $f13, 0x20($s0)
 *     add.s $f13, $f13, $f12
 *     lwc1  $f14, 0x24($s0)
 *     lbu   $a0, 0x30($s0)
 *     add.s $f12, $f14, $f12
 *     swc1  $f13, 0x20($s0)
 *     beqz  $a0, .Leboot_00085B98
 *       swc1  $f12, 0x24($s0)
 *     ori   $a0, $zero, 0x3
 *     lw    $a1, 0x38($s0)
 *     sw    $a0, 0x18($s0)
 *     ...  phases at +0x40, +0x50, +0x28, +0x30, +0x38 ...
 *     lw    $v0, 0x18($s0)
 *     lw    $s0, 0x10($sp)
 *     lw    $ra, 0x14($sp)
 *     jr    $ra
 *       addiu $sp, $sp, 0x20
 *
 * Two float fields at `0x20` and `0x24` are advanced by a delta read from
 * `0x1C` of whatever `func_0008541C` returns, and then five virtual phases
 * of the object's own class are called in a fixed order.
 *
 * **The five phases are the class's own virtual methods, called
 * non-virtually, in a fixed order.**  Each is the same eight words -
 * load `0x38($s0)`, add a constant offset, `lh` the adjustment, `lw` the
 * pointer, `jalr` with `$a0 = this + adjustment` - at offsets
 *
 *     +0x40   +0x50   +0x28   +0x30   +0x38
 *
 * which are entries 8, 10, 5, 6 and 7 of the primary table.  **This is
 * the same mechanism as `func_00085AE4` and `func_00085B10`, but used
 * for a whole update sequence rather than one qualified base call, and
 * here the order is not the table's order** - +0x40 runs first and +0x28
 * only after the state machine has admitted it.
 *
 * **The very first `beqz` splits the function in two.**  `0x30($s0)` is
 * a one-byte flag: when it is clear the code jumps straight past a block
 * that would store the constant **3** into `0x18($s0)` - the state field -
 * run phase +0x40, and then **clear the flag** with `sb $zero, 0x30($s0)`.
 * So a non-zero `0x30` means "reset me into state 3 and run one phase",
 * and the flag is self-clearing.  **`0x18` is set to 3 in exactly one
 * place in the whole function**, and it is here.
 *
 * **Which means the phases a class actually executes are decided by
 * whether it overrode them.**  Entries 7 and 8 of the primary table at
 * `0x1E5D60` are `func_00154920` and `func_00154928`, both `jr $ra` /
 * `nop`, so for a class that overrides nothing two of these five calls
 * reach empty bodies.  The other three go to whatever the class supplies.
 *
 * **The state machine is a byte plus three comparisons.**  `0x18($s0)` is
 * tested against 3, then 1, 4 and 5; state 3 falls into the retry loop
 * at `.Leboot_00085BF0`, the other three jump to `.Leboot_00085C88`.
 * Whatever the value, the function ends by returning `0x18($s0)` -
 * **the state is both the input and the output**, which is what a
 * re-entered state machine looks like when its caller feeds the result
 * back.
 *
 * `0x2C($s0)` is a counter and `0x28($s0)` its limit: the loop increments
 * the counter, compares against the limit, and retries while it is
 * smaller.  `0x30` and `0x31` are one-byte flags; `0x31` is tested, then
 * cleared with `sb $zero` before a phase runs, and `0x30` is tested twice
 * - once at the top and once at `.Leboot_00085C34`.
 *
 * **One phase zeroes a field it has just advanced.**  The last block
 * reads `0x24($s0)` into `$f12`, calls the phase at +0x30, then writes
 * `$f12 = 0.0f` back to `0x24` and the counter to 0 - so the float is
 * loaded, used as an argument by the callee through the object, and then
 * discarded.  Whether it is a "time since event" field being reset or a
 * leftover from the accumulation above is not decidable here.
 */
#include "types.h"

__attribute__((noreturn)) void func_00085B3C(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "sw    $s0, 0x10($sp)\n\t"
        "sw    $ra, 0x14($sp)\n\t"
        "jal   func_0008541C\n\t"
        "or    $s0, $a0, $zero\n\t"
        "lwc1  $f12, 0x1C($v0)\n\t"
        "lwc1  $f13, 0x20($s0)\n\t"
        "add.s $f13, $f13, $f12\n\t"
        "lwc1  $f14, 0x24($s0)\n\t"
        "lbu   $a0, 0x30($s0)\n\t"
        "add.s $f12, $f14, $f12\n\t"
        "swc1  $f13, 0x20($s0)\n\t"
        "beqz  $a0, .Leboot_00085B98\n\t"
        "swc1  $f12, 0x24($s0)\n\t"
        "ori   $a0, $zero, 0x3\n\t"
        "lw    $a1, 0x38($s0)\n\t"
        "sw    $a0, 0x18($s0)\n\t"
        "addiu $a0, $a1, 0x40\n\t"
        "lh    $a1, 0x0($a0)\n\t"
        "lw    $a2, 0x4($a0)\n\t"
        "jalr  $a2\n\t"
        "addu  $a0, $s0, $a1\n\t"
        "sb    $zero, 0x30($s0)\n\t"
        ".Leboot_00085B98:\n\t"
        "jal   func_0009140C\n\t"
        "addiu $a0, $s0, 0x10\n\t"
        "bnez  $v0, .Leboot_00085BE0\n\t"
        "nop\n\t"
        "lw    $a0, 0x38($s0)\n\t"
        "addiu $a0, $a0, 0x50\n\t"
        "lh    $a1, 0x0($a0)\n\t"
        "lw    $a2, 0x4($a0)\n\t"
        "jalr  $a2\n\t"
        "addu  $a0, $s0, $a1\n\t"
        "bnez  $v0, .Leboot_00085BE0\n\t"
        "nop\n\t"
        "lw    $a0, 0x18($s0)\n\t"
        "ori   $a1, $zero, 0x3\n\t"
        "beql  $a0, $a1, .Leboot_00085BF0\n\t"
        "lw    $a1, 0x2C($s0)\n\t"
        "b     .Leboot_00085CA8\n\t"
        "nop\n\t"
        ".Leboot_00085BE0:\n\t"
        "jal   func_00085A30\n\t"
        "or    $a0, $s0, $zero\n\t"
        "b     .Leboot_00085CAC\n\t"
        "lw    $v0, 0x18($s0)\n\t"
        ".Leboot_00085BF0:\n\t"
        "lw    $a2, 0x28($s0)\n\t"
        "addiu $a1, $a1, 0x1\n\t"
        "sw    $a1, 0x2C($s0)\n\t"
        "slt   $a1, $a1, $a2\n\t"
        "bnez  $a1, .Leboot_00085CA8\n\t"
        "nop\n\t"
        "lbu   $a1, 0x31($s0)\n\t"
        "beqz  $a1, .Leboot_00085C34\n\t"
        "nop\n\t"
        "lw    $a0, 0x38($s0)\n\t"
        "sb    $zero, 0x31($s0)\n\t"
        "addiu $a0, $a0, 0x28\n\t"
        "lh    $a1, 0x0($a0)\n\t"
        "lw    $a2, 0x4($a0)\n\t"
        "jalr  $a2\n\t"
        "addu  $a0, $s0, $a1\n\t"
        "lw    $a0, 0x18($s0)\n\t"
        ".Leboot_00085C34:\n\t"
        "ori   $a2, $zero, 0x1\n\t"
        "beq   $a0, $a2, .Leboot_00085C88\n\t"
        "lbu   $a1, 0x30($s0)\n\t"
        "ori   $a2, $zero, 0x4\n\t"
        "beq   $a0, $a2, .Leboot_00085C88\n\t"
        "ori   $a2, $zero, 0x5\n\t"
        "beq   $a0, $a2, .Leboot_00085C88\n\t"
        "nop\n\t"
        "bnez  $a1, .Leboot_00085C88\n\t"
        "nop\n\t"
        "lw    $a0, 0x38($s0)\n\t"
        "lwc1  $f12, 0x24($s0)\n\t"
        "addiu $a0, $a0, 0x30\n\t"
        "lh    $a1, 0x0($a0)\n\t"
        "lw    $a2, 0x4($a0)\n\t"
        "jalr  $a2\n\t"
        "addu  $a0, $s0, $a1\n\t"
        "sw    $zero, 0x2C($s0)\n\t"
        "mtc1  $zero, $f12\n\t"
        "swc1  $f12, 0x24($s0)\n\t"
        "lbu   $a1, 0x30($s0)\n\t"
        ".Leboot_00085C88:\n\t"
        "beqz  $a1, .Leboot_00085CA8\n\t"
        "nop\n\t"
        "lw    $a0, 0x38($s0)\n\t"
        "addiu $a0, $a0, 0x38\n\t"
        "lh    $a1, 0x0($a0)\n\t"
        "lw    $a2, 0x4($a0)\n\t"
        "jalr  $a2\n\t"
        "addu  $a0, $s0, $a1\n\t"
        ".Leboot_00085CA8:\n\t"
        "lw    $v0, 0x18($s0)\n\t"
        ".Leboot_00085CAC:\n\t"
        "lw    $s0, 0x10($sp)\n\t"
        "lw    $ra, 0x14($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}