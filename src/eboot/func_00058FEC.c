/**
 * The Sims 2 PSP - func_00058FEC (0x00058FEC, 0x0C bytes)
 *
 * Loads a word from offset 0xC, increments it by 1, returns it.
 *
 *     lw      $v0, 0xC($a0)
 *     jr      $ra
 *     addiu   $v0, $v0, 0x1
 *
 * **Load, increment, return.**  The delay slot does the increment.
 */
#include "types.h"

typedef struct Counter {
    u8 pad[0xC];
    u32 value;  /* 0x0C - loaded, incremented, returned */
} Counter;

__attribute__((noreturn)) u32 func_00058FEC(Counter *self) {
    register Counter *c asm("$a0") = self;
    __asm__ __volatile__(
        "lw     $v0, 0xC(%[c])\n\t"
        ".set noreorder\n\t"
        "jr     $ra\n\t"
        "addiu  $v0, $v0, 0x1\n\t"
        ".set reorder\n\t"
        : : [c] "r"(self)
        : "memory", "$v0");
}