/**
 * The Sims 2 PSP - func_00194AA8 (0x00194AA8, 0x8 bytes)
 *
 *     jr    $ra
 *       lw    $v0, 0x0($a0)
 *
 * The last of the eight offset-0 getters.
 *
 * **It sits 8 bytes before func_00194AB0, which is one of the nine +0x04
 * getters.**  So this address holds a getter pair of *different* offsets -
 * 0x00 and 0x04 - which is a much better signature than the identical
 * pairs at 0x0004E5CC and 0x00058078.  **An adjacent pair with different
 * bodies is one class's getter block; an adjacent pair with identical
 * bodies is one inlined getter emitted twice.**  Both shapes occur here,
 * and telling them apart is what turns the flat accessor list back into
 * per-class groups.
 */
#include "types.h"

__attribute__((noreturn)) void func_00194AA8(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "lw    $v0, 0x0($a0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}