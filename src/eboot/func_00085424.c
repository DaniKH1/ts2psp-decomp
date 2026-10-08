/**
 * The Sims 2 PSP - func_00085424 (0x00085424, 0x0C bytes)
 *
 * Loads the address of string "default_category" and returns it.
 *
 *     lui  $v0, %hi(str_default_category)
 *     jr   $ra
 *     addiu $v0, $v0, %lo(str_default_category)
 *
 * **Returns a string address.**  The string is in the module's data
 * section.
 */
#include "types.h"

__attribute__((noreturn)) char *func_00085424(void) {
    __asm__ __volatile__(
        "lui  $v0, %%hi(str_default_category)\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "addiu $v0, $v0, %%lo(str_default_category)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$v0");
}