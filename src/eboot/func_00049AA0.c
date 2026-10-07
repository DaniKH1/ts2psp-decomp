/**
 * The Sims 2 PSP - func_00049AA0 (0x00049AA0, 0x10 bytes)
 *
 * Writes a value into the second word of a global.  `$a0` is the value.
 *
 * The setter paired with func_00049A90, which reads the same word back.  The
 * address is materialised into `$a1` with `lui` + `addiu` and kept there,
 * where psp-gcc uses `$v0` and folds the `addiu` into the store's offset.
 *
 * `%%hi`/`%%lo` rather than `%hi`/`%lo`: in GCC inline asm a literal percent
 * is written `%%`.
 */
#include "types.h"

/* The global is eight bytes long, so this is its second word. */
extern u32 sym_000743A0[2];

void func_00049AA0(u32 value) {
    __asm__ __volatile__(
        "lui  $a1, %%hi(sym_000743A0)\n\t"
        "addiu $a1, $a1, %%lo(sym_000743A0)\n\t"
        "sw   %[v], 0x4($a1)\n\t"
        : : [v] "r"(value)
        : "$a1", "memory");
}