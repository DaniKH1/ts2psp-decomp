/**
 * The Sims 2 PSP - func_00194ADC (0x00194ADC, 0x20 bytes)
 *
 * Copies a three-word node to offset 8 of another, link included.
 *
 *     lw   $a2, 0x0($a1)
 *     lw   $a3, 0x4($a1)
 *     addiu $a0, $a0, 0x8
 *     lw   $a1, 0x8($a1)
 *     sw   $a2, 0x0($a0)
 *     sw   $a3, 0x4($a0)
 *     jr   $ra
 *     sw   $a1, 0x8($a0)
 *
 * **`(u32 *)((char *)dst + 8) = *(u32 *)src; ... + 0xC ...; ... + 0x10 ...;`**  - a
 * three-word copy whose destination base is formed once.
 *
 * **The third word copied is a pointer, and it is the source node's own link.**  So
 * the copy is not a memcpy of unrelated data: it duplicates a node *and keeps its
 * position in a chain*, which means the result shares its successor with the original.
 * Nothing in these seven instructions suggests the two nodes are meant to diverge
 * afterwards, and nothing here splits them either - **that is a fact about the call
 * sites, and there are none to read.**
 *
 * **`addiu $a0, $a0, 0x8` sits between the two source loads and the three stores.**
 * The compiler had the choice of `sw $a2, 0x8($a0)` and friends, three stores each
 * recomputing an offset from `$a0`, or one `addiu` and three stores at 0, 4 and 8 of the
 * new base.  **This is the base-pointer habit again** - `func_0018A650` forms the
 * source offset once for three `lwc1`s, `func_00102D34` forms it once for seventeen
 * stores - and here it is eight bytes cheaper than the alternative, not a wash.
 *
 * **Two of the three source loads happen before the base is formed and the third after
 * it**, which is the scheduler interleaving a register that dies with the other two.
 * `$a1` is dead after its last load, so `$a0` is free to take the new base at that
 * point.
 *
 * `func_000E3C24` is the same three-word copy with the destination at offset 0x44 and a
 * byte flag set afterwards, and `func_00102C84` copies three words to a fixed address.
 * **Three-word copies are a family here and the count is not established** - the shape
 * is common enough to appear in three functions without any of them being alike in
 * what they do with the result.
 */
#include "types.h"

/** Copy three words from `$a1` to offset 8 of `$a0`.
 *  @param dst In $a0: receives the copy at +0x08.
 *  @param src In $a1: three consecutive words, the third being a link. */
__attribute__((noreturn)) void func_00194ADC(void *dst, void *src) {
    (void)dst;
    (void)src;
    __asm__ __volatile__(
        "lw   $a2, 0x0($a1)\n\t"
        "lw   $a3, 0x4($a1)\n\t"
        "addiu $a0, $a0, 0x8\n\t"
        "lw   $a1, 0x8($a1)\n\t"
        "sw   $a2, 0x0($a0)\n\t"
        "sw   $a3, 0x4($a0)\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "sw   $a1, 0x8($a0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$a0", "$a1", "$a2", "$a3");
}