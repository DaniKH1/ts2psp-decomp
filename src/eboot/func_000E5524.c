/**
 * The Sims 2 PSP - func_000E5524 (0x000E5524, 0x20 bytes)
 *
 * Flips a one-bit global and remembers what it was.
 *
 *     lui  $a0, 0x5
 *     lw   $a1, 0x196C($a0)
 *     lui  $a2, 0x5
 *     addiu $a3, $a1, 0x1
 *     sw   $a1, 0x1968($a2)
 *     andi $a1, $a3, 0x1
 *     jr   $ra
 *     sw   $a1, 0x196C($a0)
 *
 * **`previous = flag; old = previous; flag = (previous + 1) & 1;`**  over the two
 * adjacent words at 0x51968 and 0x5196C.
 *
 * **The return value is whatever `$v0` already held.**  Nothing in the twenty bytes
 * writes it, so this is a `void` function and declaring it anything else would be a
 * guess.  That it flips *and records* is what makes it a `void`: the point of the
 * pair is that the old value is available afterwards, from 0x51968, and a caller
 * wanting the new value can read 0x5196C just as well.
 *
 * `(previous + 1) & 1` rather than `previous ^ 1`: increment-then-mask and xor are
 * the same for one-bit values, and the compiler chose the first, which suggests the
 * source wrote the flag as a counter or as a `++` on a small integer rather than as
 * a `bool`.  The `addiu` is on `$a3`, a scratch register, so `$a1` stays live for
 * the `sw` at 0x51968 that has to happen *before* the flip is written back.
 *
 * **Both addresses are reached through two separate `lui 0x5`s.**  The word at 0x51968
 * is stored through `$a2` and the word at 0x5196C through `$a0`, so the same high
 * half is materialised twice; one register would do the job and cost a `mov`
 * instead, which is the trade the register allocator makes throughout this tree.
 *
 * **0x51968 is inside `.text`.**  That is not a mistake in the transcription and it
 * is not unique: the module keeps writable data in the code section, so the symbol
 * map's function boundaries - which splat derives from "distance to the next label" -
 * attribute data blocks to the function that precedes them.  See
 * `tools/code_writers.py`, which measures how many addresses this affects and which
 * of them were checked by hand.
 */
#include "types.h"

/** The high half both addresses share. */
#define GLOBALS_HI 0x5
#define FLAG_OFF 0x196C      /* the one-bit flag itself */
#define OLD_OFF  0x1968      /* what it was before this call */

/** Flip the one-bit flag at 0x5196C and store its previous value at 0x51968.
 *  Returns nothing; the old value is the point. */
__attribute__((noreturn)) void func_000E5524(void) {
    __asm__ __volatile__(
        "lui  $a0, %[hi]\n\t"
        "lw   $a1, %[flag]($a0)\n\t"
        "lui  $a2, %[hi]\n\t"
        "addiu $a3, $a1, 0x1\n\t"
        "sw   $a1, %[old]($a2)\n\t"
        "andi $a1, $a3, 0x1\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "sw   $a1, %[flag]($a0)\n\t"
        ".set reorder\n\t"
        : : [hi] "i" (GLOBALS_HI), [flag] "i" (FLAG_OFF), [old] "i" (OLD_OFF)
        : "memory", "$a0", "$a1", "$a2", "$a3");
}