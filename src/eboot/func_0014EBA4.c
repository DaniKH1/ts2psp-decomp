/**
 * The Sims 2 PSP - func_0014EBA4 (0x0014EBA4, 0x10C bytes)
 *
 * A static constructor: it writes a fixed pattern into four 0xFC-byte records and
 * zeroes two globals.
 *
 *     lui   $a1, 0x7
 *     addiu $t6, $a1, -0x78D8            ; header at 0x6C728
 *     ...
 *     addiu $a1, $t6, 0x48               ; first record at 0x6C770
 *     addiu $a2, $t6, 0x140              ; 0xF8 past the first record's start
 *     ...
 *     addiu $v1, $v1, -0x1
 *     sw    $t5, 0x0($a1)
 *     ...                                 the record body
 *     addiu $a1, $a1, 0xFC               ; next record
 *     sw    $t0, 0x0($a2)
 *     bgez  $v1, .Lloop
 *     addiu $a2, $a2, 0xFC
 *     lui   $v1, 0x7
 *     addiu $t0, $v1, -0x7908
 *     sw    $zero, -0x7908($v1)
 *     jr    $ra
 *     sw    $zero, 0x4($t0)
 *
 * **Four identical records at 0x6C770, 0x6C86C, 0x6C968, 0x6CA64**, each 0xFC bytes,
 * of which these words are non-zero:
 *
 *     +0x00  4        +0x50  0xFFFF
 *     +0x10  100      +0x58  1
 *     +0x18  1        +0xF8  -1
 *     +0x1C  480
 *     +0x20  272
 *
 * and the header at 0x6C728 has 1 at +0x20, 480 at +0x24 and 0x38, 272 at +0x3C and
 * -1 at +0x08.  Two globals are zeroed: the header's own first word, and the pair at
 * 0x6C6F8.
 *
 * **480 by 272 is the PSP's framebuffer**, and that is arithmetic rather than a
 * guess: 0x1E0 and 0x110 appear in the body and nowhere else, in fields one record
 * apart and side by side in the header.  What the engine *calls* these records is
 * not established here - they could be display modes, framebuffer descriptors or
 * texture formats, and all four being identical is as consistent with "four slots,
 * differentiated later" as with "four the same".  The four bytes at +0x00 and the
 * 0xFFFF at +0x50 are the kind of sentinels that say "not set yet".
 *
 * **The two cursor registers are 0xF8 apart and both step by 0xFC**, which is the
 * detail that makes the layout unambiguous: `$a1` walks the record starts and `$a2`
 * walks 0xF8 behind them, so `$a2`'s store lands on the *last* word of the same
 * record rather than the first word of the next one.  A stride of 0xF8 would have
 * been the other reading, and the loop's `+0xFC` on both settles it.
 *
 * **The store order is the scheduler's, not the source's.**  +0x74 is written early,
 * +0x60 and +0x68 before +0x64, and the constants are materialised in the gaps -
 * `$t5`, `$t4`, `$t3`, `$t2`, `$t1`, `$t0` are each built once and reused four
 * times, which is why there are no `ori`/`addiu` pairs inside the loop at all.  That
 * reuse is the reason the loop is only three instructions of overhead.
 *
 * The loop runs four times.  `$v1` starts at 4 and the branch targets the decrement,
 * so the decrement is the *first* instruction of the body: the body runs with `$v1`
 * at 3, 2, 1 and 0, and `bgez` finally fails on the pass that would make it -1.
 * Putting the label after the decrement instead - the obvious arrangement - runs it
 * five times and shifts the branch by one instruction.
 */
#include "types.h"

/* The header's address, as `lui 0x7` + `addiu -0x78D8` builds it. */
#define HEADER_HI 0x7
#define HEADER_LO (-0x78D8)

/** The zeroed pair just below the header. */
#define ZEROED_HI 0x7
#define ZEROED_LO (-0x7908)

/** Initialise four 0xFC-byte records at 0x6C770 and the 0x48-byte header that
 *  precedes them, then zero the two globals at 0x6C728 and 0x6C6F8. */
__attribute__((noreturn)) void func_0014EBA4(void) {
    __asm__ __volatile__(
        "lui   $a1, %[hhi]\n\t"
        "addiu $t6, $a1, %[hlo]\n\t"
        "addiu $v0, $zero, 0x1E0\n\t"
        "addiu $a0, $zero, -0x1\n\t"
        "sw    $v0, 0x38($t6)\n\t"
        "addiu $a3, $zero, 0x1\n\t"
        "addiu $a2, $zero, 0x110\n\t"
        "sw    $a0, 0x8($t6)\n\t"
        "addiu $t5, $zero, 0x4\n\t"
        "addiu $t4, $zero, 0x64\n\t"
        "sw    $v0, 0x24($t6)\n\t"
        "addiu $t3, $zero, 0x1E0\n\t"
        "addiu $t2, $zero, 0x110\n\t"
        "sw    $zero, 0x28($t6)\n\t"
        "ori   $t1, $zero, 0xFFFF\n\t"
        "addiu $t0, $zero, -0x1\n\t"
        "sw    $zero, 0x2C($t6)\n\t"
        "addiu $v1, $zero, 0x4\n\t"
        "sw    $zero, 0x30($t6)\n\t"
        "sw    $zero, 0x34($t6)\n\t"
        "sw    $zero, 0x4($t6)\n\t"
        "sw    $zero, 0xC($t6)\n\t"
        "sw    $zero, 0x44($t6)\n\t"
        "sw    $zero, 0x40($t6)\n\t"
        "sw    $zero, %[hlo]($a1)\n\t"
        "addiu $a1, $t6, 0x48\n\t"
        "sw    $a3, 0x20($t6)\n\t"
        "addiu $a3, $zero, 0x1\n\t"
        "sw    $a2, 0x3C($t6)\n\t"
        "addiu $a2, $t6, 0x140\n\t"
        /* The loop label goes *before* the counter decrement, not after it: the
         * branch targets the `addiu $v1, $v1, -1`, so the decrement is the first
         * instruction of the body and runs four times rather than five. */
        "1:\n\t"
        "addiu $v1, $v1, -0x1\n\t"
        "sw    $t5, 0x0($a1)\n\t"
        "sw    $zero, 0x74($a1)\n\t"
        "sw    $t4, 0x10($a1)\n\t"
        "sw    $zero, 0x14($a1)\n\t"
        "sw    $a3, 0x18($a1)\n\t"
        "sw    $t3, 0x1C($a1)\n\t"
        "sw    $t2, 0x20($a1)\n\t"
        "sw    $zero, 0x24($a1)\n\t"
        "sw    $zero, 0x28($a1)\n\t"
        "sw    $zero, 0x2C($a1)\n\t"
        "sw    $zero, 0x30($a1)\n\t"
        "sw    $zero, 0x34($a1)\n\t"
        "sw    $zero, 0x54($a1)\n\t"
        "sw    $a3, 0x58($a1)\n\t"
        "sw    $zero, 0x5C($a1)\n\t"
        "sw    $zero, 0x38($a1)\n\t"
        "sw    $zero, 0x3C($a1)\n\t"
        "sw    $zero, 0x40($a1)\n\t"
        "sw    $zero, 0x44($a1)\n\t"
        "sw    $zero, 0x48($a1)\n\t"
        "sw    $zero, 0x4C($a1)\n\t"
        "sw    $t1, 0x50($a1)\n\t"
        "sw    $zero, 0x60($a1)\n\t"
        "sw    $zero, 0x68($a1)\n\t"
        "sw    $zero, 0x64($a1)\n\t"
        "sw    $zero, 0x6C($a1)\n\t"
        "sw    $zero, 0x70($a1)\n\t"
        "addiu $a1, $a1, 0xFC\n\t"
        "sw    $t0, 0x0($a2)\n\t"
        ".set noreorder\n\t"
        "bgez  $v1, 1b\n\t"
        "addiu $a2, $a2, 0xFC\n\t"
        ".set reorder\n\t"
        "lui   $v1, %[zhi]\n\t"
        "addiu $t0, $v1, %[zlo]\n\t"
        "sw    $zero, %[zlo]($v1)\n\t"
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "sw    $zero, 0x4($t0)\n\t"
        ".set reorder\n\t"
        : : [hhi] "i" (HEADER_HI), [hlo] "i" (HEADER_LO),
            [zhi] "i" (ZEROED_HI), [zlo] "i" (ZEROED_LO)
        : "memory", "$a0", "$a1", "$a2", "$a3", "$v0", "$v1",
          "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6");
}