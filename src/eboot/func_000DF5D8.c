/**
 * The Sims 2 PSP - func_000DF5D8 (0x000DF5D8, 0x14 bytes)
 *
 * Stores three floats (all 0.0f) to consecutive offsets 0, 4, 8
 * of the second argument, returns 0.
 *
 *     mtc1 $zero, $f12
 *     swc1 $f12, 0x0($a1)
 *     swc1 $f12, 0x4($a1)
 *     jr   $ra
 *     swc1 $f12, 0x8($a1)
 *
 * **Three float stores, returns 0.**  Same pattern as func_00096F40.
 */
#include "types.h"

__attribute__((noreturn)) u32 func_000DF5D8(void *a0, void *a1) {
    (void)a0; (void)a1;
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "mtc1 $zero, $f12\n\t"
        "swc1 $f12, 0x0($a1)\n\t"
        "swc1 $f12, 0x4($a1)\n\t"
        "jr   $ra\n\t"
        "swc1 $f12, 0x8($a1)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$f12");
}