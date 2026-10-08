/**
 * The Sims 2 PSP - func_0001FA3C (0x0001FA3C, 0x10 bytes)
 *
 * Stores a zero byte at offset 0x57FC, stores 0.0f at offset 0x57F4,
 * returns the argument pointer.
 *
 *     sb   $zero, 0x57FC($a0)
 *     mtc1 $zero, $f12
 *     jr   $ra
 *     swc1 $f12, 0x57F4($a0)
 *
 * **Two stores in four instructions.**  The byte store is separate from
 * the float store, suggesting they're different fields in a structure.
 * The delay slot holds the float store.
 *
 * Returns the original pointer.
 */
#include "types.h"

typedef struct Target {
    u8 pad[0x57FC];
    u8 flag;   /* 0x57FC - cleared to 0 */
    float value; /* 0x57F4 - set to 0.0f */
} Target;

__attribute__((noreturn)) Target *func_0001FA3C(Target *self) {
    register Target *t asm("$a0") = self;
    __asm__ __volatile__(
        "sb   $zero, 0x57FC(%[t])\n\t"
        "mtc1 $zero, $f12\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "swc1 $f12, 0x57F4(%[t])\n\t"
        ".set reorder\n\t"
        : : [t] "r"(self)
        : "memory", "$f12");
}