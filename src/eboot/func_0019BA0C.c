/**
 * The Sims 2 PSP - func_0019BA0C (0x0019BA0C, 0x8 bytes)
 *
 *     jr    $ra
 *       or    $v0, $zero, $zero
 *
 * A default virtual method: returns 0 and does nothing else.  It is a
 * complete body, not a stub placeholder in the source - the `or` sits in
 * the delay slot of the `jr`, which is the only two instructions the
 * function has.
 *
 *
 * This is slot +0x58 of the shared tail of vtable
 * `sym_001EA3E8` - the sixteen slots from +0x48 to +0xC8 that are identical in
 * all three sibling classes.  Every one of them is a default stub that
 * answers zero.  A block of this shape is a set of base-class questions
 * none of these three classes overrides: the interface exists, and the
 * answer is "no" to all sixteen.
 */
#include "types.h"

__attribute__((noreturn)) void func_0019BA0C(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "or    $v0, $zero, $zero\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
