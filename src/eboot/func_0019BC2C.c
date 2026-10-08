/**
 * The Sims 2 PSP - func_0019BC2C (0x0019BC2C, 0x14 bytes)
 *
 * Stores the argument pointer to offsets 0x0 and 0x4, clears offset 0xC,
 * then returns the argument.
 *
 *     sw   $a0, 0x0($a0)
 *     sw   $a0, 0x4($a0)
 *     sw   $zero, 0xC($a0)
 *     jr   $ra
 *     move $v0, $a0
 *
 * **Two self-references and a clear.**  Similar to func_00194F94 but
 * with an additional zeroing at offset 0xC.
 */
#include "types.h"

typedef struct Node {
    struct Node *self0;  /* 0x00 - set to self */
    struct Node *self1;  /* 0x04 - set to self */
    u32 pad;             /* 0x08 */
    u32 cleared;         /* 0x0C - set to 0 */
} Node;

__attribute__((noreturn)) Node *func_0019BC2C(Node *self) {
    register Node *n asm("$a0") = self;
    __asm__ __volatile__(
        "sw   %[n], 0x0(%[n])\n\t"
        "sw   %[n], 0x4(%[n])\n\t"
        "sw   $zero, 0xC(%[n])\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "move $v0, %[n]\n\t"
        ".set reorder\n\t"
        : : [n] "r"(n)
        : "memory");
}