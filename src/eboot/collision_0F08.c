/**
 * The Sims 2 PSP - collision_0F08 (0x001B1728, 0xF0 bytes)
 *
 *     addiu      $sp, $sp, -0x20
 *     sw         $s0, 0x10($sp)
 *     or         $s0, $a0, $zero
 *     lw         $a0, 0xC($a2)
 *     sw         $s1, 0x14($sp)
 *     or         $s1, $a1, $zero
 *     sw         $s2, 0x18($sp)
 *     sw         $ra, 0x1C($sp)
 *     beqz       $a0, .Leboot_001B178C
 *       or        $s2, $a2, $zero
 *     or         $a1, $a0, $zero
 *     lw         $a0, 0x4($s2)
 *     sll        $a1, $a1, 2
 *     addu       $a1, $a0, $a1
 *     beq        $a1, $a0, .Leboot_001B1774
 *       nop
 *     addiu      $a1, $a1, -0x4
 *   .Leboot_001B176C:
 *     bne        $a1, $a0, .Leboot_001B176C
 *       addiu     $a1, $a1, -0x4
 *   .Leboot_001B1774:
 *     or         $a0, $s2, $zero
 *     jal        func_0012C914
 *       or        $a1, $zero, $zero
 *     sw         $zero, 0xC($s2)
 *     b          .Leboot_001B17D8
 *       lw        $a1, 0x0($s0)
 *   .Leboot_001B178C:
 *     or         $a0, $s2, $zero
 *     jal        func_0012C914
 *       or        $a1, $zero, $zero
 *     srl        $a1, $v0, 2
 *     lw         $a0, 0xC($s2)
 *     bnel       $a1, $zero, .Leboot_001B17A8
 *       ori       $a1, $zero, 0x0
 *   .Leboot_001B17A8:
 *     lw         $a3, 0x4($s2)
 *     sll        $a2, $a0, 2
 *     sll        $a0, $a1, 2
 *     addu       $a2, $a3, $a2
 *     addu       $a0, $a3, $a0
 *     beq        $a2, $a0, .Leboot_001B17D0
 *       nop
 *     addiu      $a2, $a2, 0x4
 *   .Leboot_001B17C8:
 *     bne        $a2, $a0, .Leboot_001B17C8
 *       addiu     $a2, $a2, 0x4
 *   .Leboot_001B17D0:
 *     sw         $a1, 0xC($s2)
 *     lw         $a1, 0x0($s0)
 *   .Leboot_001B17D8:
 *     bnez       $a1, .Leboot_001B17E8
 *       nop
 *     b          .Leboot_001B1800
 *       or        $v0, $zero, $zero
 *   .Leboot_001B17E8:
 *     or         $a0, $s0, $zero
 *     or         $a1, $s1, $zero
 *     or         $a2, $zero, $zero
 *     jal        collision_0C18
 *       or        $a3, $s2, $zero
 *     lw         $v0, 0xC($s2)
 *   .Leboot_001B1800:
 *     lw         $s0, 0x10($sp)
 *     lw         $s1, 0x14($sp)
 *     lw         $s2, 0x18($sp)
 *     lw         $ra, 0x1C($sp)
 *     jr         $ra
 *       addiu    $sp, $sp, 0x20
 *
 * Three arguments: `$a0` -> `$s0`, `$a1` -> `$s1`, `$a2` -> `$s2`.
 * All three callees-visible arguments are objects: `$s2` is read at `0x4`
 * (a word base pointer), `0xC` (a count) and written at `0xC`; `$s0` is read
 * at `0x0` as a count-like value and passed on as an object.
 *
 * The shape is unambiguous once the two `+4` loops are seen: **`$s2` is a
 * growable array whose elements are 4 bytes wide.** `count` at `0xC`,
 * `base` at `0x4`, and the address of element `i` is `base + (i << 2)` -
 * `sll` by 2, `addu`, which is how a `u32`/`int*` array is indexed.  The
 * loops at `.Leboot_001B176C` and `.Leboot_001B17C8` walk one of those
 * addresses **backwards / forwards by 4 until it reaches the end address**,
 * which is the codewarrior idiom for `for (p = end; p != start; p -= 4)`.
 * Note the first loop walks *down* from `base + count*4` to `base`, and the
 * second walks *up* from `base + oldCount*4` to `base + newCount*4` - so
 * the second is the normalising half of the same idea.
 *
 * Reading it as `s2->count` / `s2->base`:
 *
 * - **`count == 0` (`.Leboot_001B178C`)**: call `func_0012C914(s2, 0)` and
 *   take `v0 >> 2` as the new count.  That is a *capacity query*: the
 *   callee returns a byte size or an element total and the caller converts
 *   it, `srl 2`, into an element count.  Then the address of the old count
 *   and of the new count are walked up to `base`, and the new count is
 *   stored (`sw $a1, 0xC($s2)`).  So this path is the **initial
 *   allocation**: the container starts empty and its size comes from
 *   `func_0012C914`.
 * - **`count != 0`**: the address of the current last element is walked
 *   down to `base`, then `func_0012C914(s2, 0)` is called and the count is
 *   **zeroed** - i.e. the same container is emptied, not resized.  The
 *   `lw $a1, 0x0($s0)` in the `b` delay slot is the *next* test's load, so
 *   the sequence is: clear the array, then decide whether to refill it.
 * - **`.Leboot_001B17D8`**: `if (s0->field0 != 0)` call
 *   `collision_0C18(s0, s1, 0, s2)` - **an accumulator passed in** - and
 *   return `s2->count` in `$v0`; otherwise return 0.
 *
 * So this is the **"reset then repopulate" front end of the collision
 * broad phase**: `func_0012C914(s2, 0)` is asked how much room the array
 * needs, the array is cleared, and if the query object `$s0` has anything
 * in it, `collision_0C18` is run over it with `$s1` and `$s2` and the number
 * of entries that ended up in `$s2` is the return value.
 *
 * The `bnel`/`ori $a1, $zero, 0x0` pair at `.Leboot_001B17A8` is not a
 * `bne`: `bnel` always executes the delay slot, and the delay slot simply
 * materialises the constant 0 that `srl` would otherwise have produced, so
 * the "or 0" is dead but harmless - the compiler folded the zero into the
 * branch-likely's delay slot.  Nothing in these bytes distinguishes a
 * **vector of overlaps, of colliders, or of cell indices**; the 4-byte
 * stride is the only width the code states.
 */
#include "types.h"

__attribute__((noreturn)) void collision_0F08(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu      $sp, $sp, -0x20\n\t"
        "sw         $s0, 0x10($sp)\n\t"
        "or         $s0, $a0, $zero\n\t"
        "lw         $a0, 0xC($a2)\n\t"
        "sw         $s1, 0x14($sp)\n\t"
        "or         $s1, $a1, $zero\n\t"
        "sw         $s2, 0x18($sp)\n\t"
        "sw         $ra, 0x1C($sp)\n\t"
        "beqz       $a0, .Leboot_001B178C\n\t"
        "  or        $s2, $a2, $zero\n\t"
        "or         $a1, $a0, $zero\n\t"
        "lw         $a0, 0x4($s2)\n\t"
        "sll        $a1, $a1, 2\n\t"
        "addu       $a1, $a0, $a1\n\t"
        "beq        $a1, $a0, .Leboot_001B1774\n\t"
        "  nop\n\t"
        "addiu      $a1, $a1, -0x4\n\t"
        ".Leboot_001B176C:\n\t"
        "bne        $a1, $a0, .Leboot_001B176C\n\t"
        "  addiu     $a1, $a1, -0x4\n\t"
        ".Leboot_001B1774:\n\t"
        "or         $a0, $s2, $zero\n\t"
        "jal        func_0012C914\n\t"
        "  or        $a1, $zero, $zero\n\t"
        "sw         $zero, 0xC($s2)\n\t"
        "b          .Leboot_001B17D8\n\t"
        "  lw        $a1, 0x0($s0)\n\t"
        ".Leboot_001B178C:\n\t"
        "or         $a0, $s2, $zero\n\t"
        "jal        func_0012C914\n\t"
        "  or        $a1, $zero, $zero\n\t"
        "srl        $a1, $v0, 2\n\t"
        "lw         $a0, 0xC($s2)\n\t"
        "bnel       $a1, $zero, .Leboot_001B17A8\n\t"
        "  ori       $a1, $zero, 0x0\n\t"
        ".Leboot_001B17A8:\n\t"
        "lw         $a3, 0x4($s2)\n\t"
        "sll        $a2, $a0, 2\n\t"
        "sll        $a0, $a1, 2\n\t"
        "addu       $a2, $a3, $a2\n\t"
        "addu       $a0, $a3, $a0\n\t"
        "beq        $a2, $a0, .Leboot_001B17D0\n\t"
        "  nop\n\t"
        "addiu      $a2, $a2, 0x4\n\t"
        ".Leboot_001B17C8:\n\t"
        "bne        $a2, $a0, .Leboot_001B17C8\n\t"
        "  addiu     $a2, $a2, 0x4\n\t"
        ".Leboot_001B17D0:\n\t"
        "sw         $a1, 0xC($s2)\n\t"
        "lw         $a1, 0x0($s0)\n\t"
        ".Leboot_001B17D8:\n\t"
        "bnez       $a1, .Leboot_001B17E8\n\t"
        "  nop\n\t"
        "b          .Leboot_001B1800\n\t"
        "  or        $v0, $zero, $zero\n\t"
        ".Leboot_001B17E8:\n\t"
        "or         $a0, $s0, $zero\n\t"
        "or         $a1, $s1, $zero\n\t"
        "or         $a2, $zero, $zero\n\t"
        "jal        collision_0C18\n\t"
        "  or        $a3, $s2, $zero\n\t"
        "lw         $v0, 0xC($s2)\n\t"
        ".Leboot_001B1800:\n\t"
        "lw         $s0, 0x10($sp)\n\t"
        "lw         $s1, 0x14($sp)\n\t"
        "lw         $s2, 0x18($sp)\n\t"
        "lw         $ra, 0x1C($sp)\n\t"
        "jr         $ra\n\t"
        "  addiu    $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}