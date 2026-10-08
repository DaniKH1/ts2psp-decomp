/**
 * The Sims 2 PSP - func_001102F0 (0x001102F0, 0x24 bytes)
 *
 * Loads a pointer from offset 0x8, tests if the second argument is negative
 * (via sltu $zero, $a1), stores the boolean result at offset 0x4 of the
 * pointer, stores 1 at offset 0, then advances the pointer by 8 and stores
 * it back at offset 0x8.
 *
 *     lw   $a2, 0x8($a0)
 *     sltu $a1, $zero, $a1     a1 = (a1 < 0) ? 1 : 0
 *     ori  $a3, $zero, 0x1     a3 = 1
 *     sw   $a1, 0x4($a2)       store sign flag at 0x4
 *     sw   $a3, 0x0($a2)       store 1 at 0x0
 *     lw   $a1, 0x8($a0)
 *     addiu $a1, $a1, 0x8      advance by 8
 *     jr   $ra
 *     sw   $a1, 0x8($a0)       store advanced pointer back, in delay slot
 *
 * **This looks like a stream/buffer advance function**.  The structure at
 * offset 0x8 is a pointer to a record.  The function writes a sign flag
 * (was the input negative?) at 0x4, a constant 1 at 0x0, then advances
 * the pointer by 8 bytes (two words) and stores it back.
 *
 * Returns void (no explicit return value set).
 */
#include "types.h"

typedef struct Record {
    u32 field0;   /* 0x00 - set to 1 */
    u32 sign;     /* 0x04 - set to 1 if input was negative, 0 otherwise */
} Record;

typedef struct Context {
    u32 pad[2];   /* 0x00 .. 0x07 */
    Record *rec;  /* 0x08 - pointer to record */
} Context;

__attribute__((noreturn)) void func_001102F0(Context *ctx, s32 value) {
    register Context *c asm("$a0") = ctx;
    register s32 v asm("$a1") = value;
    (void)c; (void)v;
    __asm__ __volatile__(
        "lw   $a2, 0x8($a0)\n\t"
        "sltu $a1, $zero, $a1\n\t"
        "ori  $a3, $zero, 0x1\n\t"
        "sw   $a1, 0x4($a2)\n\t"
        "sw   $a3, 0x0($a2)\n\t"
        "lw   $a1, 0x8($a0)\n\t"
        "addiu $a1, $a1, 0x8\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "sw   $a1, 0x8($a0)\n\t"
        ".set reorder\n\t"
        :
        : "r"(c), "r"(v)
        : "memory", "$a2", "$a3");
}