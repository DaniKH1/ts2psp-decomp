/**
 * The Sims 2 PSP - func_00198140 (0x00198140, 0x8 bytes)
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
 * This is slot +0x60 of the shared tail of vtable
 * `sym_001EA3E8` - the sixteen slots from +0x48 to +0xC8 that are identical in
 * all three sibling classes.  Every one of them is a default stub that
 * answers zero.  A block of this shape is a set of base-class questions
 * none of these three classes overrides: the interface exists, and the
 * answer is "no" to all sixteen.
 *
 * **The same function is slot +0x064 of the second vtable family** - the
 * eleven records at a 0xD8 stride from `sym_001EAA78` to `sym_001EB560`.
 * Both are the twelfth slot, so the two families agree on where this method
 * lives.  What differs is that the first family leaves it alone in all three
 * classes while the second family splits it: ten or eleven of those put this
 * `return 0` here and the others point the slot at `func_000AF58C`, which
 * returns its argument instead.
 *
 * That makes this one function the clearest evidence that **the two families
 * share a base class and the eleven extend it**: a slot that is invariant in
 * the first and variable in the second is a virtual the first declined to
 * override and the second did, in some of its leaves.  Which leaves is not
 * decided by this file; the eleven records are.
 */
#include "types.h"

__attribute__((noreturn)) void func_00198140(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "or    $v0, $zero, $zero\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
