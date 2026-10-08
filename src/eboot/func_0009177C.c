/**
 * The Sims 2 PSP - func_0009177C (0x0009177C, 0x0C bytes)
 *
 * Loads a word from offset 0xC, stores a float from $f12 to 0xAC
 * of that word, returns void.
 *
 *     lw   $a0, 0xC($a0)
 *     jr   $ra
 *     swc1 $f12, 0xAC($a0)
 *
 * **Loads a pointer from offset 0xC, stores float to 0xAC of it.**
 * The delay slot does the float store.
 */
#include "types.h"

__attribute__((noreturn)) void func_0009177C(void *self, float f) {
    (void)self; (void)f;
    __asm__ __volatile__(
        "lw   $a0, 0xC($a0)\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "swc1 $f12, 0xAC($a0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$f12", "$a0");
}