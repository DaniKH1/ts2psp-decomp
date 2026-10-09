/**
 * The Sims 2 PSP - func_00154930 (0x00154930, 0x8 bytes)
 *
 *     jr    $ra
 *       or    $v0, $zero, $zero
 *
 * A default virtual method that answers "no".
 *
 * **Its four neighbours - `func_00154908`, `func_00154910`,
 * `func_00154920` and `func_00154928` - are all `jr $ra` / `nop`, and
 * this one differs only in setting `$v0` first.**  The five together
 * are the whole of one base class's contribution to a large part of the
 * hierarchy, and this one appears in **47 of the 289 vtables** while
 * the four voids appear in 52, 53, 47 and 47.
 *
 * **The membership counts are close but not equal, and that is the
 * useful part.**  The four void stubs reach tables this `return 0`
 * does not, so the base class declares a method that some derived
 * classes replace with a void body and others replace with a boolean
 * answer.  **Where the base says `false` and a subclass says nothing,
 * the method has to be the void one** - the reverse is a compile error,
 * which is why the void variants are the more widely shared of the two
 * shapes.
 *
 * The whole run sits twelve bytes below `func_00154938`, which returns
 * the string `"gameObjectBehavior"` - **so this base class is the one
 * that gives the game's objects their type names.**
 */
#include "types.h"

__attribute__((noreturn)) void func_00154930(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "or    $v0, $zero, $zero\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}