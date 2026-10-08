/**
 * The Sims 2 PSP - func_000DD674 (0x000DD674, 0x28 bytes)
 *
 * Initialises a structure with a pointer to a global counter, zeroes the first
 * word, then atomically increments that counter and returns the new value.
 *
 *     lui  $a1, 0x1F
 *     addiu $a1, $a1, -0x3540   0x1ECAC0
 *     sw   $a1, 0x4($a0)        store pointer to counter at offset 4
 *     sw   $zero, 0x0($a0)      zero word 0
 *     lui  $a1, 0x1E
 *     lw   $a2, -0x6050($a1)    load counter from 0x1D9FB0
 *     move $v0, $a0             return self (in delay slot of next jr)
 *     addiu $a0, $a2, 0x1       counter + 1
 *     jr   $ra
 *     sw   $a0, -0x6050($a1)    store incremented counter back, in delay slot
 *
 * **This is an atomic-ish increment-and-return**.  The global at 0x1D9FB0 is a
 * counter.  The function reads it, increments it, stores it back, and returns
 * the incremented value.  At the same time it initialises the passed structure
 * with a pointer to that same counter at offset 0x4.
 *
 * The global 0x1ECAC0 stored at 0x04 is a different value - it's the address
 * 0x1ECAC0 itself, not the counter.  The counter is at 0x1D9FB0.
 *
 * The delay slot of the final `jr` does the store-back of the incremented
 * counter.  This is not truly atomic (no LL/SC), but in a single-threaded
 * context or with interrupts disabled it serves as a simple allocator.
 */
#include "types.h"

/* 0x1F0000 - 0x3540 = 0x1ECAC0.  A pointer stored in the structure. */
#define COUNTER_PTR  0x0001ECAC0u

/* 0x1E0000 - 0x6050 = 0x1D9FB0.  The actual counter. */
#define COUNTER_GLOBAL  0x0001D9FB0u

typedef struct Record {
    u32 w0;       /* 0x00 - zeroed */
    u32 *counter; /* 0x04 - points to COUNTER_GLOBAL */
} Record;

__attribute__((noreturn)) u32 func_000DD674(Record *self) {
    register Record *r asm("$a0") = self;
    __asm__ __volatile__(
        "lui  $a1, 0x1F\n\t"
        "addiu $a1, $a1, -0x3540\n\t"
        "sw   $a1, 0x4(%[r])\n\t"
        "sw   $zero, 0x0(%[r])\n\t"
        "lui  $a1, 0x1E\n\t"
        "lw   $a2, -0x6050($a1)\n\t"
        "move $v0, %[r]\n\t"
        "addiu $a0, $a2, 0x1\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "sw   $a0, -0x6050($a1)\n\t"
        ".set reorder\n\t"
        : : [r] "r"(r)
        : "memory", "$a1", "$a2", "$v0");
}