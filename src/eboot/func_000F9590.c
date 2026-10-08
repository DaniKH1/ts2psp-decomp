/**
 * The Sims 2 PSP - func_000F9590 (0x000F9590, 0x20 bytes)
 *
 * Reads one byte out of the 28-byte-stride structure at 0x0EB850.
 *
 *     sll  $a1, $a0, 5
 *     sll  $a0, $a0, 2
 *     subu $a0, $a1, $a0
 *     lui  $a1, 0xF
 *     addiu $a1, $a1, -0x47B0
 *     addu $a0, $a0, $a1
 *     jr   $ra
 *     lbu  $v0, 0x14($a0)
 *
 * **`return *(u8 *)(0x0EB850 + 28 * index + 0x14);`**
 *
 * The ninth of the nine functions `tools/stride_table.py` finds addressing 0x0EB850
 * with a stride of 28, and the only one that *reads* rather than writes.  Its
 * siblings set fields at +0, +8 and +C; this reads +0x14.  See that tool's census,
 * and `func_000F93E8` for the thing worth knowing about the base address: it is
 * sixteen bytes into `func_000EB840`'s prologue and carries two `jal` relocations,
 * so this load reads a `move $s1, $a1` as a byte for index 0.
 *
 * The pointer arithmetic is the same `index << 5 - index << 2` as everywhere else in
 * the cluster, and the load is in the return's delay slot - so `.set noreorder`
 * around the branch, and nothing after it.
 */
#include "types.h"

/* The base address, as the original spells it: `lui 0xF` then `addiu -0x47B0`. */
#define TABLE_BASE_HI 0xF
#define TABLE_BASE_LO (-0x47B0)

/** @param index Which of the records; nothing constrains it to a valid range.
 *  @return The byte at offset 0x14 of that record. */
__attribute__((noreturn)) u32 func_000F9590(u32 index) {
    (void)index;
    __asm__ __volatile__(
        "sll  $a1, $a0, 5\n\t"
        "sll  $a0, $a0, 2\n\t"
        "subu $a0, $a1, $a0\n\t"
        "lui  $a1, %[hi]\n\t"
        "addiu $a1, $a1, %[lo]\n\t"
        "addu $a0, $a0, $a1\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "lbu  $v0, 0x14($a0)\n\t"
        ".set reorder\n\t"
        : : [hi] "i" (TABLE_BASE_HI), [lo] "i" (TABLE_BASE_LO)
        : "memory", "$v0", "$a0", "$a1");
}