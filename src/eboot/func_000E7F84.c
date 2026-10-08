/**
 * The Sims 2 PSP - func_000E7F84 (0x000E7F84, 0x18 bytes)
 *
 * Loads a pointer from offset 0x18, adds index*4 from second argument,
 * stores the first argument at the result.
 *
 *     lui  $a2, %hi(sym_000E97A8)
 *     sll  $a1, $a1, 2
 *     addiu $a2, $a2, %lo(sym_000E97A8)
 *     addu $a1, $a1, $a2
 *     jr   $ra
 *     sw   $a0, 0x0($a1)
 *
 * **Array element store with base at 0xE97A8.**  Index in $a1 is
 * scaled by 4, added to base at 0xE97A8, then value stored.
 */
#include "types.h"

__attribute__((noreturn)) void func_000E7F84(void *self, u32 index, u32 value) {
    (void)self; (void)index; (void)value;
    __asm__ __volatile__(
        "lui  $a2, %%hi(sym_000E97A8)\n\t"
        "sll  $a1, $a1, 2\n\t"
        "addiu $a2, $a2, %%lo(sym_000E97A8)\n\t"
        "addu $a1, $a1, $a2\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "sw   $a0, 0x0($a1)\n\t"
        ".set reorder\n\t"
        :
        : "r"(self), "r"(index), "r"(value)
        : "memory", "$a0", "$a1", "$a2");
}