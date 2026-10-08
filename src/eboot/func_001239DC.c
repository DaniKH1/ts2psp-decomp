/**
 * The Sims 2 PSP - func_001239DC (0x001239DC, 0x28 bytes)
 *
 * Stores second argument at offset 0, clears offsets 0x4 through 0x14,
 * sets byte at 0x18 to 1, returns self.
 *
 *     sw   $a1, 0x0($a0)
 *     sw   $zero, 0x4($a0)
 *     sw   $zero, 0x8($a0)
 *     sw   $zero, 0xC($a0)
 *     sw   $zero, 0x10($a0)
 *     ori  $a1, $zero, 0x1
 *     sw   $zero, 0x14($a0)
 *     sb   $a1, 0x18($a0)
 *     jr   $ra
 *     move $v0, $a0
 *
 * **Self-referential initialization with flag byte.**
 * Stores second argument at offset 0, clears words at 0x4-0x14,
 * sets byte at 0x18 to 1. Returns the object itself.
 */
#include "types.h"

__attribute__((noreturn)) void *func_001239DC(void *self, void *arg) {
    register void *s asm("$a0") = self;
    register u32 a1_reg asm("$a1");
    (void)self;
    __asm__ __volatile__(
        "sw   %[a1], 0x0(%[s])\n\t"
        "sw   $zero, 0x4(%[s])\n\t"
        "sw   $zero, 0x8(%[s])\n\t"
        "sw   $zero, 0xC(%[s])\n\t"
        "sw   $zero, 0x10(%[s])\n\t"
        "ori  %[a1], $zero, 0x1\n\t"
        "sw   $zero, 0x14(%[s])\n\t"
        "sb   %[a1], 0x18(%[s])\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "move $v0, %[s]\n\t"
        ".set reorder\n\t"
        : [s] "+r"(self), [a1] "+r"(a1_reg)
        :
        : "memory");
}