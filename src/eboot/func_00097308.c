/**
 * The Sims 2 PSP - func_00097308 (0x00097308, 0x10 bytes)
 *
 * Stores 0.0f to offset 0x24 of the argument, returns 0.
 *
 *     mtc1 $zero, $f12
 *     move $v0, $zero
 *     jr   $ra
 *     swc1 $f12, 0x24($a0)
 *
 * **Stores 0.0f to offset 0x24, returns 0.**
 * The delay slot holds the float store.
 */
#include "types.h"

typedef struct Target {
    u8 pad[0x24];
    float value;  /* 0x24 - set to 0.0f */
} Target;

__attribute__((noreturn)) u32 func_00097308(Target *self) {
    register Target *t asm("$a0") = self;
    __asm__ __volatile__(
        "mtc1 $zero, $f12\n\t"
        "move $v0, $zero\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "swc1 $f12, 0x24(%[t])\n\t"
        ".set reorder\n\t"
        : : [t] "r"(self)
        : "memory", "$f12", "$v0");
}