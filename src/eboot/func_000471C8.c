/**
 * The Sims 2 PSP - func_000471C8 (0x000471C8, 0x30 bytes)
 *
 * A constructor: sets a status word to -2, points a field at a fixed global, and
 * clears five words.  Returns the object.
 *
 *     addiu $a1, $zero, -0x2     -2
 *     sw    $a1, 0x0($a0)        status = -2
 *     lui   $a1, 0x1E            0x1E0000 ...
 *     addiu $a1, $a1, 0x52A0     ... + 0x52A0 = 0x1E52A0
 *     sw    $a1, 0x4($a0)        field = &the global at 0x1E52A0
 *     sw    $zero, 0x8($a0)      five words cleared, one store each
 *     sw    $zero, 0xC($a0)
 *     sw    $zero, 0x10($a0)
 *     sw    $zero, 0x14($a0)
 *     sw    $zero, 0x18($a0)
 *     jr    $ra
 *     move  $v0, $a0
 *
 * **The field at 0x4 holds the object's own global state.**  `0x1E52A0` is reached
 * by an unrelocated-looking `lui`/`addiu` pair carrying R_MIPS_HI16/LO16
 * relocations, and nothing in any argument leads there - so this is a fixed
 * location in `.bss`, and the object is pointing at it.  That is the shape of a
 * singleton or a per-subsystem manager: one instance, whose state lives outside
 * it so that it can be reached without a pointer to the object.  `0x1E52A0` is in
 * the same page as the one-shot flag at `0x1DAA88` that func_000E7ECC sets.
 *
 * Five separate `sw $zero` rather than a loop or a `memset`: the count is a
 * compile-time constant, so the compiler unrolled it.  Worth noting that nothing
 * tries to be clever here - a block of 20 bytes of zeros is the easiest possible
 * thing to turn into a `memset` call and it did not happen.
 *
 * Status -2 is the same "sentinel, not zero" idea as the -1 in func_00116CD4.  A
 * status word of -2 usually means a third state beyond "not started" and "done",
 * often "failed" or "cancelled", which is why it cannot be 0 or 1.
 */
#include "types.h"

/* The fixed location, built as lui 0x1E + addiu 0x52A0. */
#define STATE_AT   0x0001E52A0u

typedef struct Manager {
    s32 status;      /* 0x0 - always -2 here */
    u32 *state;      /* 0x4 - points at the global at 0x1E52A0 */
    u32 clear[5];    /* 0x8 .. 0x1C - all zero */
} Manager;

Manager *func_000471C8(Manager *self) {
    register Manager *dst asm("$a0") = self;
    register u32 v asm("$a1");

    __asm__ __volatile__(
        "addiu %[v], $zero, -2\n\t"
        "sw    %[v], 0x0(%[d])\n\t"
        "lui   %[v], 0x1E\n\t"
        "addiu %[v], %[v], 0x52A0\n\t"
        "sw    %[v], 0x4(%[d])\n\t"
        "sw    $zero, 0x8(%[d])\n\t"
        "sw    $zero, 0xC(%[d])\n\t"
        "sw    $zero, 0x10(%[d])\n\t"
        "sw    $zero, 0x14(%[d])\n\t"
        "sw    $zero, 0x18(%[d])\n\t"
        : [v] "+r"(v), [d] "+r"(dst)
        :
        : "memory");

    return dst;
}