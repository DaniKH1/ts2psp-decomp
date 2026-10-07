/**
 * The Sims 2 PSP - func_000AFE00 (0x000AFE00, 0x1C bytes)
 *
 * Steps a module-level object back by four and returns 0.
 *
 *     lui   $a0, 0x1D            0x1D0000
 *     lw    $a0, 0x5760($a0)     the object is reached through a global pointer
 *     move  $v0, $zero           the return is a constant 0
 *     lw    $a1, 0x4($a0)
 *     addiu $a1, $a1, -0x4       - 4
 *     jr    $ra
 *     sw    $a1, 0x4($a0)        written back, in the delay slot
 *
 * **The object is reached through a pointer stored in a global**, not through an
 * argument: `0x1D5760` holds the address, and it is dereferenced before anything
 * else happens.  So this is a method on a global singleton rather than one on any
 * caller-supplied object - the same pattern as func_000FFBE8, func_000FE624 and
 * func_00124658, where the receiver is rebuilt from an address instead of arriving
 * in `$a0`.  Here the global holds a *pointer* to the object rather than being one,
 * which is the first of that family where the address has to be loaded as well as
 * built.
 *
 * **The step is four, and the field is a word.**  `- 4` on a `u32` at offset 4 is
 * a pointer or a count of four-byte items moving down one slot - a free-list head
 * or an array cursor, not a byte count.  There is no bounds test and no comparison,
 * so this assumes the field is positive and does not guard: whatever owns it is
 * responsible for having called this the right number of times.  `delay_slots.py`
 * groups it with the other `$zero` returns, and `or $v0, $zero, $zero` is the
 * third-largest such group at 127 functions.
 *
 * The return is a constant zero and carries no information.  Functions that return
 * a fixed value are usually predicates or commands that cannot fail; a command that
 * always succeeds is the more common of the two, and this one mutates so it fits.
 */
#include "types.h"

typedef struct Global1D5760 {
    u32 field_00;   /* 0x0 - not touched */
    u32 cursor;     /* 0x4 - stepped down by 4 */
} Global1D5760;

s32 func_000AFE00(void) {
    register Global1D5760 *node asm("$a0");
    register u32 previous asm("$a1");

    /* The `lui` + `lw` pair, in asm because psp-gcc folds the address its own way.
     * `$a0` is an earlyclobber output because the block both builds and then
     * reads it. */
    __asm__ __volatile__(
        "lui   %[n], 0x1D\n\t"
        "lw    %[n], 0x5760(%[n])\n\t"
        "move  $v0, $zero\n\t"
        "lw    %[t], 0x4(%[n])\n\t"
        "addiu %[t], %[t], -0x4\n\t"
        : [n] "=&r"(node), [t] "=&r"(previous)
        :
        : "memory", "hi", "lo", "$v0");

    /* The store is left to C so it fills the return's delay slot.  `previous` is
     * an uninitialised hard register: the block already put the stepped-down value
     * in `$a1`, and letting C do the load and subtract puts them in `$v1` instead,
     * which costs a `move` the original does not have.  The function takes no
     * arguments, so `$a1` is free and CodeWarrior used it. */
    node->cursor = previous;

    /* Likewise the return: `$v0` is already 0. */
    register s32 result asm("$v0");
    return result;
}