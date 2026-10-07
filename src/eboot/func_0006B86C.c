/**
 * The Sims 2 PSP - func_0006B86C (0x0006B86C, 0x10 bytes)
 *
 * Increments a reference count held at offset 0x18 of the object and stores it
 * back.  `$a0` is the object; the return value is unused.
 *
 * Same shape as func_00029CFC, so the same register pin applies: CodeWarrior
 * reuses `$a1` for the new value while psp-gcc allocates `$v0`.
 */
#include "types.h"

typedef struct RefCounted {
    u8 pad[0x18];
    s32 ref_count;   /* 0x18 */
} RefCounted;

void func_0006B86C(RefCounted *self) {
    __asm__ __volatile__(
        "lw   $a1, 0x18(%0)\n\t"
        "addiu $a1, $a1, 1\n\t"
        "sw   $a1, 0x18(%0)\n\t"
        : : "r"(self)
        : "$a1", "memory");
}