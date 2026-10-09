/**
 * The Sims 2 PSP - func_0016F674 (0x0016F674, 0x24 bytes)
 *
 *     addiu $sp, $sp, -0x20
 *     andi  $a3, $a2, 0xFF
 *     lui   $a2, %hi(D_66727573)
 *     sw    $ra, 0x10($sp)
 *     jal   func_000D90F4
 *       addiu $a2, $a2, %lo(D_66727573)
 *     lw    $ra, 0x10($sp)
 *     jr    $ra
 *     addiu $sp, $sp, 0x20
 *
 * Registers a chunk of type **"surf"** with func_000D90F4.
 *
 * `D_66727573` is the four bytes 73 75 72 66 - read in file order that
 * is "surf", and this is the second registration of that tag after
 * func_0010260C, so the renderer chunk table has more than one entry
 * carrying the same name.
 *
 * **The third argument is overwritten with the tag, and the fourth
 * keeps the old third masked to a byte.**  The `andi $a3, $a2, 0xFF`
 * runs before the `lui`, so $a2 is still the caller's third argument
 * when it is narrowed; after the `lui`/`addiu` pair $a2 holds the tag
 * address and the caller's value survives only in $a3.
 */
#include "types.h"

__attribute__((noreturn)) void func_0016F674(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "andi  $a3, $a2, 0xFF\n\t"
        "lui   $a2, %%hi(D_66727573)\n\t"
        "sw    $ra, 0x10($sp)\n\t"
        "jal   func_000D90F4\n\t"
        "addiu $a2, $a2, %%lo(D_66727573)\n\t"
        "lw    $ra, 0x10($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}