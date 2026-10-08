/**
 * The Sims 2 PSP - func_0008ED10 (0x0008ED10, 0x0C bytes)
 *
 * Stores the second argument at offset 0x48, clears offset 0x4C,
 * returns void.
 *
 *     sw   $a1, 0x48($a0)
 *     jr   $ra
 *     sw   $zero, 0x4C($a0)
 *
 * **Stores second argument at 0x48, clears 0x4C.**
 * The delay slot does the second store.
 */
#include "types.h"

typedef struct Record {
    u8 pad[0x48];
    u32 field1;  /* 0x48 - set to a1 */
    u32 field2;  /* 0x4C - cleared to 0 */
} Record;

__attribute__((noreturn)) void func_0008ED10(Record *self, u32 value) {
    register Record *r asm("$a0") = self;
    register u32 v asm("$a1") = value;
    (void)r; (void)v;
    __asm__ __volatile__(
        "sw   %[v], 0x48(%[r])\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "sw   $zero, 0x4C(%[r])\n\t"
        ".set reorder\n\t"
        : : [r] "r"(self), [v] "r"(value)
        : "memory");
}