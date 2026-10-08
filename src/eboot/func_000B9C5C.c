/**
 * The Sims 2 PSP - func_000B9C5C (0x000B9C5C, 0x2C bytes)
 *
 * Initialises a 6-word structure with fixed values and returns it.
 *
 *     lui  $a1, 0x1F
 *     addiu $a1, $a1, -0x6918   0x1E96E8
 *     sw   $a1, 0x18($a0)
 *     sw   $zero, 0x0($a0)
 *     addiu $a1, $zero, -0x1    -1
 *     sw   $zero, 0x8($a0)
 *     sw   $a1, 0xC($a0)
 *     sw   $zero, 0x10($a0)
 *     sw   $zero, 0x14($a0)
 *     jr   $ra
 *     move $v0, $a0             return self
 *
 * **Six words written**: 0x1E96E8 at 0x18, -1 at 0xC, zeros at 0x0, 0x8, 0x10, 0x14.
 * The structure layout:
 *   0x00: 0
 *   0x04: (untouched)
 *   0x08: 0
 *   0x0C: -1
 *   0x10: 0
 *   0x14: 0
 *   0x18: 0x1E96E8
 *
 * The value 0x1E96E8 is in the `0x1E` page, likely a pointer or handle.
 * The -1 at 0x0C is the same sentinel used elsewhere for "unlinked" or
 * "not initialised".
 *
 * Returns the structure pointer, so this is a constructor pattern.
 */
#include "types.h"

#define MAGIC_VALUE  0x0001E96E8u

typedef struct Record {
    u32 w0;      /* 0x00 - 0 */
    u32 w1;      /* 0x04 - untouched */
    u32 w2;      /* 0x08 - 0 */
    s32 state;   /* 0x0C - -1 */
    u32 w4;      /* 0x10 - 0 */
    u32 w5;      /* 0x14 - 0 */
    u32 magic;   /* 0x18 - 0x1E96E8 */
} Record;

__attribute__((noreturn)) Record *func_000B9C5C(Record *self) {
    register Record *r asm("$a0") = self;
    __asm__ __volatile__(
        "lui  $a1, 0x1F\n\t"
        "addiu $a1, $a1, -0x6918\n\t"
        "sw   $a1, 0x18(%[r])\n\t"
        "sw   $zero, 0x0(%[r])\n\t"
        "addiu $a1, $zero, -0x1\n\t"
        "sw   $zero, 0x8(%[r])\n\t"
        "sw   $a1, 0xC(%[r])\n\t"
        "sw   $zero, 0x10(%[r])\n\t"
        "sw   $zero, 0x14(%[r])\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "move $v0, %[r]\n\t"
        ".set reorder\n\t"
        : : [r] "r"(r)
        : "memory", "$a1", "$v0");
}