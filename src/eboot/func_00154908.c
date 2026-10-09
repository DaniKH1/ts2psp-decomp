/**
 * The Sims 2 PSP - func_00154908 (0x00154908, 0x8 bytes)
 *
 *     jr    $ra
 *       nop
 *
 * A virtual method whose default does nothing and returns nothing.
 *
 * **It recurs at fixed depths across the module's object model.**  The
 * vtable entries here are eight bytes - a 32-bit adjustment word and a
 * function pointer - and this function sits at entry indices **1 and 6**
 * in whichever tables use it.  Those two positions repeat in tables that
 * have nothing else in common, **which is what a shared base class looks
 * like even though no scan can count the tables themselves**: they are
 * laid out back to back and nothing marks where one ends.
 *
 * **There is more than one copy of it.**  `func_00154908`,
 * `func_00154910`, `func_00154920` and `func_00154928` are all
 * `jr $ra` / `nop` at four different addresses, and `func_00154930` -
 * `return 0` - sits among them.  A single inherited empty method would
 * have one address in every table; four distinct addresses means **the
 * compiler emitted a copy at each override site and the linker merged
 * none of them.**
 *
 * The adjustment word of every entry measured in `.data` is zero, so
 * these tables have the layout of multiple-inheritance thunk arrays and
 * the behaviour of plain vtables - the same `{adjust, fnptr}` shape that
 * `func_0019D11C`, `func_000BE138` and `func_000BD3A4` read out of the
 * +0xD0 thunk arrays, with nothing to adjust.
 */

#include "types.h"

__attribute__((noreturn)) void func_00154908(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "nop\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}