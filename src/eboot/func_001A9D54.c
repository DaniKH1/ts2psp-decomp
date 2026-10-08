/**
 * The Sims 2 PSP - func_001A9D54 (0x001A9D54, 0x14 bytes)
 *
 * Sets one bit in a flags word held at offset 0x18 of the first argument.
 *
 *     lw    $a1, 0x18($a0)
 *     lui   $a2, 0x10
 *     or    $a1, $a1, $a2
 *     jr    $ra
 *     sw    $a1, 0x18($a0)
 *
 * **`self->flags_18 |= 0x100000;`**  - sets bit 20 and leaves every other bit
 * alone.
 *
 * A read-modify-write rather than a blind store, so the other flag bits in the
 * same word survive.  `$a0` keeps the object pointer throughout - the masked-in
 * value goes to `$a1`, which is free because the argument it would have held is
 * never used - and the store is in the return's delay slot.
 *
 * See `func_001A9ACC` for why the mask needs `lui` rather than an immediate, and
 * `func_001A9D80` for the getter and `func_001A9D68` for the clearer that go with
 * this setter: bit 20 is the one bit in this word where all three survive.
 */
#include "types.h"

/** Set bit 20 (0x100000) of the flags word at offset 0x18 of the first argument. */
__attribute__((noreturn)) void func_001A9D54(void *a0) {
    (void)a0;
    __asm__ __volatile__(
        "lw    $a1, 0x18($a0)\n\t"
        "lui   $a2, 0x10\n\t"
        "or    $a1, $a1, $a2\n\t"
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "sw    $a1, 0x18($a0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$a1", "$a2");
}