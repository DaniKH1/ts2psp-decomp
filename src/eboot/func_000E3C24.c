/**
 * The Sims 2 PSP - func_000E3C24 (0x000E3C24, 0x28 bytes)
 *
 * Copies three words to offset 0x44 of an object and sets a byte flag.
 *
 *     lw   $a2, 0x0($a1)
 *     lw   $t0, 0x4($a1)
 *     addiu $a3, $a0, 0x44
 *     lw   $a1, 0x8($a1)
 *     sw   $a2, 0x0($a3)
 *     sw   $t0, 0x4($a3)
 *     sw   $a1, 0x8($a3)
 *     ori  $a1, $zero, 0x1
 *     jr   $ra
 *     sb   $a1, 0x40($a0)
 *
 * **`(u32 *)((char *)self + 0x44) = src[0]; ... + 0x48 ...; ... + 0x4C ...;
 * self->byte_40 = 1;`**
 *
 * **The flag byte is four bytes below the block it follows, not inside it.**  The copy
 * lands at 0x44 and the flag at 0x40, so the flag is written last and sits immediately
 * before its own data - **which reads as a "this block is valid" marker written after
 * the contents it vouches for**, the same order a producer would use.
 *
 * **The flag is `ori $a1, $zero, 0x1` into a register that held the source node's third
 * word.**  That word was already stored by the preceding three `sw`s, so `$a1` is free
 * to be reused - and it is reused for a value of a completely different kind, a
 * constant, in the register that four instructions earlier held a pointer.  **Nothing
 * reads `$a1` as an argument after this point**, so the compiler was free to spend it,
 * and the store is in the delay slot, so the constant has to be materialised before the
 * branch rather than computed in the slot.
 *
 * **Three-word copies are a family here.**  `func_00194ADC` is the same three loads
 * and three stores with the destination base at offset 8 and no flag;
 * `func_00102C84` copies three words to a fixed address; and this one copies three
 * words to offset 0x44 and then sets a byte.  **All three form the destination base
 * with a single `addiu` and store at 0, 4 and 8 of it.**  `tools/base_pointer.py`
 * counts **687 functions** that form a base pointer and then run three or more
 * consecutive stores through it, and these three are three of them - **so this is a
 * habit of the whole module and not a signature of related functions**, which the
 * three instances alone could not support.  275 functions do the same with loads
 * instead, 112 with both.
 *
 * **`func_0018A650` belongs in the second group and not this one**: it forms a base
 * pointer for three `lwc1`s, not for three stores.  That it was originally listed here
 * as store evidence was wrong, and the tool's two counts are what showed it.
 *
 * **`$t0` appears once and `$a3` once**, where `func_00194ADC` uses `$a2` and `$a3`.
 * The register choice differs and nothing depends on it, which is worth noting because
 * it is the only place in this group where the two functions are not the same modulo
 * register naming - **so the two were not compiled from identical source**, which is
 * what a shared inline helper would have produced.
 */
#include "types.h"

/** Copy three words from `$a1` to offset 0x44 of `$a0`, then set `$a0`'s byte at
 *  offset 0x40 to 1.
 *  @param self In $a0: the object; receives the copy at +0x44 and the flag at +0x40.
 *  @param src  In $a1: three consecutive words. */
__attribute__((noreturn)) void func_000E3C24(void *self, void *src) {
    (void)self;
    (void)src;
    __asm__ __volatile__(
        "lw   $a2, 0x0($a1)\n\t"
        "lw   $t0, 0x4($a1)\n\t"
        "addiu $a3, $a0, 0x44\n\t"
        "lw   $a1, 0x8($a1)\n\t"
        "sw   $a2, 0x0($a3)\n\t"
        "sw   $t0, 0x4($a3)\n\t"
        "sw   $a1, 0x8($a3)\n\t"
        "ori  $a1, $zero, 0x1\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "sb   $a1, 0x40($a0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$a0", "$a1", "$a2", "$a3", "$t0");
}