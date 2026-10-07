/**
 * The Sims 2 PSP - func_00140A58 (0x00140A58, 0x1C bytes)
 *
 * Reads one bit out of a packed flag byte: `flags[index] & 0x07`, where `flags`
 * is the byte array at 0x001E1B98.
 *
 * The engine packs many independent booleans into single bytes rather than
 * using a byte each - the array holds `0xF0` and `0x7F` masks side by side,
 * which is only meaningful if several bits live in one byte.  The three
 * instructions that build the address are the giveaway that it is an array
 * indexed by a runtime value rather than a single global: `lui` + `addiu` gives
 * the base, a second `addiu` adds 1 so the index lands on the *second* byte of
 * the pair at 0x1E1B98, and `addu` folds in the caller's index.
 *
 * `lb` rather than `lbu` because the mask is applied to the sign-extended value,
 * and `andi $v0, $v0, 0x7` in the return's delay slot keeps only three bits.
 *
 * The sibling func_00140A74 is the same with mask 0x04, and the pair sit four
 * words apart in the link order.
 */
#include "types.h"

/* The packed flag bytes, indexed from the second entry. */
extern s8 flag_bytes_001E1B99[];

u32 func_00140A58(u32 index) {
    /* `$a1` is left uninitialised on purpose.  The original builds the address
     * in it and never uses the incoming value, so giving it an initialiser here
     * makes GCC emit a `move $a1, $a0` to reconcile the two. */
    register u32 base asm("$a1");
    register u32 offset asm("$a0") = index;
    register s32 value asm("$v0");

    __asm__ __volatile__(
        "lui   %[a1], 0x1E\n\t"
        "addiu %[a1], %[a1], 0x1B98\n\t"
        "addiu %[a1], %[a1], 1\n\t"
        "addu  %[a0], %[a1], %[a0]\n\t"
        "lb    %[v0], 0x0(%[a0])\n\t"
        "andi  %[v0], %[v0], 0x7\n\t"
        : [a0] "+r"(offset), [a1] "+r"(base), [v0] "+r"(value)
        :
        : "memory");
    return (u32)value;
}