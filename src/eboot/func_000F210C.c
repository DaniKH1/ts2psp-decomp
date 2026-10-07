/**
 * The Sims 2 PSP - func_000F210C (0x000F210C, 0x14 bytes)
 *
 * Clears the twelve bytes at offset 0 of the object and returns the object.
 *
 * The same shape as func_000A9128, and adjacent to func_000F20F8 in the link
 * order, so it is the same clear reached through a neighbouring type; see
 * func_000A9128 for the pins.
 */
#include "types.h"

typedef struct Clearable {
    u32 a;   /* 0x00 */
    u32 b;   /* 0x04 */
    u32 c;   /* 0x08 */
} Clearable;

Clearable *func_000F210C(Clearable *self) {
    register Clearable *ptr asm("$a0") = self;

    __asm__ __volatile__(
        "sw $zero, 0x0(%0)\n\t"
        "sw $zero, 0x4(%0)\n\t"
        "sw $zero, 0x8(%0)\n\t"
        : [ptr] "+r"(ptr)
        :
        : "memory");
    return ptr;
}