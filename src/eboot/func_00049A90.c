/**
 * The Sims 2 PSP - func_00049A90 (0x00049A90, 0x10 bytes)
 *
 * Reads the second word of a global and returns it.  Four instructions, no
 * floats, no branches and no calls, so there is nothing for the two compilers
 * to disagree about - this is the shape of function where psp-gcc and
 * CodeWarrior are forced to emit the same bytes.
 *
 * The getter paired with func_00049AA0, which writes the same word.
 *
 * The asm pins what psp-gcc decides differently: the address goes in `$a0`
 * and the load sits in the return's delay slot, where GCC uses `$v0` and moves
 * the load above the return.  The original reads through `$a0` rather than
 * loading the global directly because it needs the address in a register
 * first.
 *
 * `%%hi`/`%%lo` rather than `%hi`/`%lo`: in GCC inline asm a literal percent is
 * written `%%`.
 */
#include "types.h"

/* The global is eight bytes long, so this is its second word. */
extern u32 sym_000743A0[2];

u32 func_00049A90(void) {
    u32 result;

    __asm__ __volatile__(
        "lui  $a0, %%hi(sym_000743A0)\n\t"
        "addiu $a0, $a0, %%lo(sym_000743A0)\n\t"
        "lw   %[out], 0x4($a0)\n\t"
        : [out] "=&r"(result)
        :
        : "$a0", "memory");
    return result;
}