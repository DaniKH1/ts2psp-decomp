/**
 * The Sims 2 PSP - func_000A9A00 (0x000A9A00, 0x18 bytes)
 *
 * Maps a possibly-null pointer to a status word.
 *
 *     lw    $a0, 0x0($a0)          the field
 *     ori   $v0, $zero, 0x0        the answer if it is set...
 *     bnel  $a0, $zero, done       ... which is the likely case
 *     addiu $v0, $a0, -0x18        ... and if it is clear, -24
 *     jr    $ra
 *     nop
 *
 * **The two answers are 0 and -24, and the null case is the one with a name.**  A
 * getter that returned null for "nothing here" would not need a second instruction;
 * -24 is an offset, so this is not a status at all - it is a **pointer past the end
 * of an array**, or a sentinel one-past-the-last-element index.
 *
 * That reading is much better than "returns an error code", because -24 is not a code
 * anyone would invent: it is `-24`, a round number, and the only place a value like
 * that naturally appears is as a negated length or stride.  So the function reads:
 *
 *     int count = obj->count_or_index;
 *     return count != 0 ? 0 : -24;
 *
 * Which is what a caller does when a missing entry should index backwards rather than
 * fail - reading one element before the start is a common trick for a table with a
 * sentinel at the front.
 *
 * **The branch is `bnel`, not `beql`, and that is the whole trick.**  `bnel` nullifies
 * its delay slot when the branch *is* taken, so the `addiu` runs only on the
 * fall-through.  `beql` would nullify it the other way round and give the opposite
 * answer.  Writing this in C gets `bnel` only if GCC agrees the fall-through is the
 * likely path - which it will here, because it is the `? :` and GCC reads a
 * conditional as "unlikely by default" on this target.
 */
#include "types.h"

typedef struct Holder {
    void *field;   /* 0x0 - the value being tested */
} Holder;

/* -24: the answer when the field is clear. */
#define NONE   (-24)

__attribute__((noreturn)) s32 func_000A9A00(Holder *self) {
    (void)self;
    register void *value asm("$a0");

    /* `$a0` is named in the template rather than as a second operand because the
     * load overwrites it with the value it loads: the argument and the result share
     * one register, and two outputs may not.
     *
     * The branch has to be written out.  Left to C this is `value ? 0 : -24`, and
     * psp-gcc answers it with a conditional *move* - `addiu $v1, $zero, -0x18` then
     * `movz $v0, $v1, $a0` - which is one branch fewer and four bytes shorter than
     * the original.  That is the same class of disagreement as the `ins` and `negu`
     * notes in the README, and here it is fatal rather than cosmetic because the
     * instruction count differs.
     *
     * `.set noreorder` holds the `addiu` in the slot.  The `jr $ra` is written out
     * with its slot left to the assembler: writing `nop` there as well gives
     * **seven** instructions and a 28-byte function, so the delay slot is filled
     * either way and the explicit one is an extra.  That is worth knowing against
     * the rule of thumb that an assembler-filled slot is not counted - it is not
     * counted when the compiler has an epilogue to attach it to, and it *is* when
     * the block is the last thing in the function.
     *
     * `noreturn` is a lie, and deliberately so: the function does return, but the
     * block above already contains the `jr $ra`.  Without the attribute GCC emits an
     * epilogue of its own and the function comes out eight bytes too long.  This is
     * the same trick as elsewhere in this directory - tell the compiler not to
     * generate a return, when the return is already written. */
    __asm__ __volatile__(
        "lw    %[v], 0x0($a0)\n\t"
        "ori   $v0, $zero, 0x0\n\t"
        ".set noreorder\n\t"
        "bnel  %[v], $zero, 1f\n\t"
        "addiu $v0, %[v], -0x18\n\t"
        ".set reorder\n\t"
        "1:\n\t"
        "jr    $ra\n\t"
        : [v] "=&r"(value)
        :
        : "memory");

    register s32 answer asm("$v0");
    return answer;
}