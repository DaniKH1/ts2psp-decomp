/**
 * The Sims 2 PSP - func_0012E9D4 (0x0012E9D4, 0x14 bytes)
 *
 * Sets a word to 1, then a byte to 1, in the delay slot.
 *
 *     ori  $a1, $zero, 0x1
 *     sw   $a1, 0x9C($a0)
 *     ori  $a1, $zero, 0x1
 *     jr   $ra
 *     sb   $a1, 0x98($a0)
 *
 * **Two fields, both set to 1, at different widths.**  The word at 0x9C and the
 * byte at 0x98 are both set to 1.  The byte is written in the delay slot, which
 * means the word store is the last regular instruction.
 *
 * **`ori $a1, $zero, 0x1` appears twice** - once before the word store and once
 * before the return.  The second one is strictly redundant since `$a1` still holds
 * 1, but it is what the compiler emitted.  Keeping it in the asm preserves the
 * exact instruction sequence; removing it would save an instruction and break
 * byte-match.
 *
 * The function takes one argument (the buffer) and ignores any return value.
 */
#include "types.h"

typedef struct Target {
    u8  pad_98[0x98];
    u8  byte_98;   /* 0x98 - set to 1, in delay slot */
    u8  pad_99[3];
    u32 word_9c;   /* 0x9C - set to 1 */
} Target;

__attribute__((noreturn)) void func_0012E9D4(Target *self) {
    register Target *t asm("$a0") = self;

    __asm__ __volatile__(
        "ori  $a1, $zero, 0x1\n\t"
        "sw   $a1, 0x9C(%[t])\n\t"
        "ori  $a1, $zero, 0x1\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "sb   $a1, 0x98(%[t])\n\t"
        ".set reorder\n\t"
        : : [t] "r"(t)
        : "memory", "$a1");
}