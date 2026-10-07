/**
 * The Sims 2 PSP - func_001029C8 (0x001029C8, 0x18 bytes)
 *
 * Stores three words to a 64-byte-stride array element.
 *
 *     sll  $a1, $a1, 6          index * 64
 *     addu $a0, $a0, $a1        base + index*64
 *     sw   $a3, 0x0($a0)        word 0
 *     sw   $t0, 0x4($a0)        word 1
 *     jr   $ra
 *     sw   $a2, 0x8($a0)        word 2, in delay slot
 *
 * **Three words, consecutive, 64-byte stride.**  The element size is 64 bytes (16
 * words), and this writes the first three words of the element.  The other 13 words
 * are left untouched - this is a partial initialiser or an update of a subset.
 *
 * **The three source registers are `$a3`, `$t0`, and `$a2`.**  `$a3` and `$a2` are
 * integer arguments (4th and 3rd); `$t0` is a caller-saved temporary.  The fact that
 * `$t0` is used suggests the caller had the value in a temporary, or the compiler
 * picked it because `$a3` and `$a2` were the only argument registers left.
 *
 * **`$a0` is both the base pointer and the computed address.**  The `addu` overwrites
 * the base with the target address, so the function cannot be a pure offset
 * calculation - it mutates the argument register.  The C transcription needs
 * `noreorder` and an explicit return in asm so the delay slot lands correctly.
 */
#include "types.h"

/* 64-byte element, 16 words; this writes words 0, 1, 2. */
#define STRIDE 64

typedef struct Element {
    u32 word0;   /* 0x00 */
    u32 word1;   /* 0x04 */
    u32 word2;   /* 0x08 */
    u32 rest[13]; /* 0x0C .. 0x3C */
} Element;

__attribute__((noreturn)) void func_001029C8(void) {
    /* The original uses a non-standard calling convention: 5 arguments in
     * $a0, $a1, $a2, $a3, $t0, with the 5th passed in the caller's jal delay
     * slot.  No C call sites exist (only assembly), so a void signature is
     * correct for the C build - the asm block reads the registers directly. */
    __asm__ __volatile__(
        "sll  $a1, $a1, 6\n\t"
        "addu $a0, $a0, $a1\n\t"
        "sw   $a3, 0x0($a0)\n\t"
        "sw   $t0, 0x4($a0)\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "sw   $a2, 0x8($a0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$a0", "$a1", "$a2", "$a3", "$t0");
}