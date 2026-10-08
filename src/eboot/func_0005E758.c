/**
 * The Sims 2 PSP - func_0005E758 (0x0005E758, 0x0C bytes)
 *
 * Sets a byte to 1 at offset 0x144 of the argument, returns 1.
 *
 *     ori  $a1, $zero, 0x1
 *     jr   $ra
 *     sb   $a1, 0x144($a0)
 *
 * **Sets a flag byte and returns 1.**  Same pattern as the others,
 * different offset.
 */
#include "types.h"

typedef struct Target {
    u8 pad[0x144];
    u8 flag;  /* 0x144 - set to 1 */
} Target;

__attribute__((noreturn)) u32 func_0005E758(Target *self) {
    register Target *t asm("$a0") = self;
    register u32 v asm("$a1");
    __asm__ __volatile__(
        "ori  %[v], $zero, 0x1\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "sb   %[v], 0x144(%[t])\n\t"
        ".set reorder\n\t"
        : [v] "=&r"(v)
        : [t] "r"(t)
        : "memory");
}