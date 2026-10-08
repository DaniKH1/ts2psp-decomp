/**
 * The Sims 2 PSP - func_001A9C6C (0x001A9C6C, 0x2C bytes)
 *
 * Copies three words from the structure pointed to by the second argument
 * to offset 0x48 of the first argument, then sets bit 0x100 in the word
 * at offset 0x18 of the first argument.
 *
 *     lw   $a2, 0x0($a1)        load word 0 from *a1
 *     lw   $t0, 0x4($a1)        load word 1 from *a1
 *     addiu $a3, $a0, 0x48      dest = a0 + 0x48
 *     lw   $a1, 0x8($a1)        load word 2 from *a1
 *     sw   $a2, 0x0($a3)        store word 0 to dest
 *     sw   $t0, 0x4($a3)        store word 1 to dest
 *     sw   $a1, 0x8($a3)        store word 2 to dest
 *     lw   $a1, 0x18($a0)       load flags word from a0+0x18
 *     ori  $a1, $a1, 0x100      set bit 0x100
 *     jr   $ra
 *     sw   $a1, 0x18($a0)       store back flags, in delay slot
 *
 * **Copies three consecutive words** from one structure to another, then
 * sets a flag bit (0x100 = 256) in a flags word at offset 0x18.
 *
 * The destination is at a0 + 0x48, which is 18 words into the first
 * structure. The source is the second argument's first three words.
 */
#include "types.h"

typedef struct Source {
    u32 w0;   /* 0x00 */
    u32 w1;   /* 0x04 */
    u32 w2;   /* 0x08 */
} Source;

typedef struct Dest {
    u32 pad[18];   /* 0x00 .. 0x47 */
    u32 w0;        /* 0x48 */
    u32 w1;        /* 0x4C */
    u32 w2;        /* 0x50 */
    u32 pad2[6];   /* 0x54 .. 0x6B */
    u32 flags;     /* 0x6C (0x18 from start of flags section?) wait */
} Dest;

/* Actually 0x18 from a0, not from 0x48. So flags at 0x18. */

typedef struct Dest2 {
    u32 pad[6];    /* 0x00 .. 0x17 */
    u32 flags;     /* 0x18 */
    u32 pad2[12];  /* 0x1C .. 0x47 */
    u32 w0;        /* 0x48 */
    u32 w1;        /* 0x4C */
    u32 w2;        /* 0x50 */
} Dest2;

__attribute__((noreturn)) void func_001A9C6C(Dest2 *dest, Source *src) {
    register Dest2 *d asm("$a0") = dest;
    register Source *s asm("$a1") = src;
    __asm__ __volatile__(
        "lw   $a2, 0x0(%[s])\n\t"
        "lw   $t0, 0x4(%[s])\n\t"
        "addiu $a3, %[d], 0x48\n\t"
        "lw   %[s], 0x8(%[s])\n\t"
        "sw   $a2, 0x0($a3)\n\t"
        "sw   $t0, 0x4($a3)\n\t"
        "sw   %[s], 0x8($a3)\n\t"
        "lw   $a1, 0x18(%[d])\n\t"
        "ori  $a1, $a1, 0x100\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "sw   $a1, 0x18(%[d])\n\t"
        ".set reorder\n\t"
        : [d] "+r"(d), [s] "+r"(s)
        :
        : "memory", "$a2", "$a3", "$t0");
}