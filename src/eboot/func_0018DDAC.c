/**
 * The Sims 2 PSP - func_0018DDAC (0x0018DDAC, 0x18 bytes)
 *
 * Points one field at a fixed global and sets another to -2.
 *
 *     lui   $a0, 0x1F            0x1F0000
 *     addiu $a0, $a0, -0x6AB8     0x1E9548
 *     addiu $a2, $zero, -0x2      -2
 *     sw    $a0, 0x4($a1)        field at 0x4 = &that global
 *     jr    $ra
 *     sw    $a2, 0x0($a1)          field at 0x0 = -2, in the delay slot
 *
 * Two fields of a two-word node, and **the -2 is the whole point.**  Zero would be
 * the natural "empty" value and it is not used: -2 is below both zero and -1, so it
 * is distinguishable from any valid index or handle a loop might produce.  That is
 * the same reasoning as the -1 handles in func_00116CD4 and the -2 status in
 * func_000471C8 - this engine uses negative sentinels where zero is a real value.
 *
 * So the pair reads as `{ next: -2, back: &global }` or the reverse, and either way
 * a two-word link node that is *not* part of a chain yet: the -2 marks it as a
 * standalone node, and the pointer is to a fixed location rather than to another
 * node.  A node that is in a list has a real neighbour in both fields.
 *
 * `0x1E9548` is in the same 0x1E page as the other globals found so far -
 * `0x1DAA88`, `0x1E52A0`, `0x1DA398`, `0x1DEE08` - which makes that page the
 * module's writable data rather than anything per-object.
 */
#include "types.h"

/* The fixed location: lui 0x1F, addiu -0x6AB8, so 0x1F0000 - 0x6AB8 = 0x1E9548. */
#define TARGET   0x0001E9548u

typedef struct Link {
    s32 head;    /* 0x0 - -2: not linked */
    void *back;   /* 0x4 - a fixed global while unlinked */
} Link;

/* `$a0` is never read as a pointer: the function overwrites it with the constant
 * and stores through `$a1`.  So the node arrives in the *second* parameter slot
 * and the source had a first parameter this body no longer needs - the same shape
 * as func_000DF534.  Declaring it makes `$a1` the node and costs nothing, where
 * binding the single argument to `$a1` would make GCC emit a `move`. */
void func_0018DDAC(void *unused, Link *self) {
    (void)unused;

    /* Both constants are built here rather than in C because psp-gcc folds a
     * 32-bit literal its own way: left to write `0x1E9548` it emits
     * `lui $a0, 0x1E` + `ori $a0, $a0, 0x9548`, while the original has
     * `lui $a0, 0x1F` + `addiu $a0, $a0, -0x6AB8`.  Both are the same value and
     * both are two instructions; only one of them matches. */
    register u32 addr asm("$a0");
    register s32 marker asm("$a2");
    /* `self` has to be pinned to `$a1` rather than left to the allocator: given a
     * free choice GCC put it in `$v0` and copied `$a0` into it, adding a `move` the
     * original does not have.  `$a1` is where the second argument arrives, so the
     * binding costs nothing. */
    register Link *node asm("$a1") = self;

    __asm__ __volatile__(
        "lui   %[a], 0x1F\n\t"
        "addiu %[a], %[a], -0x6AB8\n\t"
        "addiu %[m], $zero, -0x2\n\t"
        "sw    %[a], 0x4(%[self])\n\t"
        : [a] "=&r"(addr), [m] "=&r"(marker)
        : [self] "r"(node)
        : "memory");

    /* Left to C so it lands in the return's delay slot. */
    self->head = marker;
}