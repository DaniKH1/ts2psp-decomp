/**
 * The Sims 2 PSP - sortAndCullScene_069C (0x001B42B8, 0x8 bytes)
 *
 *     jr    $ra
 *       nop
 *
 * Returns immediately and does nothing else.
 *
 * **The body is empty and this is decidable from the bytes**: the single
 * delay slot is a real `nop` (`00000000`), not a useful move that happens
 * to be idle.  No register is read and none is written, so whatever the
 * caller passes in `$a0`..`$a3` and `$t0`..`$t9` comes back out untouched,
 * and whatever was in `$v0` is what the caller will see.
 *
 * It lives in `.text.sortAndCullScene`, the section the linker named after
 * the source directory that produced it.  **What the function is *for* is
 * not determinable from these eight bytes** - it is not determinable at
 * all from here.  A body of pure `jr $ra`/`nop` in a rendering section is
 * consistent with several different source-level things (an overridden
 * virtual method whose body is `{}`, a stub left after a feature was
 * cut, or a constructor of an empty base class), and the bytes do not
 * distinguish between them.
 */
#include "types.h"

__attribute__((noreturn)) void sortAndCullScene_069C(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "nop\n\t"
        ".Leboot_001B42B8:\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}