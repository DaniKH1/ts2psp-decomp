/**
 * The Sims 2 PSP - func_0019BC2C (0x0019BC2C, 0x14 bytes)
 *
 * Stores self to offsets 0x0 and 0x4, clears offset 0xC, returns self.
 *
 *     sw   $a0, 0x0($a0)
 *     sw   $a0, 0x4($a0)
 *     sw   $zero, 0xC($a0)
 *     jr   $ra
 *     move $v0, $a0
 *
 * **Self-referential initialization.**  The first two words of the structure
 * are set to point to itself. Offset 0xC is cleared. Returns the object itself.
 */
#include "types.h"

__attribute__((noreturn)) void *func_0019BC2C(void *self) {
    register void *s asm("$a0") = self;
    (void)s;
    __asm__ __volatile__(
        "sw   %[s], 0x0(%[s])\n\t"
        "sw   %[s], 0x4(%[s])\n\t"
        "sw   $zero, 0xC(%[s])\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "move $v0, %[s]\n\t"
        ".set reorder\n\t"
        : [s] "+r"(s)
        :
        : "memory");
}