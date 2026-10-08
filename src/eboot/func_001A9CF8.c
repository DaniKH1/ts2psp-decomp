/**
 * The Sims 2 PSP - func_001A9CF8 (0x001A9CF8, 0x5C bytes)
 *
 * Asks whether the object is in one of three states, using a flag bit, the
 * sentinel float, and a second flag bit.
 *
 *     lw    $a1, 0x18($a0)
 *     andi  $a2, $a1, 0x2000
 *     bnez  $a2, .Lbit_set
 *     nop
 *     lwc1  $f12, 0x1C($a0)
 *     lui   $a2, 0xBF80
 *     mtc1  $a2, $f13
 *     c.eq.s $f12, $f13
 *     nop
 *     bc1f  .Lyes
 *     ori   $a0, $zero, 0x0
 *     b     .Ltest
 *     lui   $a2, 0x1
 * .Lbit_set:
 *     b     .Lret
 *     move  $v0, $zero
 *     addiu $a2, $a2, 0x1000
 * .Ltest:
 *     and   $a1, $a1, $a2
 *     beqz  $a1, .Lmasked
 *     nop
 * .Lyes:
 *     ori   $a0, $zero, 0x1
 * .Lmasked:
 *     andi  $v0, $a0, 0xFF
 * .Lret:
 *     jr    $ra
 *     nop
 *
 * **`return (flags & 0x2000) ? 0 : (float_1C != -1.0f ? 1 : (flags & 0x1000) != 0);`**
 *
 * Bit 13 wins and answers *no*.  Failing that, the float at 0x1C answers *yes* if
 * it is anything other than the -1.0f sentinel.  Failing that too, bit 12 answers
 * for itself.  It is the natural way to read a tri-state written by the three
 * accessors beside it: `func_001A9C98` sets bit 12 and clears bit 13, `func_001A9CD4`
 * does the reverse, and `func_001A9CBC` clears both while storing a real value -
 * so "bit 13 alone" is one state, "a real float" is another, and "bit 12 alone" is
 * the third.
 *
 * **Two different masks reach the same `and`.**  `$a2` is `0x2000` on the path where
 * bit 13 was found set and `0x1000 | 0x1000 == 0x11000` on the path where it was
 * found clear and the sentinel matched, so the compiler tail-merged the two tests
 * onto one `and` and one `beqz`.  On the first path the answer is already known to be
 * zero, which is why the `bnez` skips straight to the return with `$v0` cleared
 * rather than going through the shared test.
 *
 * That merge is why the expression cannot simply be written as one C ternary chain
 * and left to psp-gcc: it would produce the same answer but not the same code, and
 * the whole point of this file is the original's bytes.
 *
 * **`.set noreorder` is needed at four of the five branches.**  Every one of them has
 * a delay slot whose instruction has to stay put - `nop` at three, `ori` and `lui`
 * and `move` at the others - and the two floating-point ones have a second reason:
 * `c.eq.s` writes the condition code that `bc1f` reads, and the `nop` between them
 * is a real instruction in the original rather than something the assembler added.
 */
#include "types.h"

/* The bit pattern of the sentinel, which is -1.0f. */
#define SENTINEL_HI 0xBF80

/** @return 0 if bit 13 of the flags word at offset 0x18 is set; otherwise 1 if the
 *  float at offset 0x1C is not -1.0f; otherwise whether bit 12 is set. */
__attribute__((noreturn)) u32 func_001A9CF8(void *a0) {
    (void)a0;
    __asm__ __volatile__(
        "lw     $a1, 0x18($a0)\n\t"
        "andi   $a2, $a1, 0x2000\n\t"
        ".set noreorder\n\t"
        "bnez   $a2, 1f\n\t"
        "nop\n\t"
        ".set reorder\n\t"
        "lwc1   $f12, 0x1C($a0)\n\t"
        "lui    $a2, %[hi]\n\t"
        ".set noreorder\n\t"
        "mtc1   $a2, $f13\n\t"
        "c.eq.s $f12, $f13\n\t"
        "nop\n\t"
        "bc1f   2f\n\t"
        "ori    $a0, $zero, 0x0\n\t"
        "b      3f\n\t"
        "lui    $a2, 0x1\n\t"
        ".set reorder\n\t"
        ".set noreorder\n\t"
        "1:\n\t"
        "b      4f\n\t"
        "move   $v0, $zero\n\t"
        ".set reorder\n\t"
        /* Label 3 is here, not on the `and`: the branch above lands on the `addiu`
         * so that it still gets to build the mask.  Only the `bnez` path, which
         * already knows the answer, skips it. */
        "3:\n\t"
        "addiu  $a2, $a2, 0x1000\n\t"
        "and    $a1, $a1, $a2\n\t"
        ".set noreorder\n\t"
        "beqz   $a1, 5f\n\t"
        "nop\n\t"
        ".set reorder\n\t"
        "2:\n\t"
        "ori    $a0, $zero, 0x1\n\t"
        "5:\n\t"
        "andi   $v0, $a0, 0xFF\n\t"
        /* `.set noreorder` has to be reopened *here*, immediately before the last
         * label, rather than left open from the `beqz` above.  A `noreorder`
         * region that spans a label boundary makes this assembler emit one extra
         * word after the block - a trailing `nop` past the symbol's own size - and
         * four bytes is four bytes the original does not have.  Reopening the
         * region at the label leaves the instruction order identical and the size
         * right. */
        ".set noreorder\n\t"
        "4:\n\t"
        "jr     $ra\n\t"
        "nop\n\t"
        ".set reorder\n\t"
        : : [hi] "i" (SENTINEL_HI)
        : "memory", "$a0", "$a1", "$a2", "$v0", "$f12", "$f13");

    /* Stops GCC appending a word of its own past the end of the block, which the
     * symbol-size check would then catch: the last thing in the block is already
     * the `nop` in the return's delay slot. */
    __builtin_unreachable();
}