/**
 * The Sims 2 PSP - func_00154938 (0x00154938, 0x0C bytes)
 *
 * Loads the address of string "gameObjectBehavior" and returns it.
 *
 *     lui  $v0, %hi(str_gameObjectBehavior)
 *     jr   $ra
 *     addiu $v0, $v0, %lo(str_gameObjectBehavior)
 *
 * **Returns a string address.**
 */
#include "types.h"

__attribute__((noreturn)) char *func_00154938(void) {
    __asm__ __volatile__(
        "lui  $v0, %%hi(str_gameObjectBehavior)\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "addiu $v0, $v0, %%lo(str_gameObjectBehavior)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$v0");
}