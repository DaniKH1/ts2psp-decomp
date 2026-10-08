/**
 * The Sims 2 PSP - func_0016789C (0x0016789C, 0x0C bytes)
 *
 * Loads the address of string "Biped_Translator" and returns it.
 *
 *     lui  $v0, %hi(str_Biped_Translator)
 *     jr   $ra
 *     addiu $v0, $v0, %lo(str_Biped_Translator)
 *
 * **Returns a string address.**
 */
#include "types.h"

__attribute__((noreturn)) char *func_0016789C(void) {
    __asm__ __volatile__(
        "lui  $v0, %%hi(str_Biped_Translator)\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "addiu $v0, $v0, %%lo(str_Biped_Translator)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$v0");
}