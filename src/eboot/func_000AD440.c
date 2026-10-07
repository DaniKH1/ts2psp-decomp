/**
 * The Sims 2 PSP - func_000AD440 (0x000AD440, 0x10 bytes)
 *
 * Sets two adjacent bytes to one.
 *
 *     ori  $a1, $zero, 0x1
 *     sb   $a1, 0x40($a0)
 *     jr   $ra
 *     sb   $a1, 0x41($a0)     the second byte, in the delay slot
 *
 * **Two bytes, one value, adjacent.**  That is the shape of a two-bit or two-boolean
 * field written as two stores - a "state" that has a couple of independent flags side
 * by side, not a counter and not a value that happens to be 1.
 *
 * The value is built with `ori $a1, $zero, 0x1` rather than `addiu`, which is the same
 * instruction with a different name and no immediate sign extension to think about.
 * psp-gcc folds the literal 1 as `addiu $a1, $zero, 1`, so the constant goes in the
 * asm.
 *
 * Nothing is read here, so the function is a pure setter and there is nothing to test
 * it against: whatever the two bytes mean, this is the code that turns them on.
 */
#include "types.h"

typedef struct Object {
    u8 pad_040[0x40];
    u8 flag_a;   /* 0x40 - set to 1 */
    u8 flag_b;   /* 0x41 - set to 1 */
} Object;

void func_000AD440(Object *self) {
    register Object *node asm("$a0") = self;
    register u32 one asm("$a1");

    /* `ori` rather than `addiu`, which is what psp-gcc would pick for the literal. */
    __asm__ __volatile__(
        "ori   %[v], $zero, 0x1\n\t"
        "sb    %[v], 0x40(%[n])\n\t"
        : [v] "=&r"(one), [n] "+r"(node)
        :
        : "memory");

    /* The second store is left to C so it lands in the return's delay slot, reading
     * the value back out of `$a1` rather than rebuilding the constant in `$v0`. */
    node->flag_b = one;
}