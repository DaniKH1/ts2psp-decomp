/**
 * The Sims 2 PSP - func_0016F698 (0x0016F698, 0x18 bytes)
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
 * the module have this shape, with destination offsets 0x48, 0x48, 0x50 and 0xB8;
 * see `func_0016BC70` for the full write-up, including why the source pointer is
 * reused for the second word and why `.set noreorder` is required.
 *
 * Byte for byte the same body as `func_0016BC70`, at a different address: two
 * translation units' copies of the same inline accessor.
 */
#include "types.h"

/** Copy the first two words of the second argument into the eight bytes at offset
 *  0x48 of the first.  Returns nothing; the copy is the whole body. */
__attribute__((noreturn)) void func_0016F698(void *a0, void *a1) {
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