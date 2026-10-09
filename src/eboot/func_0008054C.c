/**
 * The Sims 2 PSP - func_0008054C (0x0008054C, 0x8 bytes)
 *
 *     jr    $ra
 *       lw    $v0, 0x20($a0)
 *
 * A field getter returning the word at offset 0x20 - and **the only
 * getter in the module that reads 0x20.**
 *
 * That makes it the highest-offset word the accessor tier touches
 * apart from the outliers at 0x70 and beyond, which are read by single
 * functions rather than by a group.  In the class block at
 * func_000804B8 the offset sequence runs 0x04, 0x18, 0x1C, 0x20, so
 * **0x20 is the last word the class exposes and the object is close to
 * 0x24 bytes plus whatever `func_000805D4` reaches.**
 */
#include "types.h"

__attribute__((noreturn)) void func_0008054C(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "lw    $v0, 0x20($a0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}