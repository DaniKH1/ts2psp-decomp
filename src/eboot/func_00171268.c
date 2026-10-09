/**
 * The Sims 2 PSP - func_00171268 (0x00171268, 0x8 bytes)
 *
 *     jr    $ra
 *       or    $v0, $zero, $zero
 *
 * A default virtual method that returns 0.
 *
 * **It is entry 1 of the three-table family at `0x1EC528`, `0x1ECD88`
 * and `0x1EDB78`, and entry 2 is `func_00171280`, which returns 1.**
 * Two adjacent slots answering 0 and 1 is the "is X / is not X" pair
 * that several interfaces in this module open with - the
 * `sym_001EA3E8` family starts with two `return 1` methods, and the
 * `func_0018F6xx` run fills sixteen slots with `return 0` answers.
 *
 * **Entry 0 of the same three tables is `func_00196BE4`, which returns
 * `0x7F7FFFFF` - `FLT_MAX`** - and entry 3 is `func_00196C04`, a thunk
 * dispatch.  So the interface is: a limit, a negative predicate, a
 * positive one, and a call-through.  **The two predicates are the only
 * behaviour here that can differ between classes, and this one is the
 * negative half.**
 *
 * The address is worth noting on its own: this is a `return 0` at
 * `0x171268`, and `func_00154930` is another `return 0` at `0x154930`,
 * sixteen kilobytes away.  **The module contains many independent copies
 * of this two-instruction function**, one per base class that declares a
 * boolean method - the linker kept none of them.
 */
#include "types.h"

__attribute__((noreturn)) void func_00171268(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "or    $v0, $zero, $zero\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}