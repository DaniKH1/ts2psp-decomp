/**
 * The Sims 2 PSP - func_00004FDC (0x00004FDC, 0x0C bytes)
 *
 * Stores 0.0f to offset 0x5864 of the argument, returns the argument.
 *
 *     mtc1 $zero, $f12
 *     jr   $ra
 *     swc1 $f12, 0x5864($a0)
 *
 * **Writes 0.0f to a large offset.**  The delay slot holds the store.
 * Returns the original argument pointer.
 */
#include "types.h"

typedef struct Target {
    u8 pad[0x5864];
    float value;  /* 0x5864 - set to 0.0f */
} Target;

__attribute__((noreturn)) Target *func_00004FDC(Target *self) {
    register Target *t asm("$a0") = self;
    __asm__ __volatile__(
        "mtc1 $zero, $f12\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "swc1 $f12, 0x5864(%[t])\n\t"
        ".set reorder\n\t"
        : : [t] "r"(t)
        : "memory", "$f12");
}