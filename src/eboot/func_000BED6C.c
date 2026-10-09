/**
 * The Sims 2 PSP - func_000BED6C (0x000BED6C, 0x8 bytes)
 *
 *     jr    $ra
 *       lw    $v0, 0x1C($a0)
 *
 * A field getter returning the word at offset 0x1C, **byte-identical to
 * func_000BEBDC, 8 bytes before it**, and both of them sitting inside the
 * run of thirteen +0x24 getters.
 *
 * Two readings are possible for that pair and the bytes do not choose:
 *
 *  - the duplicated-getter shape, the same as func_00080584/func_0008058C
 *    and the three pairs at offset 0; or
 *  - two genuinely different members of the same class that both happen
 *    to be single words reachable at 0x1C from their own `this`.
 *
 * **The second is not available here**, because both take `$a0`
 * unmodified - a `this`-adjusted accessor would have to add something,
 * as `func_000805D4` does.  So this pair is the duplicate shape, and
 * what it adds to the 0x24 block is the offset 0x1C rather than a second
 * kind of accessor.
 */
#include "types.h"

__attribute__((noreturn)) void func_000BED6C(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "lw    $v0, 0x1C($a0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}