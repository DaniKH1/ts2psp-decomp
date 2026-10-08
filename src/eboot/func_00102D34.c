/**
 * The Sims 2 PSP - func_00102D34 (0x00102D34, 0x138 bytes)
 *
 * Copies a structure to a fixed address, then writes a tag and sixteen fixed-point
 * values to the GPU command buffer.
 *
 *     lw    $a1, 0x0($a0)
 *     lw    $a2, 0x4($a0)
 *     lw    $a3, 0x8($a0)
 *     lui   $t0, 0xE
 *     sw    $a1, 0x2168($t0)
 *     addiu $a1, $t0, 0x2168
 *     ... fifteen more loads and stores, three at a time ...
 *     lw    $a2, 0xC($a0)
 *     ... up to offset 0x3C, seventeen words in all ...
 *     lui   $a1, 0x1E
 *     lw    $a2, -0x48F0($a1)
 *     cache 0x18, 0x3C($a2)
 *     lui   $a3, 0x3E00
 *     lui   $t0, 0x3F00
 *     lui   $t1, 0x3F00
 *     lui   $t2, 0x3F00
 *     lui   $t3, 0x3F00
 *     sw    $a3, 0x0($a2)
 *     lwr   $t0, 0x1($a0)
 *     lwr   $t1, 0x5($a0)
 *     lwr   $t2, 0x9($a0)
 *     lwr   $t3, 0xD($a0)
 *     sw    $t0, 0x4($a2)
 *     ... four rows, sixteen lwr at 1, 5, 9, 13 and then 0x11, 0x15, 0x19, 0x1D ... ...
 *     sw    $t3, 0x40($a2)
 *     addiu $a0, $a2, 0x44
 *     jr    $ra
 *     sw    $a0, -0x48F0($a1)
 *
 * **`*(u32 *)0x0E2168 = arg[0..16];` then `cursor[0] = 0x3E000000;
 * cursor[1..16] = fixed_point_24bit(arg); cursor += 0x44;`**
 *
 * **The two halves are unrelated and the register allocation shows it.**  The first
 * seventeen words go to a fixed address; the second half never mentions 0x0E2168
 * again and reads from `$a0`, the argument, throughout.  `$t0` holds the fixed address
 * for the first half and is then reloaded with `lui 0x3F00` for the second, and `$a1`
 * holds the address once and is reloaded with `lui 0x1E` - so **no live value crosses
 * the boundary**, which is why a register can be reused for two unrelated purposes
 * twelve instructions apart with no spill.
 *
 * **The `lui 0x3F00`s are not dead constants, and that is the interesting part.**
 * Each is followed by an `lwr` three instructions later that loads *three* bytes - an
 * unaligned load runs from the given address to the next word boundary.  So the low
 * byte of each register is whatever the `lui` left there, which is `0x00`, and the
 * `lwr` fills bytes 1, 2 and 3.  **The result is a word of the form `0x3F00_00xyz`,
 * which read as a float is in [1.0, 1.5)** because 0x3F000000 is exactly 1.0f and the
 * three source bytes are its low twenty-four bits.  So the module decodes 24-bit
 * fractions to floats here, one word each, sixteen of them.
 *
 * **If the four `lui 0x3F00`s had been omitted the low byte would be whatever the
 * previous `lwr` left**, which is a previous row's data - so the constants are
 * load-bearing in the low byte and dead in the high half.  **That is a different thing
 * from the dead `lui`s recorded elsewhere in this tree**, where the whole register was
 * overwritten; it is the first instance here where a "dead" high half is quietly
 * supplying part of the value, and it is why the sixteen `lwr` cannot be read as
 * sixteen three-byte copies.
 *
 * **The offsets are 1, 5, 9 and 13, then 0x11, 0x15, 0x19, 0x1D** - a stride of four
 * within a row and 0x10 between rows, so the source is being read as sixteen bytes a
 * row.  **Whether the values really are three bytes at that spacing is not
 * established**: three-byte values at offsets 0, 3, 6, 9 would not be at 1, 5, 9, 13,
 * and the fourth read of each row lands inside the fourth four-byte field rather than
 * at the start of a fifth value.  What the offsets say is that the reads are
 * deliberately unaligned and deliberately strided, and the reason is not recoverable
 * from the bytes.
 *
 * `renderMeshInstances_1060` does the same thing with offsets 1, 5 and 9 - three words
 * per row instead of four - and `renderMeshInstances_1034`'s `cache 0x18, 0x3C` is the
 * same cache hint at the same offset, so **all three are the command-buffer convention
 * rather than anything about this transform.**  The cursor advances by 0x44, which is
 * the tag word plus sixteen, and is written back to the global at 0x01FB710.
 *
 * The seventeen-word copy uses `$a1` as a rolling base: `addiu $a1, $t0, 0x2168` once,
 * then stores at 0x0, 0x4, 0x8, 0xC ... 0x3C of it.  **One `addiu` against sixteen
 * would have been the alternative**, which is the same base-pointer decision
 * `func_0018A650` makes with three `lwc1`s.
 */
#include "types.h"

/** Copy seventeen words to 0x0E2168, then write a tag and sixteen fixed-point values
 *  to the command buffer and advance its cursor by 0x44.
 *  @param arg In $a0: seventeen words to copy, then sixteen bytes a row to decode. */
__attribute__((noreturn)) void func_00102D34(void *arg) {
    (void)arg;
    __asm__ __volatile__(
        "lw    $a1, 0x0($a0)\n\t"
        "lw    $a2, 0x4($a0)\n\t"
        "lw    $a3, 0x8($a0)\n\t"
        "lui   $t0, 0xE\n\t"
        "sw    $a1, 0x2168($t0)\n\t"
        "addiu $a1, $t0, 0x2168\n\t"
        "sw    $a2, 0x4($a1)\n\t"
        "sw    $a3, 0x8($a1)\n\t"
        "lw    $a2, 0xC($a0)\n\t"
        "lw    $a3, 0x10($a0)\n\t"
        "lw    $t0, 0x14($a0)\n\t"
        "sw    $a2, 0xC($a1)\n\t"
        "sw    $a3, 0x10($a1)\n\t"
        "sw    $t0, 0x14($a1)\n\t"
        "lw    $a2, 0x18($a0)\n\t"
        "lw    $a3, 0x1C($a0)\n\t"
        "lw    $t0, 0x20($a0)\n\t"
        "sw    $a2, 0x18($a1)\n\t"
        "sw    $a3, 0x1C($a1)\n\t"
        "sw    $t0, 0x20($a1)\n\t"
        "lw    $a2, 0x24($a0)\n\t"
        "lw    $a3, 0x28($a0)\n\t"
        "lw    $t0, 0x2C($a0)\n\t"
        "sw    $a2, 0x24($a1)\n\t"
        "sw    $a3, 0x28($a1)\n\t"
        "sw    $t0, 0x2C($a1)\n\t"
        "lw    $a2, 0x30($a0)\n\t"
        "lw    $a3, 0x34($a0)\n\t"
        "lw    $t0, 0x38($a0)\n\t"
        "sw    $a2, 0x30($a1)\n\t"
        "sw    $a3, 0x34($a1)\n\t"
        "sw    $t0, 0x38($a1)\n\t"
        "lw    $a2, 0x3C($a0)\n\t"
        "sw    $a2, 0x3C($a1)\n\t"
        "lui   $a1, 0x1E\n\t"
        "lw    $a2, -0x48F0($a1)\n\t"
        "cache 0x18, 0x3C($a2)\n\t"
        "lui   $a3, 0x3E00\n\t"
        "lui   $t0, 0x3F00\n\t"
        "lui   $t1, 0x3F00\n\t"
        "lui   $t2, 0x3F00\n\t"
        "lui   $t3, 0x3F00\n\t"
        "sw    $a3, 0x0($a2)\n\t"
        "lwr   $t0, 0x1($a0)\n\t"
        "lwr   $t1, 0x5($a0)\n\t"
        "lwr   $t2, 0x9($a0)\n\t"
        "lwr   $t3, 0xD($a0)\n\t"
        "sw    $t0, 0x4($a2)\n\t"
        "sw    $t1, 0x8($a2)\n\t"
        "sw    $t2, 0xC($a2)\n\t"
        "sw    $t3, 0x10($a2)\n\t"
        "lwr   $t0, 0x11($a0)\n\t"
        "lwr   $t1, 0x15($a0)\n\t"
        "lwr   $t2, 0x19($a0)\n\t"
        "lwr   $t3, 0x1D($a0)\n\t"
        "sw    $t0, 0x14($a2)\n\t"
        "sw    $t1, 0x18($a2)\n\t"
        "sw    $t2, 0x1C($a2)\n\t"
        "sw    $t3, 0x20($a2)\n\t"
        "lwr   $t0, 0x21($a0)\n\t"
        "lwr   $t1, 0x25($a0)\n\t"
        "lwr   $t2, 0x29($a0)\n\t"
        "lwr   $t3, 0x2D($a0)\n\t"
        "sw    $t0, 0x24($a2)\n\t"
        "sw    $t1, 0x28($a2)\n\t"
        "sw    $t2, 0x2C($a2)\n\t"
        "sw    $t3, 0x30($a2)\n\t"
        "lwr   $t0, 0x31($a0)\n\t"
        "lwr   $t1, 0x35($a0)\n\t"
        "lwr   $t2, 0x39($a0)\n\t"
        "lwr   $t3, 0x3D($a0)\n\t"
        "sw    $t0, 0x34($a2)\n\t"
        "sw    $t1, 0x38($a2)\n\t"
        "sw    $t2, 0x3C($a2)\n\t"
        "sw    $t3, 0x40($a2)\n\t"
        "addiu $a0, $a2, 0x44\n\t"
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "sw    $a0, -0x48F0($a1)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$a0", "$a1", "$a2", "$a3",
          "$t0", "$t1", "$t2", "$t3");
}