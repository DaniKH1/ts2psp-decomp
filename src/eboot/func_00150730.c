/**
 * The Sims 2 PSP - func_00150730 (0x00150730, 0x20 bytes)
 *
 * Copies three floats and returns the destination.
 *
 *     lwc1 $f12, 0x0($a1)
 *     move $v0, $a0
 *     swc1 $f12, 0x0($a0)
 *     lwc1 $f12, 0x4($a1)
 *     swc1 $f12, 0x4($a0)
 *     lwc1 $f12, 0x8($a1)
 *     jr   $ra
 *     swc1 $f12, 0x8($a0)
 *
 * **`*(f32 *)dst = *(f32 *)src; return dst;`**  - the three-float copy of
 * `func_00150988`, with a return value.
 *
 * **This is the sixth member of the three-float copy family** - `func_00150988`,
 * `func_00193358`, `func_001965E8`, `func_00196608` and `updateNodeGraph_0E48` are the
 * other five - and it is the only one that returns anything.  The other five are the
 * same eight instructions without the `move $v0, $a0`, so **the copy and the return are
 * one function and a void one, six times out of seven.**
 *
 * **The return value is formed before the copy, not after.**  `move $v0, $a0` is the
 * second instruction, before the first store, so `$v0` holds the destination pointer
 * even though `$a0` is clobbered by the copy's stores - **the stores are through `$a0`
 * but do not modify it**, so the copy could have returned `$a0` at the end too.  Putting
 * it early costs nothing and lets the last delay slot carry the final `swc1` instead of
 * the `move`, which is the same trade `func_0009232C` makes with its `move $v0, $a0`.
 *
 * **Six copies of eight instructions is enough to say the shape is a habit**, and the
 * other five all use the *other* spelling: `func_00150988`, `func_00193358`,
 * `func_001965E8` and `func_00196608` begin with an `addiu` that forms a base pointer,
 * and this one does not - every offset here is immediate.  **Both forms are eight
 * instructions, and the difference is what the eighth is spent on.**  In the void five it
 * is the `addiu`; here it is `move $v0, $a0`.  **So this function did not drop the base
 * pointer to make room for the return - it spent the instruction the base pointer would
 * have occupied on the return instead**, and the two are the same length either way.
 *
 * That is the one place in this module where the choice between the two forms is
 * visible *within a single family*, which makes it worth having found.  It is not a
 * mechanism, and `func_00055974` - three words through a formed base and four floats
 * through immediate offsets in the same body, neither with a return - is the case that
 * still needs explaining.  `tools/base_pointer.py` counts 687 functions of the
 * base-pointer form and this is not one of them; **the count says which is commoner,
 * and this instance is the reason not to read that as a rule.**
 */
#include "types.h"

/** Copy three floats from `$a1` to `$a0`.
 *  @param dst In $a0: receives three floats, and is returned.
 *  @param src In $a1: three floats. */
__attribute__((noreturn)) void *func_00150730(void *dst, void *src) {
    (void)dst;
    (void)src;
    __asm__ __volatile__(
        "lwc1 $f12, 0x0($a1)\n\t"
        "move $v0, $a0\n\t"
        "swc1 $f12, 0x0($a0)\n\t"
        "lwc1 $f12, 0x4($a1)\n\t"
        "swc1 $f12, 0x4($a0)\n\t"
        "lwc1 $f12, 0x8($a1)\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "swc1 $f12, 0x8($a0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "$v0", "$f12");
}