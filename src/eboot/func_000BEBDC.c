/**
 * The Sims 2 PSP - func_000BEBDC (0x000BEBDC, 0x8 bytes)
 *
 *     jr    $ra
 *       lw    $v0, 0x1C($a0)
 *
 * A field getter returning the word at offset 0x1C.
 *
 * **This one matters for a reason the address gives away.**  It sits at
 * 0x000BEBDC, inside the run of thirteen identical +0x24 getters that
 * runs from 0x000BD5F4 to 0x000BE8B8 - and it is one of only two
 * functions in that run that is *not* a 0x24 getter.  (The other is
 * func_000BED6C, 8 bytes later, also reading 0x1C.)
 *
 * So the 0x24 run is not thirteen copies of one accessor: **it is one
 * class's accessor block in which one member happens to have thirteen
 * getters and another has two.**  The offsets present in the block are
 * 0x1C and 0x24, which is a much stronger statement about the class
 * than the flat census - and it is what makes func_000BED6C worth
 * writing next to this one.
 */
#include "types.h"

__attribute__((noreturn)) void func_000BEBDC(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "lw    $v0, 0x1C($a0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}