/**
 * The Sims 2 PSP - func_0009232C (0x0009232C, 0x20 bytes)
 *
 * Clears three fields of an object and returns it.
 *
 *     lb   $a1, 0x2A($a0)
 *     sw   $zero, 0x0($a0)
 *     addiu $a2, $zero, -0xC1
 *     sh   $zero, 0x28($a0)
 *     and  $a1, $a1, $a2
 *     sb   $a1, 0x2A($a0)
 *     jr   $ra
 *     move $v0, $a0
 *
 * **`self->word_00 = 0; self->half_28 = 0; self->byte_2A &= ~0x40; return this;`**
 *
 * **The mask is spelled `-0xC1`, not `-0x40`, and not `andi $a1, $a1, 0xBF`.**
 * `~0x40` as a 32-bit constant is 0xFFFFFF3F, which does not fit a *signed*
 * 16-bit immediate - the largest positive one is 0x7FFF - so it has to be built as
 * its negation, which is the `addiu`.  A one-instruction `andi` against 0xBF would
 * have done the same job on the same byte, and the module uses `andi` elsewhere for
 * exactly that (`func_001A9CF8` masks a flags word with 0x2000, `func_000706A8`
 * masks with 0xFF00).  So which of the two spellings appears here is a fact about how
 * the source was written, not about what the mask is, and the file records the one
 * that is actually there.
 *
 * **The three fields are not adjacent, and the last two overlap.**  +0x00 is a word,
 * +0x28 a halfword and +0x2A a byte.  The source is `field_00 = 0; field_28 = 0;
 * field_2A &= ~0x40;`, and because `sh $zero, 0x28` already clears both bytes of the
 * halfword, the byte store has to put back the *other* six bits of what was there.
 * That is why the `lb` is first in address order and its result is live across two
 * stores: the load has to happen before the halfword write destroys the byte.
 *
 * `move $v0, $a0` in the delay slot is the `return this`, and `$a0` is clobbered so
 * the pointer has to be named in the block rather than left in the incoming
 * register.
 */
#include "types.h"

/** Clear three fields of the object and return it.
 *  @param self In $a0: the object, whose pointer is returned.
 *  @return     `self`. */
__attribute__((noreturn)) void *func_0009232C(void *self) {
    __asm__ __volatile__(
        "lb   $a1, 0x2A($a0)\n\t"
        "sw   $zero, 0x0($a0)\n\t"
        "addiu $a2, $zero, -0xC1\n\t"
        "sh   $zero, 0x28($a0)\n\t"
        "and  $a1, $a1, $a2\n\t"
        "sb   $a1, 0x2A($a0)\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "move $v0, $a0\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$v0", "$a0", "$a1", "$a2");
}