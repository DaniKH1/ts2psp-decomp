/**
 * The Sims 2 PSP - func_00054510 (0x00054510, 0x0C bytes)
 *
 * Loads the half-word at offset 0x4, returns it zero-extended to 16 bits.
 *
 *     lw   $v0, 0x4($a0)
 *     jr   $ra
 *     andi $v0, $v0, 0xFFFF
 *
 * **Loads a half-word as a full word.**  The `andi` masks off the upper
 * 16 bits.  The delay slot does the masking.
 */
#include "types.h"

typedef struct Target {
    u32 pad;
    u16 value;  /* 0x04 */
} Target;

__attribute__((noreturn)) u32 func_00054510(Target *self) {
    register Target *t asm("$a0") = self;
    __asm__ __volatile__(
        "lw   $v0, 0x4(%[t])\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "andi $v0, $v0, 0xFFFF\n\t"
        ".set reorder\n\t"
        : : [t] "r"(self)
        : "memory", "$v0");
}