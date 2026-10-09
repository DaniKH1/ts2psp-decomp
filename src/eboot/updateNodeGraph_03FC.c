/**
 * The Sims 2 PSP - updateNodeGraph_03FC (0x1B5C7C, 0x24 bytes)
 *
 *     lw $v0, 0x1C($a0)
 *     beqz $v0, .Leboot_001B5C90
 *     nop
 *     b .Leboot_001B5C98
 *     nop
 *   .Leboot_001B5C90
 *     lui $v0, %%hi(sym_00074208)
 *     addiu $v0, $v0, %%lo(sym_00074208)
 *   .Leboot_001B5C98
 *     jr $ra
 *     nop
 *
 * updateNodeGraph: pull the float at 0x1C out of the caller and hand back a graph pointer.
 */

#include "types.h"

__attribute__((noreturn)) void updateNodeGraph_03FC(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "lw $v0, 0x1C($a0)\n\t"
        "beqz $v0, .Leboot_001B5C90\n\t"
        "nop\n\t"
        "b .Leboot_001B5C98\n\t"
        "nop\n\t"
        ".Leboot_001B5C90:\n\t"
        "lui $v0, %%hi(sym_00074208)\n\t"
        "addiu $v0, $v0, %%lo(sym_00074208)\n\t"
        ".Leboot_001B5C98:\n\t"
        "jr $ra\n\t"
        "nop\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
