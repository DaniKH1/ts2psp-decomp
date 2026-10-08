/**
 * The Sims 2 PSP - func_001A9D80 (0x001A9D80, 0x14 bytes)
 *
 * Tests one bit of a flags word held at offset 0x18 of the first argument.
 *
 *     lw    $a0, 0x18($a0)
 *     lui   $a1, 0x10
 *     and   $v0, $a0, $a1
 *     jr    $ra
 *     sltu  $v0, $zero, $v0
 *
 * **`return (self->flags_18 & 0x100000) != 0;`**  - bit 20 of the word at offset
 * 0x18.
 *
 * See `func_001A9ACC` for the full write-up: `and` leaves the masked word in `$v0`
 * and `sltu` narrows it to 0 or 1, and the mask needs `lui` because 0x100000 does
 * not fit a signed 16-bit immediate.
 *
 * **This is the only bit of the word whose accessor family is complete.**
 * `tools/flag_accessors.py` is the census; all three of the others for this
 * offset-0x18 word sit within 0x14 bytes of each other:
 *
 *     func_001A9D54  set    bit 20   |= 0x100000
 *     func_001A9D68  clear  bit 20   &= 0xffefffff
 *     func_001A9D80  get    bit 20   (this one)
 *
 * Bit 17 has only the getter and bit 18 only the setter.  Four booleans' worth of
 * accessors survived the link for this class; only one of them kept all three
 * directions.
 */
#include "types.h"

/** Test bit 20 (0x100000) of the flags word at offset 0x18 of the first argument.
 *  @return 1 if the bit is set, 0 otherwise. */
__attribute__((noreturn)) u32 func_001A9D80(void *a0) {
    (void)a0;
    __asm__ __volatile__(
        "lw    $a0, 0x18($a0)\n\t"
        "lui   $a1, 0x10\n\t"
        "and   $v0, $a0, $a1\n\t"
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "sltu  $v0, $zero, $v0\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$a0", "$a1", "$v0");
}