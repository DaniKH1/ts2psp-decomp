/**
 * The Sims 2 PSP - func_000D55B0 (0x000D55B0, 0x1C bytes)
 *
 * Clears five words at offsets 0x0, 0x4, 0x8, 0xC, 0x10, then returns
 * the original pointer.
 *
 *     sw   $zero, 0x0($a0)
 *     sw   $zero, 0x4($a0)
 *     sw   $zero, 0x8($a0)
 *     sw   $zero, 0xC($a0)
 *     sw   $zero, 0x10($a0)
 *     jr   $ra
 *     move $v0, $a0
 *
 * **Five-word clear with self return.**  The move to $v0 is the only
 * instruction after the stores, making this a clear-and-return-self
 * function.
 */
#include "types.h"

typedef struct Record {
    u32 a;   /* 0x00 - cleared */
    u32 b;   /* 0x04 - cleared */
    u32 c;   /* 0x08 - cleared */
    u32 d;   /* 0x0C - cleared */
    u32 e;   /* 0x10 - cleared */
} Record;

__attribute__((noreturn)) Record *func_000D55B0(Record *self) {
    register Record *r asm("$a0") = self;
    __asm__ __volatile__(
        "sw   $zero, 0x0(%[r])\n\t"
        "sw   $zero, 0x4(%[r])\n\t"
        "sw   $zero, 0x8(%[r])\n\t"
        "sw   $zero, 0xC(%[r])\n\t"
        "sw   $zero, 0x10(%[r])\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "move $v0, %[r]\n\t"
        ".set reorder\n\t"
        : : [r] "r"(r)
        : "memory");
}