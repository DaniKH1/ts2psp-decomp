/**
 * The Sims 2 PSP - func_000F92A0 (0x000F92A0, 0x50 bytes)
 *
 * Reads a word from a 28-byte-stride array element, ORs it with 0x8, writes
 * it back, and writes two other computed words to the same element.
 *
 *     sll  $a3, $a0, 5          index * 32
 *     sll  $a0, $a0, 2          index * 4
 *     subu $a0, $a3, $a0        index * 28 (32 - 4)
 *     lui  $a3, 0xF
 *     addiu $a3, $a3, -0x47B0   base address = 0xEB850
 *     addu $a0, $a0, $a3        address of element
 *     lw   $a3, 0x0($a0)        load word 0
 *     sra  $t0, $a1, 2          arg1 >> 2
 *     ori  $a3, $a3, 0x8        word0 |= 0x8
 *     srl  $t0, $t0, 30         top 2 bits of (arg1 >> 2)
 *     sw   $a3, 0x0($a0)        write back word0
 *     addu $a1, $a1, $t0        arg1 += top 2 bits
 *     sra  $a3, $a2, 2          arg2 >> 2
 *     sra  $a1, $a1, 2          (arg1 + top2) >> 2
 *     srl  $a3, $a3, 30         top 2 bits of (arg2 >> 2)
 *     sw   $a1, 0x8($a0)        word2 = adjusted arg1
 *     addu $a1, $a2, $a3        arg2 + top2
 *     sra  $a1, $a1, 2          (arg2 + top2) >> 2
 *     jr   $ra
 *     sw   $a1, 0xC($a0)        word3 = adjusted arg2, in delay slot
 *
 * **This is the same 28-byte stride as `func_000F9414` (base 0xEB850)** but
 * instead of just writing a constant, it reads the first word, sets bit 3
 * (0x8), and writes two adjusted arguments to offsets 0x8 and 0xC.
 *
 * **The adjustment is a divide-by-4 with rounding.**  `sra $reg, 2` is arithmetic
 * right shift by 2 (divide by 4, rounding toward negative infinity).  The
 * `srl $tmp, $reg, 30` extracts the two bits that were shifted out, and adding
 * them back implements round-to-nearest for positive numbers - if the two low
 * bits were 0b11 (3), adding 1 rounds up.  For negative numbers it's more
 * complex (rounds toward zero rather than negative infinity), but the pattern
 * is consistent: divide by 4 with some rounding.
 *
 * **Three arguments**: index in `$a0`, value1 in `$a1`, value2 in `$a2`.
 * Returns void, writes three words to the element.
 */
#include "types.h"

/* 0xF0000 - 0x47B0 = 0xEB850.  Same base as func_000F9414. */
#define BASE     0x000EB850u
#define STRIDE   28u

typedef struct Element {
    u32 word0;   /* 0x00 - read, OR 0x8, write back */
    u32 word1;   /* 0x04 - untouched */
    u32 word2;   /* 0x08 - set to adjusted arg1 */
    u32 word3;   /* 0x0C - set to adjusted arg2 */
    u32 rest[4]; /* 0x10 .. 0x1B */
} Element;

__attribute__((noreturn)) void func_000F92A0(void) {
    /* The caller passes arguments in $a0 (base), $a1 (arg1), $a2 (arg2).
     * The asm block reads them directly, so no C parameters. */
    __asm__ __volatile__(
        "sll  $a3, $a0, 5\n\t"
        "sll  $a0, $a0, 2\n\t"
        "subu $a0, $a3, $a0\n\t"
        "lui  $a3, 0xF\n\t"
        "addiu $a3, $a3, -0x47B0\n\t"
        "addu $a0, $a0, $a3\n\t"
        "lw   $a3, 0x0($a0)\n\t"
        "sra  $t0, $a1, 2\n\t"
        "ori  $a3, $a3, 0x8\n\t"
        "srl  $t0, $t0, 30\n\t"
        "sw   $a3, 0x0($a0)\n\t"
        "addu $a1, $a1, $t0\n\t"
        "sra  $a3, $a2, 2\n\t"
        "sra  $a1, $a1, 2\n\t"
        "srl  $a3, $a3, 30\n\t"
        "sw   $a1, 0x8($a0)\n\t"
        "addu $a1, $a2, $a3\n\t"
        "sra  $a1, $a1, 2\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "sw   $a1, 0xC($a0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$a0", "$a1", "$a2", "$a3", "$t0");
}