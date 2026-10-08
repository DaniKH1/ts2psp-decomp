/**
 * The Sims 2 PSP - func_000F9274 (0x000F9274, 0x2C bytes)
 *
 * Sets a bit in the first word of a record and writes a value into its second.
 *
 *     sll  $a2, $a0, 5
 *     sll  $a0, $a0, 2
 *     subu $a0, $a2, $a0
 *     lui  $a2, 0xF
 *     addiu $a2, $a2, -0x47B0
 *     addu $a0, $a0, $a2
 *     lw   $a2, 0x0($a0)
 *     sw   $a1, 0x4($a0)
 *     ori  $a1, $a2, 0x10
 *     jr   $ra
 *     sw   $a1, 0x0($a0)
 *
 * **`record->flags |= 0x10; record->field4 = value;`**  - in that order, over the
 * 28-byte-stride structure at 0x0EB850.
 *
 * The two halves of the word at +0 are kept apart for as long as the code can
 * afford: the flags word is read into `$a2`, which frees `$a1` for the second
 * argument so the store at +4 happens *before* the `ori`, and only then is the flag
 * word written back.  A source-level `record->flags |= 0x10; record->field4 = value;`
 * produces exactly this order.
 *
 * `$a0` is doing three jobs - the index, the scaled offset, and the record pointer -
 * which is why the scaling is `index << 5 - index << 2` into `$a2` first: the index
 * has to survive long enough to be scaled before `$a0` is reused as the pointer.
 * The subtract is `$a2 - $a0`, not `$a1 - $a2` as `tools/c_shapes.py --show` prints
 * it: `mipsdis` picks the mnemonic from the opcode's preferred operand order, so a
 * listing and a byte comparison can disagree about which way round a commutative
 * operation reads, and only the bytes settle it.
 *
 * One of the nine functions `tools/stride_table.py` finds addressing 0x0EB850, and
 * one of the seven of those that write.  About the base address itself, see
 * `func_000F93E8`: it is inside `func_000EB840`'s prologue and carries two `jal`
 * relocations, so this function's read-modify-write turns `move $s1, $a1` into
 * `move $s1, $a1 | 0x10` for index 0.
 */
#include "types.h"

/* The base address, as the original spells it: `lui 0xF` then `addiu -0x47B0`. */
#define TABLE_BASE_HI 0xF
#define TABLE_BASE_LO (-0x47B0)

/** Set bit 4 of the first word of record `index`, and store `value` in its second
 *  word.  `value` arrives in $a1 and `index` in $a0. */
__attribute__((noreturn)) void func_000F9274(u32 index, u32 value) {
    (void)index;
    (void)value;
    __asm__ __volatile__(
        "sll  $a2, $a0, 5\n\t"
        "sll  $a0, $a0, 2\n\t"
        "subu $a0, $a2, $a0\n\t"
        "lui  $a2, %[hi]\n\t"
        "addiu $a2, $a2, %[lo]\n\t"
        "addu $a0, $a0, $a2\n\t"
        "lw   $a2, 0x0($a0)\n\t"
        "sw   $a1, 0x4($a0)\n\t"
        "ori  $a1, $a2, 0x10\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "sw   $a1, 0x0($a0)\n\t"
        ".set reorder\n\t"
        : : [hi] "i" (TABLE_BASE_HI), [lo] "i" (TABLE_BASE_LO)
        : "memory", "$a0", "$a1", "$a2");
}