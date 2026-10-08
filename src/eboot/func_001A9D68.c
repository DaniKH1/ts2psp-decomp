/**
 * The Sims 2 PSP - func_001A9D68 (0x001A9D68, 0x18 bytes)
 *
 * Loads a pointer from offset 0x18, masks it with 0xFFFF0000 (keeps upper
 * 16 bits, clears lower 16), stores it back to offset 0x18.
 *
 *     lw   $a1, 0x18($a0)
 *     lui  $a2, 0xFFF0
 *     addiu $a2, $a2, -0x1     0xFFF00000 - 0x1 = 0xFEFFFFF? Wait...
 *     and  $a1, $a1, $a2
 *     jr   $ra
 *     sw   $a1, 0x18($a0)
 *
 * **Wait: `lui $a2, 0xFFF0` + `addiu -0x1` = 0xFFF00000 - 0x1 = 0xFEFFFFF**.
 * That's not 0xFFFF0000. Let me check: 0xFFF0 << 16 = 0xFFF00000. -0x1 = 0xFFEFFFFF.
 * That masks to keep upper 12 bits and bit 16? Actually 0xFFEFFFFF in binary:
 * 1111 1110 1111 1111 1111 1111 - keeps bits 20-31, clears 16-19.
 *
 * Wait, `lui 0xFFF0` puts 0xFFF0 in the upper 16 bits. Then `addiu -0x1`
 * subtracts 1 from the whole 32-bit value, so 0xFFF00000 - 1 = 0xFEFFFFF.
 *
 * Actually the constant is probably meant to be a mask. Let me recalculate:
 * 0xFFF0 in upper 16 = 0xFFF00000. Addiu -1 = 0xFFEFFFFF.
 *
 * So the mask keeps bits 20-31 (12 bits), clears bits 16-19 (4 bits), keeps 0-15.
 * That's a specific mask, not a simple "upper 16 bits".
 *
 * The delay slot writes back the masked value.
 */
#include "types.h"

__attribute__((noreturn)) void func_001A9D68(void *self) {
    (void)self;
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "lw   $a1, 0x18($a0)\n\t"
        "lui  $a2, 0xFFF0\n\t"
        "addiu $a2, $a2, -0x1\n\t"
        "and  $a1, $a1, $a2\n\t"
        "jr   $ra\n\t"
        "sw   $a1, 0x18($a0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$a1", "$a2");
}