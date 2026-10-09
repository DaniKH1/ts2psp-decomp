/**
 * The Sims 2 PSP - func_00154910 (0x00154910, 0x8 bytes)
 *
 *     jr    $ra
 *       nop
 *
 * A void default, byte-identical to `func_00154908`, `func_00154920`
 * and `func_00154928` - **and the most widely shared of the four, in 53
 * of the 289 vtables in `.data`.**
 *
 * **Four separate addresses rather than one is the finding.**  A single
 * empty base method would appear at one address in every table that
 * inherited it; four distinct addresses means the compiler emitted a
 * copy per override site rather than sharing one definition.
 *
 * The run of defaults around it - `func_00154908` (52 tables),
 * `func_00154920` (47), `func_00154928` (47), and `func_00154930`, which
 * returns 0 and appears in 47 - is **the shared base of the whole
 * object model**: roughly fifty vtables out of 289 inherit from a class
 * whose methods are all defaults, and the ones that do something
 * replace exactly one or two of these.
 *
 * `func_00154938`, twelve bytes further on, returns the string
 * `"gameObjectBehavior"` and is how the hierarchy names itself.
 */
#include "types.h"

__attribute__((noreturn)) void func_00154910(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "nop\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}