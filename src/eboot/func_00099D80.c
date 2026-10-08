/**
 * The Sims 2 PSP - func_00099D80 (0x00099D80, 0x14 bytes)
 *
 * Stores the third and second arguments to consecutive globals in the 0x1 page.
 *
 *     lui  $a0, 0x1
 *     sw   $a2, -0x3608($a0)   0x10000 - 0x3608 = 0xC9F8
 *     lui  $a0, 0x1
 *     jr   $ra
 *     sw   $a1, -0x3604($a0)   0x10000 - 0x3604 = 0xC9FC
 *
 * **Two consecutive words stored**: `$a2` (3rd arg) to 0xC9F8, `$a1` (2nd arg)
 * to 0xC9FC.  The first argument `$a0` is unused.
 *
 * **The same `lui $a0, 0x1` appears twice** - the compiler didn't reuse the
 * value from the first.  This is a small missed optimisation that the
 * transcription must preserve.
 *
 * Returns void, no return value used.
 */
#include "types.h"

/* 0x10000 - 0x3608 = 0xC9F8.  Low global reached via lui 0x1 + negative offset. */
#define GLOBAL_W2  0x0000C9F8u

/* 0x10000 - 0x3604 = 0xC9FC. */
#define GLOBAL_W1  0x0000C9FCu

__attribute__((noreturn)) void func_00099D80(u32 unused, u32 w1, u32 w2) {
    (void)unused; (void)w1; (void)w2;
    __asm__ __volatile__(
        "lui  $a0, 0x1\n\t"
        "sw   $a2, -0x3608($a0)\n\t"
        "lui  $a0, 0x1\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "sw   $a1, -0x3604($a0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$a0", "$a1", "$a2");
}