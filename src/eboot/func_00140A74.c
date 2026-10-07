/**
 * The Sims 2 PSP - func_00140A74 (0x00140A74, 0x1C bytes)
 *
 * Reads one bit out of a packed flag byte: `flags[index] & 0x04`, where `flags`
 * is the byte array at 0x001E1B98.
 *
 * The same bitfield accessor as func_00140A58 with mask 0x04 instead of 0x07 -
 * the two are four words apart in the link order, which is how the engine lays
 * out a family of one-bit accessors over the same array.  See it for the pins,
 * in particular why `$a1` is left uninitialised.
 */
#include "types.h"

/* The packed flag bytes, indexed from the second entry. */
extern s8 flag_bytes_001E1B99[];

u32 func_00140A74(u32 index) {
    /* `$a1` is uninitialised on purpose; the original builds the address in it
     * and never reads the incoming value. */
    register u32 base asm("$a1");
    register u32 offset asm("$a0") = index;
    register s32 value asm("$v0");

    __asm__ __volatile__(
        "lui   %[a1], 0x1E\n\t"
        "addiu %[a1], %[a1], 0x1B98\n\t"
        "addiu %[a1], %[a1], 1\n\t"
        "addu  %[a0], %[a1], %[a0]\n\t"
        "lb    %[v0], 0x0(%[a0])\n\t"
        "andi  %[v0], %[v0], 0x4\n\t"
        : [a0] "+r"(offset), [a1] "+r"(base), [v0] "+r"(value)
        :
        : "memory");
    return (u32)value;
}