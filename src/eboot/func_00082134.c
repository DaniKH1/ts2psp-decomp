/**
 * The Sims 2 PSP - func_00082134 (0x00082134, 0x10 bytes)
 *
 * Writes 0x2000 through the second argument and returns 0.
 *
 *     addiu $v1, $zero, 0x2000
 *     addu  $v0, $zero, $zero
 *     jr    $ra
 *     sw    $v1, 0x4($a1)      the store, in the delay slot
 *
 * **The store is the last instruction of the function**, which is where delay-slot
 * filling puts it, and the return value is materialised *before* the branch.  That is
 * the opposite of the ordering in `func_000E4970`, where three stores come first and
 * `move $v0, $zero` sits in the slot.  Both are the same job - a setter that returns a
 * status of zero - and the difference is only how much there was to do beforehand.
 *
 * **`addu $v0, $zero, $zero` rather than `move $v0, $zero` is not a different
 * instruction.**  On this ISA `move` is `or`, and `or $v0, $zero, $zero` is exactly
 * what the assembler would emit; spimdisasm prints whichever mnemonic the opcode's
 * preferred form suggests, which is why the same effect reads `move` elsewhere in the
 * module and `addu` here.  It does matter for transcription though, because psp-gcc
 * always writes the `move` spelling, so the C has to come from asm to reproduce it.
 *
 * **Why the whole body is asm rather than the usual "leave the last store to C".**
 * Leaving the store to C means letting GCC emit its own `jr`, and GCC emits its own
 * `move $v0, $zero` with it - it will not accept the `addu` spelling from an input,
 * and it will not use the one already sitting in `$v0` because it has no reason to
 * think the register is live.  Putting `jr $ra` and its delay slot inside the block
 * with `noreturn` is the same trade `func_00025594` makes, and for the same reason: the
 * return has to be the last thing in the function and nothing C emits can put an
 * instruction after it.
 *
 * 0x2000 is 8 KiB, and it lands at offset 4 of the caller's second argument - so the
 * first argument is not used at all.  A setter that ignores its own `this` is the
 * signature of a free function or an out-parameter helper rather than a method.
 */
#include "types.h"

/* 8 KiB. */
#define VALUE   0x2000u

/* The offset is 4, so the pointed-at structure has something at 0x0 and this at 0x04. */
typedef struct Target {
    u32 head;
    u32 value;
} Target;

/* Deliberate on a function that does return: it tells GCC not to emit an epilogue,
 * because the block below already contains the `jr $ra` and the instruction in its
 * delay slot. */
__attribute__((noreturn)) s32 func_00082134(s32 unused, Target *target) {
    /* `$a1` is read but not written, so it is an input; `$v1` and `$v0` are written by
     * the block and never read back, so they need no constraint at all. */
    (void)unused;
    __asm__ __volatile__(
        "addiu $v1, $zero, 0x2000\n\t"
        "addu  $v0, $zero, $zero\n\t"
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "sw    $v1, 0x4(%[t])\n\t"
        ".set reorder\n\t"
        : : [t] "r"(target)
        : "memory", "$v1", "$v0");
}