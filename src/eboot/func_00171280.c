/**
 * The Sims 2 PSP - func_00171280 (0x00171280, 0x8 bytes)
 *
 *     jr    $ra
 *       ori   $v0, $zero, 0x1
 *
 * A default virtual method that returns 1.
 *
 * **It is entry 2 of the three-table family at `0x1EC528`, `0x1ECD88`
 * and `0x1EDB78`, and entry 1 is `func_00171268`, which returns 0.**
 * Sixteen bytes apart in `.text` and adjacent in the table: **the pair
 * is one "is X" / "is not X" query about the same object**, the same
 * arrangement the `sym_001EA3E8` family opens with and the same one
 * `func_000B9D34` belongs to.
 *
 * Entry 0 of those tables is `func_00196BE4`, returning `FLT_MAX`, and
 * entry 3 is `func_00196C04`, a thunk dispatch - so this is the positive
 * half of a two-answer interface that also carries a limit and a
 * call-through.
 *
 * **The pairing is the interesting part, not the constant.**  Two
 * eight-byte functions, one `or` and one `ori`, four hundred bytes apart
 * in `.text` and adjacent in every table that uses them, together answer
 * one question twice.  A module that emitted one shared pair per base
 * class instead would have saved a handful of bytes and lost the ability
 * to override each half independently - **which is exactly the property
 * the three tables share.**
 */
#include "types.h"

__attribute__((noreturn)) void func_00171280(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "ori   $v0, $zero, 0x1\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}