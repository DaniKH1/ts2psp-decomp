/**
 * The Sims 2 PSP - func_000A9128 (0x000A9128, 0x14 bytes)
 *
 * Clears the twelve bytes at offset 0 of the object and returns the object, so
 * callers can write `obj = func_000A9128(obj)`.
 *
 * Three `sw $zero` rather than one `memset`: the compiler unrolls a constant
 * small size into individual stores.
 *
 * As in func_00052604 the pointer is both argument and return, and the copy
 * back into `$v0` is only free if the value in `$a0` already counts as the
 * result - so the pointer is a `register` variable pinned to `$a0` and the asm's
 * output as well as its input.  The copy then lands in the return's delay slot
 * rather than being hoisted above the stores.
 */
#include "types.h"

typedef struct Clearable {
    u32 a;   /* 0x00 */
    u32 b;   /* 0x04 */
    u32 c;   /* 0x08 */
} Clearable;

Clearable *func_000A9128(Clearable *self) {
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