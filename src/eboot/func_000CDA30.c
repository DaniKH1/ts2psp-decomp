/**
 * The Sims 2 PSP - func_000CDA30 (0x000CDA30, 0x24 bytes)
 *
 * Stores self to offsets 0x0 and 0x4, clears offsets 0x8 through 0x18,
 * returns self.
 *
 *     sw   $a0, 0x0($a0)
 *     sw   $a0, 0x4($a0)
 *     sw   $zero, 0x8($a0)
 *     sw   $zero, 0xC($a0)
 *     sw   $zero, 0x10($a0)
 *     sw   $zero, 0x14($a0)
 *     sw   $zero, 0x18($a0)
 *     jr   $ra
 *     move $v0, $a0
 *
 * **Self-referential initialization with extended clearing.**
 * Stores self at offsets 0 and 4, clears words at 0x8 through 0x18.
 * Returns the object itself.
 */
#include "types.h"

__attribute__((noreturn)) void *func_000CDA30(void *self) {
    register void *s asm("$a0") = self;
    (void)s;
    __asm__ __volatile__(
        "sw   %[s], 0x0(%[s])\n\t"
        "sw   %[s], 0x4(%[s])\n\t"
        "sw   $zero, 0x8(%[s])\n\t"
        "sw   $zero, 0xC(%[s])\n\t"
        "sw   $zero, 0x10(%[s])\n\t"
        "sw   $zero, 0x14(%[s])\n\t"
        "sw   $zero, 0x18(%[s])\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "move $v0, %[s]\n\t"
        ".set reorder\n\t"
        : [s] "+r"(s)
        :
        : "memory");
}