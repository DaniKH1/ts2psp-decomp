/**
 * The Sims 2 PSP - func_0019B9DC (0x0019B9DC, 0x28 bytes)
 *
 *     addiu $sp, $sp, -0x20
 *     or    $a3, $a2, $zero
 *     or    $a2, $a1, $zero
 *     lui   $a1, %hi(D_2161756C)
 *     sw    $ra, 0x10($sp)
 *     jal   func_000D6B20
 *       addiu $a1, $a1, %lo(D_2161756C)
 *     lw    $ra, 0x10($sp)
 *     jr    $ra
 *     addiu $sp, $sp, 0x20
 *
 * Registers a handler for the **same "!vlab" tag** as func_0019B9B8,
 * but through a different entry point: func_000D6B20 rather than
 * func_000D90F4.
 *
 * **The two `or`s are the whole point.**  Both arguments shift right by
 * one - `$a1` becomes `$a2` and `$a2` becomes `$a3` - so this is a
 * four-argument call with one argument pre-loaded, the tag being the
 * first of the three remaining slots.  Register shuffling rather than
 * arithmetic.
 *
 * Read together with func_0019B9B8 and func_0016F674, the "!vlab" tag
 * has **two** entry points and "surf" has one, which is the first
 * evidence that the two names are not parallel: the tag is not a key
 * into a single table, it selects a registration path.
 */
#include "types.h"

__attribute__((noreturn)) void func_0019B9DC(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "or    $a3, $a2, $zero\n\t"
        "or    $a2, $a1, $zero\n\t"
        "lui   $a1, %%hi(D_2161756C)\n\t"
        "sw    $ra, 0x10($sp)\n\t"
        "jal   func_000D6B20\n\t"
        "addiu $a1, $a1, %%lo(D_2161756C)\n\t"
        "lw    $ra, 0x10($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}