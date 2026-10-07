/**
 * The Sims 2 PSP - func_0012C954 (0x0012C954, 0x28 bytes)
 *
 * The same round-down-to-32 as func_0012828C, but writing to a global instead of
 * through an argument.
 *
 *     addiu $a2, $a0, 0x1F        n + 31
 *     addiu $a3, $zero, -0x20     the round-down mask, built as an addiu
 *     and   $a2, $a2, $a3         aligned = n & ~31
 *     lui   $a3, 0x6              0x60000 - the global's page
 *     addu  $a0, $a0, $a1         n + delta
 *     sw    $a2, 0x47D0($a3)     the global gets `aligned`
 *     addiu $a1, $a3, 0x47D0     recompose the address, now in $a1
 *     sw    $a0, 0x4($a1)        the global's second field
 *     jr    $ra
 *     sw    $zero, 0x8($a1)      ... and a third field, cleared
 *
 * **This is what `0x47D0` is: an absolute address.**  `lui $a3, 0x6` then
 * `0x47D0($a3)` is the module-relative address `0x647D0`, a fixed location in
 * `.bss` rather than anything derived from an argument.  The engine reaches for
 * it directly.
 *
 * Why the address is built twice is the interesting part.  Once `lui` has loaded
 * the page into `$a3`, the store could use `0x47D0($a3)` again for the next two -
 * but `$a3` is needed as a *value* immediately afterwards, for
 * `addiu $a1, $a3, 0x47D0`.  So the compiler materialises the full 32-bit address
 * in `$a1` and switches to a zero-displacement store.  `$a2`'s mask dies at the
 * `and`, which frees `$a3` for the `lui` in the same slot.
 *
 * The three fields at 0x647D0 are therefore {aligned, n + delta, 0} - the same
 * pair func_0012828C writes to an object, plus a field explicitly cleared rather
 * than left alone.  Comparing the two is the way to tell what that third field
 * means: the object version does not touch it, so the clearing here is specific
 * to this call site and not part of the shared helper.
 */
#include "types.h"

/* The fixed location the original addresses directly: lui $a3, 0x6 with a
 * 0x47D0 displacement.  A distinct type from the caller's own state, because it
 * is global and not reached through any argument. */
typedef struct GlobalExtent {
    u32 aligned;   /* 0x647D0 */
    u32 applied;   /* 0x647D4 */
    u32 cleared;   /* 0x647D8 */
} GlobalExtent;

void func_0012C954(u32 n, u32 delta) {
    register u32 aligned asm("$a2");
    register u32 mask asm("$a3");
    register u32 sum asm("$a0");
    register u32 addr asm("$a1");

    __asm__ __volatile__(
        "addiu %[aligned], %[n], 0x1F\n\t"
        "addiu %[mask], $zero, -0x20\n\t"
        "and   %[aligned], %[aligned], %[mask]\n\t"
        "lui   %[mask], 0x6\n\t"
        "addu  %[sum], %[n], %[d]\n\t"
        "sw    %[aligned], 0x47D0(%[mask])\n\t"
        "addiu %[addr], %[mask], 0x47D0\n\t"
        : [aligned] "+r"(aligned), [mask] "+r"(mask),
          [sum] "+r"(sum), [addr] "+r"(addr)
        : [n] "r"(n), [d] "r"(delta)
        : "memory");

    ((GlobalExtent *)addr)->applied = sum;
    ((GlobalExtent *)addr)->cleared = 0;
}