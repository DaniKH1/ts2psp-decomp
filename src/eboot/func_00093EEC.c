/**
 * The Sims 2 PSP - func_00093EEC (0x00093EEC, 0x3C bytes)
 *
 * Copies four words to a fixed address and bumps a sixteen-bit global counter.
 *
 *     lw    $a2, 0x0($a0)
 *     lw    $a3, 0x4($a0)
 *     lw    $a0, 0x8($a0)
 *     lui   $t0, 0xE
 *     lui   $t1, 0x1D
 *     sw    $a2, 0x21C0($t0)
 *     addiu $t2, $t0, 0x21C0
 *     lh    $a2, 0x4D00($t1)
 *     sw    $a3, 0x4($t2)
 *     sw    $a0, 0x8($t2)
 *     addiu $a0, $a2, 0x1
 *     sh    $a0, 0x4D00($t1)
 *     lui   $a0, 0xE
 *     jr    $ra
 *     sw    $a1, 0x21CC($a0)
 *
 * **`*(u32 *)0x0E21C0 = src[0]; 0x0E21C4 = src[1]; 0x0E21C8 = src[2]; 0x0E21CC = arg;
 * *(s16 *)0x01D4D00 += 1;`**
 *
 * **The two addresses are of different kinds, and that is the point of writing both
 * down separately.**  0x01D4D00 is in `.data` and has seven relocations naming it: it
 * is a real global counter.  0x0E21C0 is inside `.text`, 0x50 bytes into
 * `func_000E2170`'s nominal body and before that function's own last `jr $ra`, with
 * five relocations naming it - **the same shape as 0x0E2168, which `func_00102D34`
 * writes seventeen words to, and the two are 88 bytes apart.**
 *
 * So this function writes four words into the code section and increments a data
 * counter in the same breath.  **Whether the 0x0E21C0 store is a runtime patch or an
 * initialisation of a data block the symbol map has no label for is not established**,
 * and the project's existing position on that question applies unchanged: splat's
 * function sizes are "distance to the next label", there is no label at 0x0E21C0, and
 * the same test that admits the three verified clusters in
 * `tools/code_writers.py --real` cannot separate the two readings.
 *
 * **The third word copied comes from the source object and the fourth from the first
 * argument**, so the four words are not contiguous in the source: they are two of
 * `self->word_0`, `self->word_4`, `self->word_8` and one caller's word.  **That is
 * what makes this four words and not three** - `func_00102C84` copies three from one
 * object, and this one fills the fourth from elsewhere.
 *
 * **The counter is `lh` then `sh`, so it is signed and can go negative.**  `lh $a2,
 * 0x4D00($t1)` sign-extends, `addiu $a0, $a2, 1` increments, `sh` truncates back to
 * sixteen bits.  **The signedness is real for the increment and irrelevant for the
 * store** - the arithmetic is the same modulo 2^16 either way - so a signed load is
 * one instruction wider than needed and says only that the compiler read the field as
 * a signed short.  An unsigned `lhu` would have been the same length.
 *
 * **`lui $a0, 0xE` is repeated in the last two instructions**, after `$a0` has been
 * reused for the incremented counter.  The address is rebuilt from scratch instead of
 * kept in `$t0`, because `$t0` has to be free for the seventeen-store copy in
 * `func_00102D34`'s style of budget - here it is simply one `lui` cheaper than
 * shuffling a register.
 */
#include "types.h"

/** Copy three words from `$a0` plus `$a1` to 0x0E21C0, and increment the halfword
 *  counter at 0x01D4D00.
 *  @param src In $a0: three words.
 *  @param arg In $a1: stored as the fourth word. */
__attribute__((noreturn)) void func_00093EEC(void *src, void *arg) {
    (void)src;
    (void)arg;
    __asm__ __volatile__(
        "lw    $a2, 0x0($a0)\n\t"
        "lw    $a3, 0x4($a0)\n\t"
        "lw    $a0, 0x8($a0)\n\t"
        "lui   $t0, 0xE\n\t"
        "lui   $t1, 0x1D\n\t"
        "sw    $a2, 0x21C0($t0)\n\t"
        "addiu $t2, $t0, 0x21C0\n\t"
        "lh    $a2, 0x4D00($t1)\n\t"
        "sw    $a3, 0x4($t2)\n\t"
        "sw    $a0, 0x8($t2)\n\t"
        "addiu $a0, $a2, 0x1\n\t"
        "sh    $a0, 0x4D00($t1)\n\t"
        "lui   $a0, 0xE\n\t"
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "sw    $a1, 0x21CC($a0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$a0", "$a1", "$a2", "$a3",
          "$t0", "$t1", "$t2");
}