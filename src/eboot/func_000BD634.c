/**
 * The Sims 2 PSP - func_000BD634 (0x000BD634, 0xA0 bytes)
 *
 *     addiu $sp, $sp, -0x20
 *     sw    $s1, 0x14($sp)
 *     or    $s1, $a0, $zero
 *     sw    $s0, 0x10($sp)
 *     sw    $ra, 0x18($sp)
 *     beqz  $a0, .Leboot_000BD6C0
 *       or    $s0, $a1, $zero
 *     lui   $a0, 0x1F
 *     addiu $a0, $a0, %lo(sym_001EAA78)
 *     sw    $a0, 0x18($s1)
 *     lw    $a0, 0x0($s1)
 *     lw    $a1, 0x24($s1)
 *     jal   func_0019E490
 *       lw    $a0, 0x40($a0)
 *     beqz  $s1, .Leboot_000BD6B0
 *       andi  $a0, $s0, 0x1
 *     lui   $a0, 0x1F
 *     addiu $a0, $a0, %lo(sym_001E9E30)
 *     beqz  $s1, .Leboot_000BD6AC
 *       sw    $a0, 0x18($s1)
 *     lui   $a0, 0x1F
 *     addiu $a0, $a0, %lo(sym_001E9478)
 *     beqz  $s1, .Leboot_000BD6AC
 *       sw    $a0, 0x18($s1)
 *     lui   $a0, 0x1F
 *     addiu $a0, $a0, %lo(sym_001E9300)
 *     sw    $a0, 0x18($s1)
 *     or    $a0, $s1, $zero
 *     jal   func_000B9C88
 *       or    $a1, $zero, $zero
 *   .Leboot_000BD6AC:
 *     andi  $a0, $s0, 0x1
 *   .Leboot_000BD6B0:
 *     beqz  $a0, .Leboot_000BD6C0
 *     nop
 *     jal   func_0012771C
 *       or    $a0, $s1, $zero
 *   .Leboot_000BD6C0:
 *     lw    $s0, 0x10($sp)
 *     lw    $s1, 0x14($sp)
 *     lw    $ra, 0x18($sp)
 *     jr    $ra
 *     addiu $sp, $sp, 0x20
 *     lw    $s1, 0x14($sp)
 *     lw    $ra, 0x18($sp)
 *     jr    $ra
 *     addiu $sp, $sp, 0x20
 *
 * The fourth function of the group that starts at func_000BD5F4, and the
 * one that does the actual work: **four globals are stored into the same
 * field, 0x18 of the object, in sequence.**
 *
 *     sym_001EAA78   the instance's own global
 *     sym_001E9E30   shared
 *     sym_001E9478   shared
 *     sym_001E9300   shared
 *
 * Each store overwrites the previous one, so the last store wins and the
 * earlier three are dead.  **Whether that is intended cannot be told from
 * these bytes** - it is either four fields aliased to one offset by a
 * layout mistake, or four registrations whose field is a scratch slot
 * each callee is expected to consume before the next store.
 *
 * **The three `beqz $s1` branches in the middle are all dead.**
 * `$s1` holds `$a0`, and the `beqz $a0` at the top already returned when
 * `$a0` was null - so by the time control reaches them `$s1` is known
 * non-zero and every one of them falls through.  Each sits in a delay
 * slot that holds a useful store, which is what makes them cheap
 * artefacts of the scheduler rather than tests.
 *
 * **Two of them land on the same instruction, and the third lands one
 * later.**  `.Leboot_000BD6AC` is the `andi $a0, $s0, 0x1` and
 * `.Leboot_000BD6B0` is the `beqz $a0` that follows it - **the two labels
 * sit between two adjacent instructions**, because the compiler had a
 * use for that `andi` on the skipped path and a different one on the
 * fall-through path.  Putting them in the other order moves every
 * branch by one word and nothing in the function looks different.
 *
 * **`andi $a0, $s0, 0x1` appears twice** and both results are dead: the
 * first is the delay slot of a `beqz $s1` that never branches, the second
 * tests bit 0 of the caller's second argument to decide whether to call
 * func_0012771C.  The bit-0 test is the same one `func_0019D4AC`,
 * `func_0019D508` and `func_0019D564` use after a base constructor.
 *
 * `jal func_000B9C88` with `$a1 = 0` is the **base-constructor call those
 * three constructors make**, so this group is the same class family.
 */
#include "types.h"

__attribute__((noreturn)) void func_000BD634(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "sw    $s1, 0x14($sp)\n\t"
        "or    $s1, $a0, $zero\n\t"
        "sw    $s0, 0x10($sp)\n\t"
        "sw    $ra, 0x18($sp)\n\t"
        "beqz  $a0, .Leboot_000BD6C0\n\t"
        "or    $s0, $a1, $zero\n\t"
        "lui   $a0, 0x1F\n\t"
        "addiu $a0, $a0, %%lo(sym_001EAA78)\n\t"
        "sw    $a0, 0x18($s1)\n\t"
        "lw    $a0, 0x0($s1)\n\t"
        "lw    $a1, 0x24($s1)\n\t"
        "jal   func_0019E490\n\t"
        "lw    $a0, 0x40($a0)\n\t"
        "beqz  $s1, .Leboot_000BD6B0\n\t"
        "andi  $a0, $s0, 0x1\n\t"
        "lui   $a0, 0x1F\n\t"
        "addiu $a0, $a0, %%lo(sym_001E9E30)\n\t"
        "beqz  $s1, .Leboot_000BD6AC\n\t"
        "sw    $a0, 0x18($s1)\n\t"
        "lui   $a0, 0x1F\n\t"
        "addiu $a0, $a0, %%lo(sym_001E9478)\n\t"
        "beqz  $s1, .Leboot_000BD6AC\n\t"
        "sw    $a0, 0x18($s1)\n\t"
        "lui   $a0, 0x1F\n\t"
        "addiu $a0, $a0, %%lo(sym_001E9300)\n\t"
        "sw    $a0, 0x18($s1)\n\t"
        "or    $a0, $s1, $zero\n\t"
        "jal   func_000B9C88\n\t"
        "or    $a1, $zero, $zero\n\t"
        ".Leboot_000BD6AC:\n\t"
        "andi  $a0, $s0, 0x1\n\t"
        ".Leboot_000BD6B0:\n\t"
        "beqz  $a0, .Leboot_000BD6C0\n\t"
        "nop\n\t"
        "jal   func_0012771C\n\t"
        "or    $a0, $s1, $zero\n\t"
        ".Leboot_000BD6C0:\n\t"
        "lw    $s0, 0x10($sp)\n\t"
        "lw    $s1, 0x14($sp)\n\t"
        "lw    $ra, 0x18($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}