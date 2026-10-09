/**
 * The Sims 2 PSP - sortAndCullScene_06A4 (0x001B42C0, 0x128 bytes)
 *
 *     addiu      $sp, $sp, -0x40
 *     sw         $s1, 0x24($sp)
 *     lui        $s1, %hi(D_C9010100)
 *     sw         $s0, 0x20($sp)
 *     sw         $s3, 0x2C($sp)
 *     ori        $s3, $zero, 0x1
 *     or         $s0, $a0, $zero
 *     addiu      $s1, $s1, %lo(D_C9010100)
 *     sw         $s2, 0x28($sp)
 *     sw         $s4, 0x30($sp)
 *     sw         $ra, 0x34($sp)
 *     lw         $s4, 0x110($s0)
 *     lw         $a1, 0x5C($s4)
 *     min        $a1, $a1, $s3
 *     jal        func_000ABE34
 *       or        $a0, $s0, $zero
 *     lwc1       $f12, 0x118($s0)
 *     lwc1       $f13, 0x114($s0)
 *     lw         $a0, 0x110($s0)
 *     mul.s      $f12, $f12, $f13
 *     lwc1       $f14, 0x40($a0)
 *     lui        $a0, (0x3F800000 >> 16)
 *     mtc1       $a0, $f15
 *     mul.s      $f12, $f12, $f14
 *     swc1       $f15, 0x3B0($s0)
 *     swc1       $f15, 0x3B4($s0)
 *     swc1       $f15, 0x3B8($s0)
 *     or         $a0, $s0, $zero
 *     swc1       $f12, 0x3BC($s0)
 *     jal        func_000ABF20
 *       lw        $a1, 0x54($s4)
 *     sh         $s3, 0x3F8($s0)
 *     lw         $a0, 0x110($s0)
 *     lw         $a0, 0x3C($a0)
 *     andi       $a0, $a0, 0x10
 *     sltu       $a0, $zero, $a0
 *     andi       $a0, $a0, 0xFF
 *     bnez       $a0, .Leboot_001B436C
 *       or        $s2, $v0, $zero
 *     lw         $a0, 0x3F0($s0)
 *     addiu      $a1, $zero, -0x1001
 *     and        $a0, $a0, $a1
 *     sw         $a0, 0x3F0($s0)
 *   .Leboot_001B436C:
 *     lw         $a0, 0x5C($s4)
 *     blez       $a0, .Leboot_001B4398
 *       nop
 *     sw         $s3, 0x3F4($s0)
 *     or         $a0, $s0, $zero
 *     or         $a1, $zero, $zero
 *     jal        func_000AC190
 *       or        $a2, $zero, $zero
 *     sw         $s2, 0x198($s0)
 *     b          .Leboot_001B43C8
 *       sw        $s1, 0x19C($s0)
 *   .Leboot_001B4398:
 *     sw         $zero, 0x3F4($s0)
 *     sw         $s2, 0x198($s0)
 *     sw         $s1, 0x19C($s0)
 *     addiu      $a0, $s0, 0x180
 *     or         $a1, $zero, $zero
 *     or         $a2, $zero, $zero
 *     ori        $a3, $zero, 0x1
 *     jal        func_001029C8
 *       or        $t0, $zero, $zero
 *     sw         $zero, 0x190($s0)
 *     mtc1       $zero, $f12
 *     swc1       $f12, 0x18C($s0)
 *   .Leboot_001B43C8:
 *     lw         $s0, 0x20($sp)
 *     lw         $s1, 0x24($sp)
 *     lw         $s2, 0x28($sp)
 *     lw         $s3, 0x2C($sp)
 *     lw         $s4, 0x30($sp)
 *     lw         $ra, 0x34($sp)
 *     jr         $ra
 *       addiu    $sp, $sp, 0x40
 *
 * One object argument (`$s0`), one argument in, nothing out: a
 * **per-object setup / initialisation pass** over a fairly large record.
 * Every callee takes `$s0` (plus small scalars), and almost every write goes
 * back into `$s0` at high offsets, so this is writing the object's own
 * runtime state rather than producing a value.
 *
 * `$s4 = s0->field_110` is dereferenced as a sub-record: fields `0x3C`,
 * `0x40`, `0x54`, `0x5C` are read from it, so `field_110` is a pointer to
 * a secondary structure - the render settings / material object.
 *
 * What the bytes say concretely:
 *
 * - **`func_000ABE34(s0, min(s4->field_5C, 1))`** - the `min` with the
 *   constant 1 in `$s3` clamps a flag to a boolean, so `field_5C` is an
 *   integer flag passed as `0` or `1`.  Its return value in `$v0` is saved
 *   into `$s2` later, and that value ends up stored at `s0+0x198`, i.e.
 *   **the first word of a pair at `0x198`/`0x19C` whose second word is the
 *   address of the constant pool entry `D_C9010100`** - a plain literal
 *   (0xC9010100: a colour/rotation-ish constant, or a magic value) being
 *   handed to the following code as a two-word value.
 * - **A float product**: `s0->f118 * s0->f114 * s4->f40` is stored at
 *   `s0+0x3BC`, while `0x3B0`, `0x3B4`, `0x3B8` are all set to **1.0f**
 *   (`mtc1 0x3F800000`).  Three consecutive floats of 1.0 next to a
 *   computed fourth is the shape of a **4-float transform row or colour**
 *   whose first three components are identity and whose fourth carries a
 *   computed scale.  Which of the two the bytes support is not decidable:
 *   the four slots are written, nothing else in this function reads them,
 *   and 0x3B0 is far past the sub-record at 0x110.
 * - **`sh $s3, 0x3F8($s0)`** stores 1 as a **halfword**, and `s0+0x3F0` is
 *   then masked with `~0x1000` (`addiu $a1, $zero, -0x1001` builds
 *   `0xFFFFEFFF`, so bit 12 is cleared) - bit 12 of that word is a flag.
 * - **The bit test at `0x3C`**: `andi 0x10`, then `sltu $a0, $zero, $a0`
 *   and `andi 0xFF` turns the masked bit into a clean **byte-sized bool**,
 *   which is then branched on.  So `s4->field_3C & 0x10` is the condition
 *   for whether `s0+0x3F0`'s bit 12 gets cleared.  Nothing here names that
 *   bit; only its width and effect are known.
 * - **`.Leboot_001B436C` is a loop head reached from above**, not just a
 *   branch target of the flag test: the `bnez` back edge at `0x001B4354`
 *   jumps to it, so the whole "clamp, set 1.0f block, compute product"
 *   sequence is **iterated** while that bool stays true, with
 *   `s0->field_110` reloaded each pass.  The loop has no other exit, so a
 *   single iteration happens only when the flag clears on the first test.
 * - **`blez s4->field_5C`** splits the tail: a non-negative `field_5C` (i.e.
 *   flag == 1) stores 1 at `s0+0x3F4`, calls `func_000AC190(s0, 0, 0)` and
 *   leaves; otherwise `s0+0x3F4` is zeroed and
 *   **`func_001029C8(s0+0x180, 0, 0, 1, 0)`** is called instead, followed
 *   by `s0+0x190 = 0` and `*(float *)(s0+0x18C) = 0.0f`.  Both arms write
 *   the `0x198`/`0x19C` pair before diverging, so that pair is set up
 *   unconditionally.  The sub-object at `s0+0x180` is therefore a small
 *   record with fields at `0x18C` (float), `0x190` (word), and it is
 *   handed to `func_001029C8` with a mode of 1 - which reads like an
 *   **"initialise/resize this sub-buffer"** call, but the callee is not
 *   disassembled here so that stays a guess.
 *
 * The `mtc1`/`swc1` pair with `$zero` is a plain `0.0f` store; the
 * `min` instruction is the real `MIN` opcode (`sll $a1,$a1,0; or ...`-
 * free form), used here as the canonical `a < 1 ? a : 1`.
 */
#include "types.h"

__attribute__((noreturn)) void sortAndCullScene_06A4(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu      $sp, $sp, -0x40\n\t"
        "sw         $s1, 0x24($sp)\n\t"
        "lui        $s1, %%hi(D_C9010100)\n\t"
        "sw         $s0, 0x20($sp)\n\t"
        "sw         $s3, 0x2C($sp)\n\t"
        "ori        $s3, $zero, 0x1\n\t"
        "or         $s0, $a0, $zero\n\t"
        "addiu      $s1, $s1, %%lo(D_C9010100)\n\t"
        "sw         $s2, 0x28($sp)\n\t"
        "sw         $s4, 0x30($sp)\n\t"
        "sw         $ra, 0x34($sp)\n\t"
        "lw         $s4, 0x110($s0)\n\t"
        "lw         $a1, 0x5C($s4)\n\t"
        "min        $a1, $a1, $s3\n\t"
        "jal        func_000ABE34\n\t"
        "  or        $a0, $s0, $zero\n\t"
        "lwc1       $f12, 0x118($s0)\n\t"
        "lwc1       $f13, 0x114($s0)\n\t"
        "lw         $a0, 0x110($s0)\n\t"
        "mul.s      $f12, $f12, $f13\n\t"
        "lwc1       $f14, 0x40($a0)\n\t"
        "lui        $a0, (0x3F800000 >> 16)\n\t"
        "mtc1       $a0, $f15\n\t"
        "mul.s      $f12, $f12, $f14\n\t"
        "swc1       $f15, 0x3B0($s0)\n\t"
        "swc1       $f15, 0x3B4($s0)\n\t"
        "swc1       $f15, 0x3B8($s0)\n\t"
        "or         $a0, $s0, $zero\n\t"
        "swc1       $f12, 0x3BC($s0)\n\t"
        "jal        func_000ABF20\n\t"
        "  lw        $a1, 0x54($s4)\n\t"
        "sh         $s3, 0x3F8($s0)\n\t"
        "lw         $a0, 0x110($s0)\n\t"
        "lw         $a0, 0x3C($a0)\n\t"
        "andi       $a0, $a0, 0x10\n\t"
        "sltu       $a0, $zero, $a0\n\t"
        "andi       $a0, $a0, 0xFF\n\t"
        "bnez       $a0, .Leboot_001B436C\n\t"
        "  or        $s2, $v0, $zero\n\t"
        "lw         $a0, 0x3F0($s0)\n\t"
        "addiu      $a1, $zero, -0x1001\n\t"
        "and        $a0, $a0, $a1\n\t"
        "sw         $a0, 0x3F0($s0)\n\t"
        ".Leboot_001B436C:\n\t"
        "lw         $a0, 0x5C($s4)\n\t"
        "blez       $a0, .Leboot_001B4398\n\t"
        "  nop\n\t"
        "sw         $s3, 0x3F4($s0)\n\t"
        "or         $a0, $s0, $zero\n\t"
        "or         $a1, $zero, $zero\n\t"
        "jal        func_000AC190\n\t"
        "  or        $a2, $zero, $zero\n\t"
        "sw         $s2, 0x198($s0)\n\t"
        "b          .Leboot_001B43C8\n\t"
        "  sw        $s1, 0x19C($s0)\n\t"
        ".Leboot_001B4398:\n\t"
        "sw         $zero, 0x3F4($s0)\n\t"
        "sw         $s2, 0x198($s0)\n\t"
        "sw         $s1, 0x19C($s0)\n\t"
        "addiu      $a0, $s0, 0x180\n\t"
        "or         $a1, $zero, $zero\n\t"
        "or         $a2, $zero, $zero\n\t"
        "ori        $a3, $zero, 0x1\n\t"
        "jal        func_001029C8\n\t"
        "  or        $t0, $zero, $zero\n\t"
        "sw         $zero, 0x190($s0)\n\t"
        "mtc1       $zero, $f12\n\t"
        "swc1       $f12, 0x18C($s0)\n\t"
        ".Leboot_001B43C8:\n\t"
        "lw         $s0, 0x20($sp)\n\t"
        "lw         $s1, 0x24($sp)\n\t"
        "lw         $s2, 0x28($sp)\n\t"
        "lw         $s3, 0x2C($sp)\n\t"
        "lw         $s4, 0x30($sp)\n\t"
        "lw         $ra, 0x34($sp)\n\t"
        "jr         $ra\n\t"
        "  addiu    $sp, $sp, 0x40\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}