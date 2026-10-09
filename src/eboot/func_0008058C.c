/**
 * The Sims 2 PSP - func_0008058C (0x0008058C, 0x8 bytes)
 *
 *     jr    $ra
 *       lw    $v0, 0x18($a0)
 *
 * A field getter returning the word at offset 0x18, **byte-identical to
 * func_00080584 which sits 8 bytes before it.**
 *
 * Two identical adjacent functions is the duplicated-getter signature,
 * and it is worth being precise about what it rules out: this is not a
 * second class with the same field number, and it is not a getter for a
 * different member of the same class.  **It is the same member, emitted
 * twice**, because the compiler inlined it into two call sites that
 * needed the function's address or could not share it.
 *
 * The module has three such identical pairs at offset 0 (0x0004E5CC,
 * 0x00058078, 0x00080288) and this is the first one at a non-zero
 * offset.
 */
#include "types.h"

__attribute__((noreturn)) void func_0008058C(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "lw    $v0, 0x18($a0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}