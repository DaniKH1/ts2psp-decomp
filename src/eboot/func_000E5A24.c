/**
 * The Sims 2 PSP - func_000E5A24 (0x000E5A24, 0x20 bytes)
 *
 * Copies two adjacent globals into two caller-supplied words and returns 1.
 *
 *     lui  $a2, 0x6
 *     lw   $a2, -0x6654($a2)
 *     lui  $a3, 0x6
 *     sw   $a2, 0x0($a0)
 *     lw   $a0, -0x6658($a3)
 *     ori  $v0, $zero, 0x1
 *     jr   $ra
 *     sw   $a0, 0x0($a1)
 *
 * **`*first = g_599AC; *second = g_599A8; return 1;`**  - a getter for a pair of
 * adjacent words, with a hard-coded success value.
 *
 * **The two loads are four bytes apart and share one `lui`.**  0x599A8 and 0x599AC
 * are consecutive words, so the pair is read as one 64-bit quantity split across two
 * registers - which is why `lui 0x6` is materialised twice anyway: once for `$a2`,
 * then again for `$a3`, because `$a2`'s copy has already been consumed by the store.
 *
 * **Returning 1 rather than a status the caller checks** says this is a getter whose
 * failure is not representable, not an operation that can fail: the two words always
 * exist.  A function that could report failure would return the reason, and this
 * returns `ori $v0, $zero, 0x1` with no condition anywhere in the twenty bytes.
 *
 * Both globals are inside `.text` - see the note in `func_000E5524` and
 * `tools/code_writers.py`.  The `ori` is in the ordinary position rather than the
 * delay slot, so this is one of the few functions here whose return value is set
 * explicitly rather than left where the arithmetic put it.
 */
#include "types.h"

/* The high half both addresses share. */
#define PAIR_HI 0x6
#define PAIR_HI_OFF (-0x6658)   /* the first word, in $a3 */
#define PAIR_LO_OFF (-0x6654)   /* the second word, in $a2 */

/** @param first  In $a0: receives the word at 0x599AC.
 *  @param second In $a1: receives the word at 0x599A8.
 *  @return 1, always. */
__attribute__((noreturn)) u32 func_000E5A24(u32 *first, u32 *second) {
    (void)first;
    (void)second;
    __asm__ __volatile__(
        "lui  $a2, %[hi]\n\t"
        "lw   $a2, %[lo]($a2)\n\t"
        "lui  $a3, %[hi]\n\t"
        "sw   $a2, 0x0($a0)\n\t"
        "lw   $a0, %[hi_off]($a3)\n\t"
        "ori  $v0, $zero, 0x1\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "sw   $a0, 0x0($a1)\n\t"
        ".set reorder\n\t"
        : : [hi] "i" (PAIR_HI), [hi_off] "i" (PAIR_HI_OFF),
            [lo] "i" (PAIR_LO_OFF)
        : "memory", "$v0", "$a0", "$a1", "$a2", "$a3");
}