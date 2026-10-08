/**
 * The Sims 2 PSP - func_000F8140 (0x000F8140, 0x18 bytes)
 *
 * Stores four registers to a structure and returns 0.
 *
 *     sw   $a1, 0x0($a0)
 *     sw   $a3, 0x8($a0)
 *     sw   $a2, 0x4($a0)
 *     sw   $t0, 0xC($a0)
 *     jr   $ra
 *     move $v0, $zero
 *
 * **Four-word store to a structure.**  The order is 0x0, 0x8, 0x4, 0xC -
 * not sequential, which suggests the original source had the fields in a
 * different order or the compiler reordered them.
 *
 * Returns 0.
 */
#include "types.h"

typedef struct Record {
    u32 w0;   /* 0x00 - from $a1 */
    u32 w1;   /* 0x04 - from $a2 */
    u32 w2;   /* 0x08 - from $a3 */
    u32 w3;   /* 0x0C - from $t0 */
} Record;

__attribute__((noreturn)) u32 func_000F8140(Record *self, u32 a1, u32 a2, u32 a3, u32 t0) {
    register Record *r asm("$a0") = self;
    (void)a1; (void)a2; (void)a3; (void)t0;
    __asm__ __volatile__(
        "sw   $a1, 0x0(%[r])\n\t"
        "sw   $a3, 0x8(%[r])\n\t"
        "sw   $a2, 0x4(%[r])\n\t"
        "sw   $t0, 0xC(%[r])\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "move $v0, $zero\n\t"
        ".set reorder\n\t"
        : : [r] "r"(r)
        : "memory");
}