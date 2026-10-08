/**
 * The Sims 2 PSP - func_000A8174 (0x000A8174, 0x0C bytes)
 *
 * Clears a byte at offset 0x6D of the third argument, clears
 * a byte at offset 0x6C of the third argument, returns void.
 *
 *     sb   $zero, 0x6D($a2)
 *     jr   $ra
 *     sb   $zero, 0x6C($a2)
 *
 * **Clears two bytes at offsets 0x6D and 0x6C of the third argument.**
 * The delay slot does the second byte clear.
 */
#include "types.h"

__attribute__((noreturn)) void func_000A8174(void *a0, void *a1, void *a2) {
    (void)a0; (void)a1; (void)a2;
    __asm__ __volatile__(
        "sb   $zero, 0x6D($a2)\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "sb   $zero, 0x6C($a2)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$a2");
}