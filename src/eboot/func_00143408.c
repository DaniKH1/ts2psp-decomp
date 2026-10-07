/**
 * The Sims 2 PSP - func_00143408 (0x00143408, 0x34 bytes)
 *
 * One step of a linear congruential generator.
 *
 *     lui   $a0, 0x1E
 *     lw    $a0, 0x1F8C($a0)      the state, reached through a global pointer
 *     lui   $a1, 0x41C6
 *     lw    $a2, 0x58($a0)
 *     addiu $a1, $a1, 0x4E6D      0x41C64E6D = 1103515245
 *     mult  $a2, $a1              state * 1103515245
 *     lui   $a1, 0x8000
 *     addiu $v0, $a1, -0x1        0x7FFFFFFF
 *     mflo  $a1                   the low word of the product
 *     addiu $a1, $a1, 0x3039      + 12345
 *     sw    $a1, 0x58($a0)        the new state
 *     jr    $ra
 *     and   $v0, $a1, $v0         return the new state & 0x7FFFFFFF
 *
 * **1103515245 and 12345 are the POSIX `rand()` constants**, the ones printed in
 * the C standard's own example.  So the sequence is
 * `state = state * 1103515245 + 12345`, and this is the whole generator in thirteen
 * instructions.
 *
 * Two details are worth having right rather than assuming:
 *
 * * **Only `mflo` is taken.**  The multiply is 32x32->64 and the high word is
 *   discarded, so the recurrence is modulo 2^32.  `mult` rather than
 *   `multu` means the state is treated as signed, which changes nothing for the
 *   low word - it is the same bits either way - but it is what the source asked for.
 * * **The mask is applied after the store, to the value that was stored.**  So the
 *   generator advances by the full 32-bit recurrence and only the *returned* value
 *   is clamped to 31 bits.  A different design, the one the standard's example
 *   shows, would do `(state >> 16) & 0x7FFFFFFF`: shift first, and the state would
 *   be kept masked.  This one keeps a full 32-bit state, which is why the period is
 *   2^32 rather than 2^31.
 *
 * The state lives at 0x58 of an object reached through the pointer at 0x1E1F8C, so
 * the generator is a field of a larger object rather than a file-static - a world,
 * a tile map, or a simulation context that owns its own randomness.  That matters
 * more than it looks: two objects with independent states means two independent
 * sequences, which is what a game needs when it wants reproducible streams.
 *
 * **A real `mult`, no strength reduction.**  1103515245 is odd and not near a
 * power of two, so - the same conclusion `func_00049BC4` reached from the other
 * side - there is nothing to shift and add.  This is the second function in the
 * repository where that is the whole reason.
 */
#include "types.h"

typedef struct RngOwner {
    u8 pad_000[0x58];
    u32 state;   /* 0x58 - the whole generator state */
} RngOwner;

u32 func_00143408(void) {
    register RngOwner *node asm("$a0");
    register u32 value asm("$a1");
    register u32 state_in asm("$a2");

    /* Everything but the final mask, which has to be in the delay slot and so is
     * left to C.  `$a2` is the state read out of the object and `$a1` ends up
     * holding the new state, which is what the mask needs. */
    __asm__ __volatile__(
        "lui   %[n], 0x1E\n\t"
        "lw    %[n], 0x1F8C(%[n])\n\t"
        "lui   %[v], 0x41C6\n\t"
        "lw    %[t], 0x58(%[n])\n\t"
        "addiu %[v], %[v], 0x4E6D\n\t"
        "mult  %[t], %[v]\n\t"
        "lui   %[v], 0x8000\n\t"
        "addiu $v0, %[v], -0x1\n\t"
        "mflo  %[v]\n\t"
        "addiu %[v], %[v], 0x3039\n\t"
        "sw    %[v], 0x58(%[n])\n\t"
        : [n] "=&r"(node), [v] "=&r"(value), [t] "=&r"(state_in)
        :
        : "memory", "hi", "lo");

    /* The mask is read back out of `$v0`, where the block already built it.  Written
     * plainly - `value & 0x7FFFFFFF` - psp-gcc recognises "clear the sign bit", in
     * signed *and* unsigned form, and emits `ext $v0, %[v], 0, 31`: one instruction,
     * the same value, and two words that differ from the original.  Laundering the
     * constant through an empty asm stops that but costs a `lui` + `ori` to rebuild
     * it, because `$v0` is then dead as far as the compiler knows.
     *
     * Reading it as a register instead costs nothing and gets both the register and
     * the instruction right: the block ends with the mask in `$v0`, the return's
     * destination is `$v0`, and the only C that says is "and it with the state". */
    register u32 mask asm("$v0");
    return value & mask;
}