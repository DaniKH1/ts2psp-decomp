/**
 * The Sims 2 PSP - func_00055974 (0x00055974, 0x4C bytes)
 *
 * Copies a three-word node, four floats and two bytes into an object.
 *
 *     lw   $a3, 0x0($a1)
 *     lw   $t1, 0x4($a1)
 *     addiu $t0, $a0, 0xAC
 *     lw   $a1, 0x8($a1)
 *     sw   $a3, 0x0($t0)
 *     sw   $t1, 0x4($t0)
 *     sw   $a1, 0x8($t0)
 *     lwc1 $f12, 0x0($a2)
 *     swc1 $f12, 0x68($a0)
 *     lwc1 $f12, 0x4($a2)
 *     ori  $a1, $zero, 0x1
 *     swc1 $f12, 0x6C($a0)
 *     lwc1 $f12, 0x8($a2)
 *     swc1 $f12, 0x70($a0)
 *     lwc1 $f12, 0xC($a2)
 *     sb   $a1, 0x10A($a0)
 *     swc1 $f12, 0x74($a0)
 *     jr   $ra
 *     sb   $a1, 0x10B($a0)
 *
 * **`*(u32 *)((char *)self + 0xAC) = node[0..2]; *(f32 *)(self + 0x68) = v.x; ...;
 * self->byte_10A = 1; self->byte_10B = 1;`**
 *
 * **This function uses both spellings of the same idea, and that is the interesting
 * part of it.**
 *
 * The three-word copy forms a base - `addiu $t0, $a0, 0xAC` - and stores at 0, 4 and 8
 * of it.  The four-float copy does **not**: it reads `0x0`, `0x4`, `0x8`, `0xC` of
 * `$a2` and writes `0x68`, `0x6C`, `0x70`, `0x74` of `$a0`, every one with an immediate
 * offset.  **One `addiu` and eight accesses against eight accesses with immediates is a
 * tie on instruction count**, and for the float half the compiler took the immediate
 * form.
 *
 * **So `tools/base_pointer.py`'s 687 is not a preference the compiler has and sometimes
 * loses - it is a choice it makes in one function and not the other, twelve
 * instructions apart.**  What decides it is not established, and the obvious
 * candidates all fail to explain this instance: the word copy needs a fresh register
 * (`$t0`) and the float copy would too, so register pressure is not it; `$a0` is needed
 * again for `sb $a1, 0x10A($a0)`, so preserving the object pointer is a reason but not
 * a difference between the two halves.  **The observation stands and the mechanism
 * does not, and writing down a mechanism here would be inventing one.**
 *
 * **The two flag bytes are set to 1 and nothing ever clears them in this function** -
 * they are at 0x10A and 0x10B, a hundred and seventy bytes past the end of the float
 * block at 0x74 and sixty-two past the end of the word block at 0xB4.  **So the object
 * has a pair of adjacent bytes somewhere else entirely that this function only ever
 * turns on**, which is what a "seen" or "valid" pair looks like, and the offsets being
 * unrelated to either copy is what says they belong to a third thing.
 *
 * **`ori $a1, $zero, 0x1` is materialised between the second and third float**, and
 * `$a1` held the source node's third word, already stored by then.  The two `sb`s are
 * 0x10A and 0x10B, so the flag is a *pair* of adjacent bytes both set to the same
 * constant, not a two-byte value - **which is the difference between marking something
 * with two flags and writing one halfword of 0x0101**, and the `sb`/`sb` pair settles it
 * as the former.
 */
#include "types.h"

/** Copy a three-word node to +0xAC, four floats to +0x68, and set two flag bytes at
 *  +0x10A and +0x10B to 1.
 *  @param self In $a0: the object.
 *  @param node In $a1: three words.
 *  @param v    In $a2: four floats. */
__attribute__((noreturn)) void func_00055974(void *self, void *node, void *v) {
    (void)self;
    (void)node;
    (void)v;
    __asm__ __volatile__(
        "lw   $a3, 0x0($a1)\n\t"
        "lw   $t1, 0x4($a1)\n\t"
        "addiu $t0, $a0, 0xAC\n\t"
        "lw   $a1, 0x8($a1)\n\t"
        "sw   $a3, 0x0($t0)\n\t"
        "sw   $t1, 0x4($t0)\n\t"
        "sw   $a1, 0x8($t0)\n\t"
        "lwc1 $f12, 0x0($a2)\n\t"
        "swc1 $f12, 0x68($a0)\n\t"
        "lwc1 $f12, 0x4($a2)\n\t"
        "ori  $a1, $zero, 0x1\n\t"
        "swc1 $f12, 0x6C($a0)\n\t"
        "lwc1 $f12, 0x8($a2)\n\t"
        "swc1 $f12, 0x70($a0)\n\t"
        "lwc1 $f12, 0xC($a2)\n\t"
        "sb   $a1, 0x10A($a0)\n\t"
        "swc1 $f12, 0x74($a0)\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "sb   $a1, 0x10B($a0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$a0", "$a1", "$a3", "$t0", "$t1", "$f12");
}