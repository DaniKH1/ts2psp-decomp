/**
 * The Sims 2 PSP - func_00154920 (0x00154920, 0x8 bytes)
 *
 *     jr    $ra
 *       nop
 *
 * A void default, byte-identical to `func_00154908`, `func_00154910`
 * and `func_00154928`, and present in **47 of the 289 vtables**.
 *
 * It is the third of the four, and its slot positions are what give the
 * group away.  Measured across the tables that use it, these defaults
 * land at slots 10 through 40 in **every one** - `func_00154908` at
 * slots 12, 14, 18, 24; `func_00154910` at 10, 12, 16, 22;
 * `func_00154920` at 16, 18, 22, 28; `func_00154928` at 18, 20, 24, 30.
 * **The same four addresses recur at overlapping slot ranges in
 * unrelated tables, which is the signature of one base class with four
 * empty methods in a row rather than four unrelated coincidences.**
 *
 * The nearest non-default in the group is `func_00154930`, also eight
 * bytes, which returns 0 and shares the same 47-table membership.
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