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
 * this one differs only in setting `$v0` first.**  The five are four
 * copies of one empty method and one `return 0`, at five addresses.
 *
 * **But this one is rare where the voids are common** - it appears in a
 * single run, against thirty-two for `func_00154908` and twenty-nine for
 * `func_00154928`.  That asymmetry is forced rather than accidental: a
 * subclass may replace a `false`-returning method with a void body, but
 * not a void one with a boolean.  **So the void defaults are the ones
 * most classes inherit untouched, and the `return 0` is a method almost
 * every derived class had to supply.**
 *
 * Twelve bytes past it sits `func_00154938`, which returns the string
 * `"gameObjectBehavior"` - **so this base class is the one that gives
 * the game's objects their type names.**
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