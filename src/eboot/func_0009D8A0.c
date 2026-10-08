/**
 * The Sims 2 PSP - func_0009D8A0 (0x0009D8A0, 0x0C bytes)
 *
 * Stores the second argument at offset 0x48, stores a byte from the
 * third argument at offset 0x204, returns void.
 *
 *     sw   $a1, 0x48($a0)
 *     jr   $ra
 *     sb   $a2, 0x204($a0)
 *
 * **Stores a word and a byte.**  The word at 0x48 is the second
 * argument, the byte at 0x204 is from the third argument's low byte.
 */
#include "types.h"

typedef struct Target {
    u8 pad[0x48];
    u32 field1;  /* 0x48 - set to $a1 */
    u8 pad2[0x1BC];
    u8 field2;   /* 0x204 - set to low byte of $a2 */
} Target;

__attribute__((noreturn)) void func_0009D8A0(void *self, u32 w, u32 b) {
    register void *p asm("$a0") = self;
    register u32 wval asm("$a1");
    register u32 bval asm("$a2");
    __asm__ __volatile__(
        "sw   %[w], 0x48(%[p])\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "sb   %[b], 0x204(%[p])\n\t"
        ".set reorder\n\t"
        : [p] "+r"(p)
        : [w] "r"(wval), [b] "r"(bval)
        : "memory");
}