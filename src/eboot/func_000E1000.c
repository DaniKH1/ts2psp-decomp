/**
 * The Sims 2 PSP - func_000E1000 (0x000E1000, 0x1C bytes)
 *
 * Sets two module globals to fixed values: one to 3, one to -1.
 *
 *     ori   $a0, $zero, 0x3
 *     lui   $a1, 0x1E
 *     sw    $a0, -0x5DC8($a1)      0x1A2238 = 3
 *     addiu $a0, $zero, -0x1
 *     lui   $a1, 0x1
 *     jr    $ra
 *     sw    $a0, 0x1574($a1)       0x11574 = -1
 *
 * **The two globals are in different regions**, and that is the interesting part.
 * `0x1A2238` is in the `0x1A` page, where the module's other writable globals live;
 * `0x11574` is 0x10000 plus an offset, far below any of them.
 *
 * That is not a mistake - it is a convention.  A `lui` of `0x1` with a positive
 * offset is the cheap way to reach something low in memory without a `lui` of `0x0`
 * (which does not exist), and a linker script that puts a table near the bottom of
 * the image gets it addressed this way for free.  `func_00097200` writes to `0xC9E4`
 * the same way.
 *
 * So this function initialises one global from the module's usual page and one from
 * the low region, and the two are probably related by what the low region *is*: a
 * table the engine sets up before it is ready to touch anything else.
 *
 * **No arguments, no return, and the values are unrelated.**  3 and -1 have nothing to
 * do with each other, so this is not one value written twice - it is two separate
 * settings, and the compiler interleaved them only because it had two free registers.
 * A function that sets one global is four instructions; this is seven because it sets
 * two.
 *
 * The -1 is built with `addiu` rather than `ori`, because -1 does not fit a logical
 * immediate and `ori` would be wrong for it; the 3 uses `ori`, which is the same
 * instruction `addiu` would be.  Both spellings are in here because the two constants
 * want different ones.
 */
#include "types.h"

/* 0x1E0000 - 0x5DC8.  The module's ordinary globals page. */
#define FIRST_GLOBAL    0x0001A2238u

/* 0x10000 + 0x1574.  The low region, addressed the cheap way. */
#define SECOND_GLOBAL   0x00011574u

#define FIRST_VALUE     3u
#define SECOND_VALUE    (-1)

void func_000E1000(void) {
    register u32 first asm("$a0");
    register u32 page asm("$a1");

    /* Both constants and both `lui`s are in the asm: psp-gcc folds a literal its own
     * way, and 0x1A2238 is `lui` + `addiu -0x5DC8` where it would rather emit
     * `lui` + `ori`. */
    __asm__ __volatile__(
        "ori   %[v], $zero, 0x3\n\t"
        "lui   %[p], 0x1E\n\t"
        "sw    %[v], -0x5DC8(%[p])\n\t"
        "addiu %[v], $zero, -0x1\n\t"
        "lui   %[p], 0x1\n\t"
        : [v] "=&r"(first), [p] "=&r"(page)
        :
        : "memory", "hi", "lo");

    /* The second store is left to C so it lands in the return's delay slot.  The
     * `lui 0x1` above is still live in `%[p]` and the -1 in `%[v]`, so the address and
     * the value both cost nothing to rebuild - reading them back is what stops gcc
     * putting -1 in `$v0` instead and adding an instruction. */
    *(s32 *)(page + 0x1574) = (s32)first;
}