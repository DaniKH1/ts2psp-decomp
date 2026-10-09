/**
 * The Sims 2 PSP - func_000AF58C (0x000AF58C, 0x8 bytes)
 *
 *     jr    $ra
 *       or    $v0, $a0, $zero
 *
 * A default virtual method that returns its own argument.
 *
 * **This is the value in slot +0x064 of ten of the eleven vtables**
 * (`sym_001EAA78` through `sym_001EB2E8`).  The eleventh,
 * `sym_001EB560`, points that slot at `func_00198140` instead, which
 * returns 0 - **so the slot has exactly two values but the split is
 * 10 against 1, not a balanced one.**  Read as a question the interface
 * asks, "return this" is what it means almost everywhere, and
 * `sym_001EB560` is the one class that answers no.
 *
 * That is the cheapest possible form of a behavioural difference: no
 * body, no state, just whether the object answers for itself.  Read
 * with slot +0x04C, where ten classes leave `func_0018F668` (the void
 * default) in place and the same eleventh class puts `func_000C00AC`
 * there, **the interface has two such switches and exactly one class
 * sets both.**
 *
 * 33 functions in the module are this exact two-instruction shape.
 * This one is not one of them by accident - it is the default the ten
 * leave alone, so it is inherited rather than written.
 */
#include "types.h"

__attribute__((noreturn)) void func_000AF58C(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "or    $v0, $a0, $zero\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}