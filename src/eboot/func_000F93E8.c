/**
 * The Sims 2 PSP - func_000F93E8 (0x000F93E8, 0x2C bytes)
 *
 * Indexes a 28-byte-stride structure at 0x0EB850 and initialises three of its
 * fields.
 *
 *     sll  $a1, $a0, 5
 *     sll  $a0, $a0, 2
 *     subu $a0, $a1, $a0
 *     lui  $a1, 0xF
 *     addiu $a1, $a1, -0x47B0
 *     ori  $a2, $zero, 0xA
 *     addu $a0, $a0, $a1
 *     sw   $a2, 0x0($a0)
 *     sw   $zero, 0x8($a0)
 *     jr   $ra
 *     sw   $zero, 0xC($a0)
 *
 * **`*(u32 *)(0x0EB850 + 28 * index) = 10`, and the words at +8 and +C of the
 * same record to zero.**  The word at +4 is not touched.
 *
 * **The stride is computed rather than multiplied.**  `index << 5 - index << 2` is
 * `index * 28`, and the compiler reached for shifts because 28 is 32 - 4 and both
 * are powers of two.  That it does so here, and uses `mult`/`mflo` for fixed-point
 * products in `func_000E8F08`, is the same choice CodeWarrior makes: shifts for a
 * small constant, multiply for a general 32-bit product.
 *
 * **The base is a bare `lui`/`addiu` pair**, 0x0F0000 + (-0x47B0) = 0x0EB850, and
 * the relocation list confirms it: both halves are R_MIPS_HI16/R_MIPS_LO16 against
 * the same target.
 *
 * ## The base address is inside a function's body
 *
 * Nine functions materialise 0x0EB850, and `tools/stride_table.py` is the census.
 * All nine index or walk it with stride 28, and one of them zeroes 32 records of it
 * in a loop with `addiu $p, $p, 0x1C` in the branch's delay slot.
 *
 * **0x0EB850 is not data.**  It is sixteen bytes into `func_000EB840`'s prologue -
 * the instruction there is `move $s1, $a1` - and the twelve words that follow carry
 * two R_MIPS_26 relocations at 0x0EB85C and 0x0EB884, which is to say two `jal`s.
 * A record at index 1 would land on `sw $ra, 0x34($sp)` and index 4 exactly on the
 * next function's entry point, so the "structure" cannot be one.
 *
 * **And this function is reached.**  There is a real `jal func_0F93E8` at
 * 0x0EAAA8, in a delay slot fed by `lbu $a0, 0x7E($a0)` - a byte out of the object,
 * 0 to 255, with nothing masking it.
 *
 * So the module contains nine live functions writing into its own code section, at
 * addresses computed from an index a caller supplies from a byte field.  **What that
 * means is not decided here.**  Three things are consistent with the bytes and
 * cannot be told apart from inside this function: the accessors are unreachable in
 * practice; the structure was optimised away and its storage reused; or the
 * original link placed a data object over code.  The third is the least likely and
 * the first is not provable from here either.  What *is* provable is the arithmetic
 * above, and it is what this file transcribes.
 *
 * The nine, for reference:
 *
 *     func_000F924C  walk, zeroes 32 records
 *     func_000F9274  scale
 *     func_000F92A0  scale
 *     func_000F92F0  scale
 *     func_000F93E8  scale, this one
 *     func_000F9414  scale
 *     func_000F9438  scale
 *     func_000F9544  scale
 *     func_000F9590  scale
 */
#include "types.h"

/* The base address, as the original spells it: `lui 0xF` then `addiu -0x47B0`. */
#define TABLE_BASE_HI 0xF
#define TABLE_BASE_LO (-0x47B0)

/** Point at record `index` of the 28-byte-stride structure at 0x0EB850 and set its
 *  word at +0 to 10 and its words at +8 and +C to zero.  The word at +4 is left
 *  alone.  See the comment above: that address is inside `func_000EB840`'s body. */
__attribute__((noreturn)) void func_000F93E8(u32 index) {
    (void)index;
    __asm__ __volatile__(
        "sll  $a1, $a0, 5\n\t"
        "sll  $a0, $a0, 2\n\t"
        "subu $a0, $a1, $a0\n\t"
        "lui  $a1, %[hi]\n\t"
        "addiu $a1, $a1, %[lo]\n\t"
        "ori  $a2, $zero, 0xA\n\t"
        "addu $a0, $a0, $a1\n\t"
        "sw   $a2, 0x0($a0)\n\t"
        "sw   $zero, 0x8($a0)\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "sw   $zero, 0xC($a0)\n\t"
        ".set reorder\n\t"
        : : [hi] "i" (TABLE_BASE_HI), [lo] "i" (TABLE_BASE_LO)
        : "memory", "$a0", "$a1", "$a2");
}