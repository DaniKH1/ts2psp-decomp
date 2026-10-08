/**
 * The Sims 2 PSP - func_0016BC70 (0x0016BC70, 0x18 bytes)
 *
 * Copies two words out of one structure into a member of another.
 *
 *     lw    $a2, 0x0($a1)
 *     addiu $a0, $a0, 0x48
 *     lw    $a1, 0x4($a1)
 *     sw    $a2, 0x0($a0)
 *     jr    $ra
 *     sw    $a1, 0x4($a0)
 *
 * **An eight-byte copy into `a0->field_48`, read from `*a1`.**  Four functions in
 * the module have this shape, with destination offsets 0x48, 0x48, 0x50 and 0xB8.
 *
 * The scheduling is the interesting part.  The first source word is loaded into
 * `$a2` *before* the destination pointer is adjusted, because `$a1` - the source
 * pointer - is about to be overwritten by the second load.  Reusing `$a1` for the
 * second word is what makes this a two-word copy with no spare register: the
 * pointer dies at the same moment its last use does, so the code never needs a
 * third one.  A copy that read both words before writing either would need one.
 *
 * `.set noreorder` is required: the assembler would hoist the preceding `sw` into
 * the delay slot, which would store the *second* word at offset 0 and the first at
 * offset 4 - the copy transposed.
 */
#include "types.h"

/** Copy the first two words of the second argument into the eight bytes at offset
 *  0x48 of the first.  Returns nothing; the copy is the whole body. */
__attribute__((noreturn)) void func_0016BC70(void *a0, void *a1) {
    (void)a0;
    (void)a1;
    __asm__ __volatile__(
        "lw    $a2, 0x0($a1)\n\t"
        "addiu $a0, $a0, 0x48\n\t"
        "lw    $a1, 0x4($a1)\n\t"
        "sw    $a2, 0x0($a0)\n\t"
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "sw    $a1, 0x4($a0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$a0", "$a1", "$a2");
}