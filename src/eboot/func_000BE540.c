/**
 * The Sims 2 PSP - func_000BE540 (0x000BE540, 0xE8 bytes)
 *
 *     addiu $sp, $sp, -0x20
 *     sw    $s0, 0x10($sp)
 *     sw    $ra, 0x14($sp)
 *     jal   func_000BD3A4
 *       or    $s0, $a0, $zero
 *     lw    $a0, 0x8($s0)
 *     lw    $a1, 0x24($s0)
 *     lwc1  $f12, 0x14($a0)
 *     swc1  $f12, 0xB0($a1)
 *     lw    $a0, 0x8($s0)
 *     lw    $a1, 0x24($s0)
 *     lwc1  $f12, 0x18($a0)
 *     swc1  $f12, 0xB4($a1)
 *     lw    $a0, 0x24($s0)
 *     lw    $a0, 0xB8($a0)
 *     mtc1  $a0, $f12
 *     bgez  $a0, .Leboot_000BE594
 *       cvt.s.w $f12, $f12
 *     lui   $a0, 0x4F80
 *     mtc1  $a0, $f13
 *     add.s $f12, $f12, $f13
 *   .Leboot_000BE594:
 *     or    $a0, $s0, $zero
 *     jal   updateNodeGraph_08C4
 *       ori   $a1, $zero, 0x7
 *     lw    $a0, 0x24($s0)
 *     lw    $a0, 0xBC($a0)
 *     mtc1  $a0, $f12
 *     bgez  $a0, .Leboot_000BE5C0
 *       cvt.s.w $f12, $f12
 *     lui   $a0, 0x4F80
 *     mtc1  $a0, $f13
 *     add.s $f12, $f12, $f13
 *   .Leboot_000BE5C0:
 *     or    $a0, $s0, $zero
 *     jal   updateNodeGraph_08C4
 *       ori   $a1, $zero, 0x8
 *     lw    $a0, 0x28($s0)
 *     beqz  $a0, .Leboot_000BE618
 *       nop
 *     lw    $a1, 0xC($s0)
 *     lw    $a2, 0x0($s0)
 *     or    $a0, $a1, $a0
 *     sw    $a0, 0xC($s0)
 *     lw    $a0, 0x4($s0)
 *     lw    $a1, 0x38($a2)
 *     sra   $a3, $a0, 5
 *     sll   $a3, $a3, 0x2
 *     addu  $a1, $a1, $a3
 *     lw    $a3, 0x0($a1)
 *     andi  $a0, $a0, 0x1F
 *     ori   $t0, $zero, 0x1
 *     sllv  $a0, $t0, $a0
 *     or    $a0, $a3, $a0
 *     sw    $a0, 0x0($a1)
 *     sb    $zero, 0x34($a2)
 *   .Leboot_000BE618:
 *     lw    $s0, 0x10($sp)
 *     lw    $ra, 0x14($sp)
 *     jr    $ra
 *       addiu $sp, $sp, 0x20
 *
 * Slot +0x034 of `sym_001EB060`.
 *
 * **The last block is a bitfield set, and it is the clearest thing in
 * this slot.**  `0x4($s0)` holds a bit index; `sra`/`sll` by 5 and 2
 * makes the word index, `andi 0x1F` makes the bit within the word, and
 * `ori 1; sllv` builds the mask:
 *
 *   words[this->0x4 >> 5] |= 1 << (this->0x4 & 31)
 *
 * where `words` is the array at `0x38` of the object at `0x0($s0)`.  So
 * **the object at `0x0($s0)` owns a 32-bit-bitset index and a pointer to
 * the words at 0x38** - 1024 flags per word pair, and nothing here says
 * how many words there are.
 *
 * **`this->0xC |= this->0x28` accumulates a value into a field**, and
 * `sb $zero, 0x34($a2)` clears a single flag byte on the same object.
 * The whole tail is therefore: OR a value in, set one bit, clear one
 * byte.
 *
 * **Two unsigned-int-to-float conversions, and both results are
 * thrown away.**  The idiom is unmistakable -
 *
 *   mtc1  $a0, $f12
 *   bgez  $a0, .skip
 *     cvt.s.w $f12, $f12
 *   lui   $a0, 0x4F80
 *   mtc1  $a0, $f13
 *   add.s $f12, $f12, $f13
 *   .skip:
 *
 * - `0x4F800000` is 2^31, so this is `(float)x` biased by 2^31 when `x`
 * is negative, which is exactly how an unsigned 32-bit integer is
 * converted to a float without a wider register.  **The first reads
 * `0xB8` of the destination and the second `0xBC`, and `$f12` is not
 * read again after either** - the next instruction on each path reloads
 * `$a0` with `$s0` and calls `updateNodeGraph_08C4`.  Two complete
 * conversions, both dead.
 *
 * **The two calls pass 7 and then 8** - a consecutive pair, the same
 * shape as `func_000BE4D4` and `func_000BE75C` pass to `func_000BA26C`
 * from slot +0x01C.  **The callee here is `updateNodeGraph_08C4` at
 * 0x1B6144, not `func_000BA26C`, so the two are not the same
 * function**; what repeats is the calling convention of "one call per
 * adjacent channel number", which now appears in two different slots
 * with two different callees.
 *
 * The two float copies at the top are this slot's standard prologue, the
 * same 0x14/0x18 into 0xB0/0xB4 that `func_000BD9FC` performs.
 */
#include "types.h"

__attribute__((noreturn)) void func_000BE540(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "sw    $s0, 0x10($sp)\n\t"
        "sw    $ra, 0x14($sp)\n\t"
        "jal   func_000BD3A4\n\t"
        "or    $s0, $a0, $zero\n\t"
        "lw    $a0, 0x8($s0)\n\t"
        "lw    $a1, 0x24($s0)\n\t"
        "lwc1  $f12, 0x14($a0)\n\t"
        "swc1  $f12, 0xB0($a1)\n\t"
        "lw    $a0, 0x8($s0)\n\t"
        "lw    $a1, 0x24($s0)\n\t"
        "lwc1  $f12, 0x18($a0)\n\t"
        "swc1  $f12, 0xB4($a1)\n\t"
        "lw    $a0, 0x24($s0)\n\t"
        "lw    $a0, 0xB8($a0)\n\t"
        "mtc1  $a0, $f12\n\t"
        "bgez  $a0, .Leboot_000BE594\n\t"
        "cvt.s.w $f12, $f12\n\t"
        "lui   $a0, 0x4F80\n\t"
        "mtc1  $a0, $f13\n\t"
        "add.s $f12, $f12, $f13\n\t"
        ".Leboot_000BE594:\n\t"
        "or    $a0, $s0, $zero\n\t"
        "jal   updateNodeGraph_08C4\n\t"
        "ori   $a1, $zero, 0x7\n\t"
        "lw    $a0, 0x24($s0)\n\t"
        "lw    $a0, 0xBC($a0)\n\t"
        "mtc1  $a0, $f12\n\t"
        "bgez  $a0, .Leboot_000BE5C0\n\t"
        "cvt.s.w $f12, $f12\n\t"
        "lui   $a0, 0x4F80\n\t"
        "mtc1  $a0, $f13\n\t"
        "add.s $f12, $f12, $f13\n\t"
        ".Leboot_000BE5C0:\n\t"
        "or    $a0, $s0, $zero\n\t"
        "jal   updateNodeGraph_08C4\n\t"
        "ori   $a1, $zero, 0x8\n\t"
        "lw    $a0, 0x28($s0)\n\t"
        "beqz  $a0, .Leboot_000BE618\n\t"
        "nop\n\t"
        "lw    $a1, 0xC($s0)\n\t"
        "lw    $a2, 0x0($s0)\n\t"
        "or    $a0, $a1, $a0\n\t"
        "sw    $a0, 0xC($s0)\n\t"
        "lw    $a0, 0x4($s0)\n\t"
        "lw    $a1, 0x38($a2)\n\t"
        "sra   $a3, $a0, 5\n\t"
        "sll   $a3, $a3, 0x2\n\t"
        "addu  $a1, $a1, $a3\n\t"
        "lw    $a3, 0x0($a1)\n\t"
        "andi  $a0, $a0, 0x1F\n\t"
        "ori   $t0, $zero, 0x1\n\t"
        "sllv  $a0, $t0, $a0\n\t"
        "or    $a0, $a3, $a0\n\t"
        "sw    $a0, 0x0($a1)\n\t"
        "sb    $zero, 0x34($a2)\n\t"
        ".Leboot_000BE618:\n\t"
        "lw    $s0, 0x10($sp)\n\t"
        "lw    $ra, 0x14($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}