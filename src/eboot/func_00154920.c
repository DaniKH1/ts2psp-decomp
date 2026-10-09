/**
 * The Sims 2 PSP - func_00154920 (0x00154920, 0x8 bytes)
 *
 *     jr    $ra
 *       nop
 *
 * A void default, byte-identical to `func_00154908`, `func_00154910`
 * and `func_00154928`.
 *
 * **It appears in far fewer tables than its neighbours** - five runs,
 * against thirty-two for `func_00154908` and twenty-nine for
 * `func_00154928` - and that difference is the useful part.  The four
 * are four distinct overrides of four distinct base methods, not four
 * spellings of one: **the two that most classes inherit unchanged are
 * the two the compiler emitted copies of at nearly every site, and this
 * one is a method most derived classes replaced.**
 *
 * Its entry indices run 2 and 45 where it appears, so it is not confined
 * to the base class's own slots either.
 *
 * `func_00154930`, sixteen bytes further on, is `return 0` and is
 * equally rare; `func_00154938`, twelve bytes past that, returns the
 * class name `"gameObjectBehavior"`.
 */

#include "types.h"

__attribute__((noreturn)) void func_00154920(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "nop\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}