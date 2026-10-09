/**
 * The Sims 2 PSP - func_000BFBAC (0x000BFBAC, 0x80 bytes)
 *
 *     addiu $sp, $sp, -0x20
 *     sw    $s1, 0x14($sp)
 *     or    $s1, $a0, $zero
 *     sw    $s0, 0x10($sp)
 *     sw    $ra, 0x18($sp)
 *     beqz  $a0, .Leboot_000BFC18
 *       or    $s0, $a1, $zero
 *     lui   $a0, 0x1F
 *     addiu $a0, $a0, %lo(sym_001EB560)
 *     sw    $a0, 0x18($s1)
 *     lw    $a0, 0x0($s1)
 *     lw    $a1, 0x24($s1)
 *     jal   func_0019F058
 *       lw    $a0, 0x40($a0)
 *     beqz  $s1, .Leboot_000BFC08
 *       andi  $a0, $s0, 0x1
 *     lui   $a0, 0x1F
 *     addiu $a0, $a0, %lo(sym_001E9300)
 *     sw    $a0, 0x18($s1)
 *     or    $a0, $s1, $zero
 *     jal   func_000B9C88
 *       or    $a1, $zero, $zero
 *     andi  $a0, $s0, 0x1
 *   .Leboot_000BFC08:
 *     beqz  $a0, .Leboot_000BFC18
 *       nop
 *     jal   func_0012771C
 *       or    $a0, $s1, $zero
 *   .Leboot_000BFC18:
 *     lw    $s0, 0x10($sp)
 *     lw    $s1, 0x14($sp)
 *     lw    $ra, 0x18($sp)
 *     jr    $ra
 *       addiu $sp, $sp, 0x20
 *
 * Slot +0x0C of `sym_001EB560` - the eleventh and last of the sibling
 * constructors, and **the only one that is not the 160-byte template.**
 *
 * **It is the template with two of the four stores deleted**, 32 words
 * instead of 40.  The other ten do this:
 *
 *   lui/addiu sym_001E9E30 ; sw 0x18   ; beqz $s1 (dead)
 *   lui/addiu sym_001E9478 ; sw 0x18   ; beqz $s1 (dead)
 *
 * and this one does neither, leaving only `sym_001E9300`.  So the chain
 * of four globals stored into field 0x18 becomes a chain of two: its own
 * vtable, then the shared `sym_001E9300` overwriting it.  **The dead
 * stores went from three to one because two of them were deleted, not
 * because the surviving one started mattering** - the last store wins
 * either way, so the observable effect on this class is exactly the
 * same as on the other ten.
 *
 * **And this is the class that overrides all three of slots +0x01C,
 * +0x04C and +0x064** - the only one of the eleven.  The shortest
 * constructor and the extra overrides sit in the same record.  That is
 * suggestive but it is not evidence: the constructor's dead stores and
 * its vtable's live entries are unrelated code, and nothing in these
 * bytes ties them together.  What *is* decidable is that this one is
 * also the class whose `+0x064` answers 0 while the other ten return
 * their argument.
 *
 * **Only two labels, where the other ten have three.**  The pair that
 * sat *between* the `andi` and the `beqz` around the flag test is gone,
 * because there is now only one `beqz $s1` to land on rather than three.
 * The placement rule that cost `func_000BD634` four attempts is the same
 * one, but with fewer labels to get wrong.
 *
 * Its callee at word 12 is `func_0019F058`.  Across the eleven
 * constructors those callees run strictly upward -
 * `func_0019E490`, `590`, `684`, `780`, `888`, `9C4`, `EB18`, `EC9C`,
 * `EDA4`, `EF78`, `F058` - **one per class, in vtable order, with
 * irregular gaps from 0xE0 to 0x1D4 bytes over ten steps totalling
 * 0xBC8.**  That the sequence is
 * ordered at all is a fact about the linker more than the source, but
 * it does mean the eleven constructors' callees form a contiguous run of
 * related code with nothing else interleaved between them.
 */
#include "types.h"

__attribute__((noreturn)) void func_000BFBAC(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "sw    $s1, 0x14($sp)\n\t"
        "or    $s1, $a0, $zero\n\t"
        "sw    $s0, 0x10($sp)\n\t"
        "sw    $ra, 0x18($sp)\n\t"
        "beqz  $a0, .Leboot_000BFC18\n\t"
        "or    $s0, $a1, $zero\n\t"
        "lui   $a0, 0x1F\n\t"
        "addiu $a0, $a0, %%lo(sym_001EB560)\n\t"
        "sw    $a0, 0x18($s1)\n\t"
        "lw    $a0, 0x0($s1)\n\t"
        "lw    $a1, 0x24($s1)\n\t"
        "jal   func_0019F058\n\t"
        "lw    $a0, 0x40($a0)\n\t"
        "beqz  $s1, .Leboot_000BFC08\n\t"
        "andi  $a0, $s0, 0x1\n\t"
        "lui   $a0, 0x1F\n\t"
        "addiu $a0, $a0, %%lo(sym_001E9300)\n\t"
        "sw    $a0, 0x18($s1)\n\t"
        "or    $a0, $s1, $zero\n\t"
        "jal   func_000B9C88\n\t"
        "or    $a1, $zero, $zero\n\t"
        "andi  $a0, $s0, 0x1\n\t"
        ".Leboot_000BFC08:\n\t"
        "beqz  $a0, .Leboot_000BFC18\n\t"
        "nop\n\t"
        "jal   func_0012771C\n\t"
        "or    $a0, $s1, $zero\n\t"
        ".Leboot_000BFC18:\n\t"
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