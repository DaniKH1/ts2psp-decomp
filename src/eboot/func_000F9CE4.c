/**
 * The Sims 2 PSP - func_000F9CE4 (0x000F9CE4, 0x18 bytes)
 *
 * Loads a pointer from offset 0x10, loads a pointer from offset 0x18,
 * computes index*4, adds to base, loads value, returns it.
 *
 *     lw   $a1, 0x10($a0)
 *     lw   $a0, 0x18($a0)
 *     sll  $a1, $a1, 2
 *     addu $a0, $a0, $a1
 *     jr   $ra
 *     lw   $v0, 0x0($a0)
 *
 * **Loads a value from an array at index.**  The array base is at
 * offset 0x18, index at 0x10 is scaled by 4 and added to base.
 */
#include "types.h"

__attribute__((noreturn)) u32 func_000F9CE4(void *self) {
    register void *p asm("$a0") = self;
    __asm__ __volatile__(
        "lw   $a1, 0x10(%[p])\n\t"
        "lw   $a0, 0x18(%[p])\n\t"
        "sll  $a1, $a1, 2\n\t"
        "addu $a0, $a0, $a1\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "lw   $v0, 0x0($a0)\n\t"
        ".set reorder\n\t"
        : [p] "+r"(p)
        :
        : "memory");
}