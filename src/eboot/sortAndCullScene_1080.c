/**
 * The Sims 2 PSP - sortAndCullScene_1080 (0x001B4C9C, 0x34 bytes)
 *
 * Counts the array that starts 0x110 into `self` and returns the last element's
 * field at offset 8.
 *
 *     lw   $a1, 0x140($a0)
 *     addiu $a0, $a0, 0x110
 *     subu $a1, $a1, $a0
 *     ori  $a2, $zero, 0xC
 *     div  $zero, $a1, $a2
 *     mflo $a1
 *     addiu $a1, $a1, -0x1
 *     addu $a2, $a1, $a1
 *     addu $a1, $a1, $a2
 *     sll  $a1, $a1, 2
 *     addu $a0, $a0, $a1
 *     jr   $ra
 *     lwc1 $f0, 0x8($a0)
 *
 * **`return ((f32 *)((char *)self + 0x110))[(int)((self->end - self->base) / 12) *
 * 12 - 12 + 8];`**
 *
 * **Twelve is written twice, once as a divisor and once as a multiplier, and the
 * compiler's way of building each is different.**
 *
 * The divisor is `ori $a2, $zero, 0xC` - a *pseudo*-instruction that is really
 * `addiu`, which is how any constant outside a signed 16-bit range is normally
 * spelled, and 12 is inside that range so nothing forced it.  Then `sll 2` at the end
 * makes a multiply by four, which is the same choice the two lerps make: **four is
 * spelled `sll 2` everywhere in this module and never `sll $x, $x, 2` reached for by
 * chance.**  So the size of the record is known to be 12 in both directions and the
 * original expressed the divide as a divide rather than keeping the quotient and
 * multiplying, which costs four instructions - `addu`, `addu`, `sll`, `addu` - to
 * recover something `mflo` had already computed.
 *
 * **`div` and not `divu`.**  The word is `00a6001a`: SPECIAL, function 0x1A is `DIV`
 * (signed) and 0x1B would be `DIVU`, so the count of a signed byte difference is
 * what is divided, and a negative `end - base` would trap instead of producing a
 * huge unsigned quotient.  The operand order is `$a1 / $a2` - the word's `rs` field
 * is 5 and `rt` is 6 - which is worth writing down because a listing and the raw
 * word can be read two ways and only the word settles it.
 *
 * The multiply is `addu $a2, $a1, $a1` then `addu $a1, $a1, $a2`, so twelve times a
 * count is reached as `3n` then `<<2` rather than as three shift-and-add steps or a
 * `mult`.  **`mult`/`mflo` would have been one instruction and is nowhere in this
 * function** even though the ISA has it and the previous instruction is `mflo` for
 * a division that used it.  Four instructions to redo what one `mult` would give,
 * because `n` is already a register and the shift-and-add sequence is what psp-gcc
 * emits for a constant multiplier.
 *
 * **The result is loaded in the delay slot.**  `lwc1 $f0, 0x8($a0)` is the function's
 * whole result, and it has to be the instruction *after* `jr $ra` because `$a0` is
 * only correct there - so `.set noreorder` is required, and under `.set reorder` the
 * assembler would fill the slot with the `addu` instead and the return would be the
 * pointer cast to a float.
 */
#include "types.h"

/** Return the float at offset 8 of the last 12-byte element of the array.
 *  @param self In $a0: the array's base is at +0x110 and its end pointer at +0x140.
 *  @return     The float, in `$f0`. */
__attribute__((noreturn)) void sortAndCullScene_1080(void *self) {
    (void)self;
    __asm__ __volatile__(
        "lw   $a1, 0x140($a0)\n\t"
        "addiu $a0, $a0, 0x110\n\t"
        "subu $a1, $a1, $a0\n\t"
        "ori  $a2, $zero, 0xC\n\t"
        "div  $zero, $a1, $a2\n\t"
        "mflo $a1\n\t"
        "addiu $a1, $a1, -0x1\n\t"
        "addu $a2, $a1, $a1\n\t"
        "addu $a1, $a1, $a2\n\t"
        "sll  $a1, $a1, 2\n\t"
        "addu $a0, $a0, $a1\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "lwc1 $f0, 0x8($a0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "hi", "lo", "$a0", "$a1", "$a2", "$f0");
}