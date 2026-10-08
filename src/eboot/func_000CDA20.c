/**
 * The Sims 2 PSP - func_000CDA20 (0x000CDA20, 0x10 bytes)
 *
 * Clears three words at offsets 0x8, 0xC, and 0x10 of the argument.
 * The first two stores come before the return, the third is in the delay slot.
 *
 *     sw   $zero, 0x8($a0)
 *     sw   $zero, 0xC($a0)
 *     jr   $ra
 *     sw   $zero, 0x10($a0)    delay slot
 */
#include "types.h"

typedef struct Record {
    u32 pad[2];   /* 0x00 .. 0x07 */
    u32 a;        /* 0x08 - cleared */
    u32 b;        /* 0x0C - cleared */
    u32 c;        /* 0x10 - cleared */
} Record;

__attribute__((noreturn)) void func_000CDA20(Record *self) {
    register Record *r asm("$a0") = self;
    __asm__ __volatile__(
        "sw   $zero, 0x8(%[r])\n\t"
        "sw   $zero, 0xC(%[r])\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "sw   $zero, 0x10(%[r])\n\t"
        ".set reorder\n\t"
        : : [r] "r"(r)
        : "memory");
}