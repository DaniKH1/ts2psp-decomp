/**
 * The Sims 2 PSP - func_000FBFD4 (0x000FBFD4, 0x1C bytes)
 *
 * Compares two words in an object and returns whether they differ.
 *
 *     lw    $a1, 0x208($a0)
 *     lw    $a0, 0x20C($a0)
 *     xor   $a0, $a1, $a0
 *     sltiu $a0, $a0, 0x1
 *     andi  $v0, $a0, 0xFF
 *     jr    $ra
 *     sltiu $v0, $v0, 0x1
 *
 * **`return *(u32 *)((char *)self + 0x208) != *(u32 *)((char *)self + 0x20C);`**
 *
 * **One `xor` and two signed-looking steps to say "not equal".**  `xor` of two values
 * is zero exactly when they are equal, so the whole function is a zero test on the
 * exclusive-or - and psp-gcc spells a zero test as `sltiu rt, rs, 1`, which is the
 * unsigned form of `rs == 0`.  The second `sltiu $v0, $v0, 1` then flips it, giving
 * `!=` from `==`.
 *
 * **The `andi 0xFF` in the middle is dead**, and it is the same dead mask as
 * `func_000BF49C`'s: `sltiu` against an immediate of 1 produces exactly 0 or 1, so
 * masking with 0xFF cannot change it.  **Those two functions are 2 of the 168 the
 * module uses that shape in** (`tools/boolean_shapes.py`), and the other two examples
 * the tool shows are `xor`, `sltiu 1`, `andi 0xFF`, `bnez` / `beqz` - the same
 * zero-test idiom with a different flag word.  **So this is the module's standard
 * spelling of "is this zero" and the mask is dead in all 168**, which is a compiler
 * habit visible at scale rather than a slip in one function.
 *
 * **The two `sltiu` are not the same instruction.**  The first is `$a0 = ($a0 < 1)`,
 * an unsigned comparison whose operand is the caller's data; the second is
 * `$v0 = ($v0 < 1)` on a value that is already 0 or 1, so it is a logical NOT.  Same
 * mnemonic, same immediate, no relation between them - which is why the redundant
 * `andi` between them reads as deliberate narrowing of a *different* value and is in
 * fact nothing at all.
 *
 * **The fields are four bytes apart, at 0x208 and 0x20C**, so this compares two
 * adjacent words and returns one bit about them.  `func_001A9ABC` is the same idea at
 * a different scale - one bit of one word - and both are in the set
 * `tools/flag_accessors.py` would want if it counted comparison accessors rather than
 * mask accessors.
 */
#include "types.h"

/** Test whether the words at offset 0x208 and 0x20C differ.
 *  @param self In $a0: the object.
 *  @return     1 if they differ, 0 if equal, in `$v0`. */
__attribute__((noreturn)) long func_000FBFD4(void *self) {
    (void)self;
    __asm__ __volatile__(
        "lw    $a1, 0x208($a0)\n\t"
        "lw    $a0, 0x20C($a0)\n\t"
        "xor   $a0, $a1, $a0\n\t"
        "sltiu $a0, $a0, 0x1\n\t"
        "andi  $v0, $a0, 0xFF\n\t"
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "sltiu $v0, $v0, 0x1\n\t"
        ".set reorder\n\t"
        :
        :
        : "$v0", "$a0", "$a1");
}