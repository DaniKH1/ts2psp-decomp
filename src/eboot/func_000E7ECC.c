/**
 * The Sims 2 PSP - func_000E7ECC (0x000E7ECC, 0x10 bytes)
 *
 * Sets the word at module address `0x1DAA88` to 1.
 *
 *     ori  $a0, $zero, 0x1    build 1
 *     lui  $a1, 0x1E          the page: 0x1E0000
 *     jr   $ra
 *     sw   $a0, -0x5578($a1)  ... and the offset within it
 *
 * **`lui $a1, 0x1E` with a negative displacement is the module's own `gp`-relative
 * addressing.**  The two halves are recombined by the assembler into a single
 * `addiu $a1, $gp, -0x5578` once the linker knows `$gp`, which is why the
 * relocation on the `lui` looks like an ordinary immediate here but is not one.
 * `0x1E0000 - 0x5578 = 0x1DAA88`.
 *
 * This is the smallest instructive function in the queue, because it is a single
 * global flag being set and nothing else.  One-bit globals like this are how the
 * engine records "already initialised" and similar one-shot state: the value is
 * only ever 1, and there is no code path here that clears it, so the accessor is
 * almost certainly a `lw` and a `beqz` somewhere else.  The next function starts
 * at `0x1E7EDC` and begins with three `lui`s building three separate addresses,
 * which is a different kind of work entirely.
 *
 * The constant 1 has to be built with `ori`, not `addiu`.  **Both assemble a 1
 * out of `$zero`, and they are different instructions** - `addiu $a0, $zero, 1`
 * is `0x24010001` and `ori $a0, $zero, 1` is `0x34010001`.  GCC picks `addiu`
 * because it has no reason to prefer the logical-or form, so the `ori` is written
 * out here.
 */
#include "types.h"

void func_000E7ECC(void) {
    register u32 one asm("$a0");
    register u32 page asm("$a1");

    /* All three instructions are here because none of them survives being written
     * in C: GCC folds the absolute address into `lui $v0, 0x1D` + `ori $v0, $v0,
     * 0xAA88` with the pointer in $v0, whereas the original keeps the address as
     * a page plus a signed displacement, in $a1.
     *
     * Both registers are outputs and neither is initialised, because the asm
     * writes them before anything reads them.  That is what lets GCC reuse $a0
     * and $a1 here instead of setting up temporaries of its own.
     *
     * With no `jr` in the block, GCC emits its own return and fills the delay
     * slot.  It duplicates the `sw`, which is safe: storing the same value to the
     * same address twice has the same effect as storing it once.
     */
    __asm__ __volatile__(
        "ori %[one], $zero, 1\n\t"
        "lui %[page], 0x1E\n\t"
        "sw  %[one], -0x5578(%[page])\n\t"
        : [one] "=&r"(one), [page] "=&r"(page)
        :
        : "memory");
}