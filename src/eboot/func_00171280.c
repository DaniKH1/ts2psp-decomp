/**
 * The Sims 2 PSP - func_00171280 (0x171280, 0x8 bytes)
 *
 *     jr $ra
 *     ori $v0, $zero, 0x1
 *
 * renderMeshInstances: one phase of the mesh-instance renderer.
 */

#include "types.h"

__attribute__((noreturn)) void func_00171280(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "jr $ra\n\t"
        "ori $v0, $zero, 0x1\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
