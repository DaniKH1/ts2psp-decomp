/**
 * The Sims 2 PSP - func_00154910 (0x00154910, 0x8 bytes)
 *
 *     jr    $ra
 *       nop
 *
 * A void default, byte-identical to `func_00154908`, `func_00154920`
 * and `func_00154928` - **and one of four separate copies of the same
 * two instructions.**
 *
 * **Four addresses rather than one is the finding.**  A single empty base
 * method would appear at the same address in every table that inherited
 * it.  `func_00154908`, `func_00154910`, `func_00154920` and
 * `func_00154928` are all `jr $ra` / `nop` at four distinct addresses, so
 * the compiler emitted a copy at each override site and the linker
 * deduplicated none of them.
 *
 * **How far each copy spread is measurable and they are not equal** -
 * `func_00154908` recurs at entry indices 1 and 6 across the module's
 * tables, `func_00154928` at 2 and 7, while this one appears in only a
 * single run and `func_00154920` in five.  **So the four are not
 * interchangeable:** they are distinct overrides of distinct base
 * methods, and the two widely-used ones are the slots that most classes
 * leave alone.
 *
 * Twelve bytes past `func_00154930` sits `func_00154938`, which returns
 * the string `"gameObjectBehavior"` - the class-name getter that makes
 * this whole group worth reading.
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