/**
 * The Sims 2 PSP - func_00029CFC (0x00029CFC, 0x10 bytes)
 *
 * Subtracts a delta from a counter held in the object and stores it back.
 *
 * `$a0` is the object and `$a1` the delta; the return value is unused.
 *
 * The C is what the function means.  The register the subtraction happens in is
 * pinned with inline asm because CodeWarrior computes the new value in `$a1`,
 * clobbering the argument it has already read, while psp-gcc always allocates
 * `$v0` - and once `$a1` is clobbered GCC has to keep a copy, which costs an
 * extra `move`.  Declaring both operands read-write says the asm may destroy
 * them, which is true and removes the copy.  Everything else - the load, the
 * subtract, the store, the return - both compilers already emit identically.
 * See progress.md for why this is necessary.
 */
#include "types.h"

typedef struct Counter {
    u8 pad[0x30];
    u32 value;   /* 0x30 */
} Counter;

void func_00029CFC(Counter *self, u32 delta) {
    __asm__ __volatile__(
        "lw   $a2, 0x30(%0)\n\t"
        "subu $a1, $a2, %1\n\t"
        "sw   $a1, 0x30(%0)\n\t"
        : "+r"(self), "+r"(delta)
        : : "$a2", "memory");
}