/**
 * The Sims 2 PSP - func_000E7F9C (0x000E7F9C, 0x34 bytes)
 *
 * Clears one entry in a halfword array, sets one byte in the object, and stores a
 * pointer into a word array.
 *
 *     lui  $a3, 0xF
 *     addu $a2, $a1, $a1
 *     addiu $a3, $a3, -0x6838
 *     addu $a2, $a2, $a3
 *     sh   $zero, 0x0($a2)
 *     ori  $a2, $zero, 0x4
 *     sb   $a2, 0x30($a0)
 *     lui  $a2, 0xF
 *     sll  $a1, $a1, 2
 *     addiu $a2, $a2, -0x68D8
 *     addu $a1, $a1, $a2
 *     jr   $ra
 *     sw   $a0, 0x0($a1)
 *
 * **`halfwords[0xE97C8 / 2 + index] = 0; self->byte_30 = 4;
 * words[0xE9728 / 4 + index] = (u32)self;`**  - with `self` in `$a0` and `index` in
 * `$a1`.
 *
 * **Two arrays of different element width, plus a field on the object**, and the
 * index is shared by all three.  The halfword array is indexed by `index * 2` - the
 * `addu $a2, $a1, $a1` doubling, which is cheaper than a shift - and the word array
 * by `index << 2`.
 *
 * **The order is what a source with three statements does.**  Clear the halfword,
 * set the object's byte, store the pointer.  `$a2` carries the halfword address, is
 * then reused for the constant 4, and is then reused again for the word array's high
 * half, so no register is held across all three statements - which costs three
 * `lui`s and saves two `mov`s.
 *
 * **`$a1` is written twice and read once**, so it cannot be an asm input: it holds
 * the index, it becomes `index << 2`, and it becomes the word array's address.  The
 * parameters are therefore named in the comment and the block clobbers `$a0` through
 * `$a3` outright, which is the same trade `func_0000BEC0` makes.
 *
 * **This is the 0xE9 cluster.**  0x0E9728 has four bytes per entry and 0x0E97C8 two,
 * with 0x0E97A8 at twenty-eight in between (`func_000E7F84`, thirty-six bytes
 * earlier).  `tools/code_writers.py` counts twelve functions addressing 0x0E9728
 * and nine addressing 0x0E97A8, every one of them live, and every one landing
 * inside a function's body rather than in a data section - which is the module
 * keeping writable data in `.text`, and the reason splat's function sizes overlap
 * it.
 *
 * `4` written as a byte at offset 0x30 is the kind of small enumerator a record
 * keeps beside its indices; nothing here says which.
 */
#include "types.h"

/* Both array bases are `lui 0xF` plus a negative low half. */
#define HALF_HI 0xF
#define HALF_LO (-0x6838)
#define WORD_HI 0xF
#define WORD_LO (-0x68D8)

/** Clear one halfword entry, set the object's byte at 0x30 to 4, and store the
 *  object's address into one word entry.
 *  @param self  In $a0.
 *  @param index In $a1, shared by all three stores. */
__attribute__((noreturn)) void func_000E7F9C(void) {
    __asm__ __volatile__(
        "lui  $a3, %[hhi]\n\t"
        "addu $a2, $a1, $a1\n\t"
        "addiu $a3, $a3, %[hlo]\n\t"
        "addu $a2, $a2, $a3\n\t"
        "sh   $zero, 0x0($a2)\n\t"
        "ori  $a2, $zero, 0x4\n\t"
        "sb   $a2, 0x30($a0)\n\t"
        "lui  $a2, %[whi]\n\t"
        "sll  $a1, $a1, 2\n\t"
        "addiu $a2, $a2, %[wlo]\n\t"
        "addu $a1, $a1, $a2\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "sw   $a0, 0x0($a1)\n\t"
        ".set reorder\n\t"
        : : [hhi] "i" (HALF_HI), [hlo] "i" (HALF_LO),
            [whi] "i" (WORD_HI), [wlo] "i" (WORD_LO)
        : "memory", "$a0", "$a1", "$a2", "$a3");
}