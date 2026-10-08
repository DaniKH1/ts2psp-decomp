/**
 * The Sims 2 PSP - func_0008FDFC (0x0008FDFC, 0x0C bytes)
 *
 * Loads the address of global 0x00C9C8 and returns it.
 *
 *     lui  $v0, %hi(sym_0000C9C8)
 *     jr   $ra
 *     addiu $v0, $v0, %lo(sym_0000C9C8)
 *
 * **Returns a global address.**  0x00C9C8 is the address.
 */
#include "types.h"

__attribute__((noreturn)) void *func_0008FDFC(void) {
    __asm__ __volatile__(
        "lui  $v0, %%hi(sym_0000C9C8)\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "addiu $v0, $v0, %%lo(sym_0000C9C8)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$v0");
}