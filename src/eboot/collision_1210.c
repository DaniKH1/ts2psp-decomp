/**
 * The Sims 2 PSP - collision_1210 (0x001B1A30, 0xC0 bytes)
 *
 * A sign-only, branchless overlap test between two 12-halfword volumes in the
 * first argument and an 8-halfword volume in the second.
 *
 *     addiu $a2, $a1, 0x8
 *     lh    $a3, 0x4($a0)      ; a[2]
 *     lh    $t0, 0x4($a2)      ; b[6]
 *     addiu $t1, $a0, 0xC      ; &a[6]
 *     lh    $t2, 0x0($a1)      ; b[0]
 *     lh    $t3, 0x0($t1)      ; a[6]
 *     subu  $a3, $a3, $t0      ; a[2] - b[6]
 *     lh    $v0, 0x4($a1)      ; b[2]
 *     subu  $t3, $t2, $t3      ; b[0] - a[6]
 *     lh    $v1, 0x4($t1)      ; a[8]
 *     and   $a3, $a3, $t3
 *     lh    $t3, 0x0($a0)      ; a[0]
 *     lh    $t4, 0x0($a2)      ; b[4]
 *     lh    $t5, 0x2($a0)      ; a[1]
 *     subu  $v1, $v0, $v1      ; b[2] - a[8]
 *     lh    $a2, 0x2($a2)      ; b[5]
 *     and   $a3, $a3, $v1
 *     subu  $v1, $t5, $a2      ; a[1] - b[5]
 *     lh    $a1, 0x2($a1)      ; b[1]
 *     lh    $t5, 0x2($t1)      ; a[7]
 *     subu  $t3, $t3, $t4      ; a[0] - b[4]
 *     and   $t3, $t3, $v1
 *     subu  $t5, $a1, $t5      ; b[1] - a[7]
 *     lh    $v1, 0xA($a0)      ; a[5]
 *     and   $t3, $t3, $t5
 *     lh    $t6, 0x6($t1)      ; a[9]
 *     and   $a3, $a3, $t3
 *     subu  $t0, $v1, $t0      ; a[5] - b[6]
 *     lh    $t3, 0xA($t1)      ; a[11]
 *     lh    $v1, 0x6($a0)      ; a[3]
 *     lh    $a0, 0x8($a0)      ; a[4]
 *     subu  $t2, $t2, $t6      ; b[0] - a[9]
 *     and   $t0, $t0, $t2
 *     subu  $a0, $a0, $a2      ; a[4] - b[5]
 *     subu  $t2, $v0, $t3      ; b[2] - a[11]
 *     lh    $a2, 0x8($t1)      ; a[10]
 *     subu  $t3, $v1, $t4      ; a[3] - b[4]
 *     and   $a0, $t3, $a0
 *     subu  $a1, $a1, $a2      ; b[1] - a[10]
 *     and   $t0, $t0, $t2
 *     and   $a0, $a0, $a1
 *     and   $a0, $t0, $a0
 *     sra   $a1, $a3, 31
 *     sra   $a0, $a0, 31
 *     andi  $v0, $a1, 0x1
 *     andi  $a0, $a0, 0x2
 *     jr    $ra
 *     or    $v0, $v0, $a0
 *
 * **What is provable: two sign products and a two-bit answer.**  Writing the
 * first argument's twelve signed halfwords as `a[0..11]` and the second's eight as
 * `b[0..7]` (note the gap: `b[3]` is never read, and `a2 = b + 8` makes `b[4]`
 * onward reachable as one group):
 *
 *     s1 = (a[0]-b[4]) & (a[1]-b[5]) & (a[2]-b[6])
 *        & (b[0]-a[6]) & (b[1]-a[7]) & (b[2]-a[8])
 *
 *     s2 = (a[3]-b[4]) & (a[4]-b[5]) & (a[5]-b[6])
 *        & (b[0]-a[9]) & (b[1]-a[10]) & (b[2]-a[11])
 *
 *     return ((s1 >> 31) & 1) | ((s2 >> 31) & 2)
 *
 * Each `s1` is the sign of the product of three differences taken against one
 * group and three taken the other way round, so its sign bit is the parity of the
 * six comparisons - which is exactly what an orientation or interval-overlap
 * decision needs and no multiply.  There is no multiply in the function at all,
 * and no branch but the return: the two answers are folded into one word as
 * bits 0 and 1, giving 0, 1, 2 or 3.
 *
 * **So there are two volumes being tested against one,** `a[0..2]` against
 * `a[6..8]` and `a[3..5]` against `a[9..11]`, both against `b[0..2]`/`b[4..6]`,
 * and the result says which of the two overlapped.  That the function answers a
 * question about *two* candidates in one call is what the caller's jump table
 * confirms: `collision_0C18` is the only caller, it takes `$a0 = this` and
 * `$a1` from the stack, and it does `sltiu $a1, $a0, 0x16` on
 * `lbu($s4+0x1C) | result` - a **22-way jump table**, into which a two-bit code
 * and a byte-sized field are OR'd together.  A one-bit "collided" answer would be
 * redundant with that byte; a two-bit one is not.
 *
 * **What is not settled here: which end of each interval is the low one.**  The
 * code only ever asks for the *sign* of six differences, and the answer flips
 * meaning depending on which of `a[0]`/`a[6]` is the minimum.  Nothing in the
 * function says, and the one caller is 752 bytes of jump table that would have to
 * be read to settle it.  So the comment above gives the arithmetic exactly and
 * stops there, which is the only claim the bytes support.
 *
 * **The scheduling is interleaved for a reason that is worth recording.**  Each
 * load is issued as early as the register pressure allows and consumed by an `and`
 * several instructions later, so no load waits on the subtraction before the next
 * one starts; the two chains (`$a3` and `$t0`/`$a0`) run in parallel and only meet
 * at the `and $a0, $t0, $a0`.  The registers are recycled hard - `$a2` is a pointer
 * for the first four loads and then becomes `b[5]`, `$a0` is the object for
 * twenty instructions and then becomes `a[4]` - which is why this reads as noise
 * until the sixteen loads are sorted by offset.
 */
#include "types.h"

/** @param a0 Twelve signed halfwords: two volumes, `a[0..2]` against `a[6..8]` and
 *         `a[3..5]` against `a[9..11]`.
 *  @param a1 Eight signed halfwords: one volume, `b[0..2]` and `b[4..6]`.
 *  @return A two-bit code, bit 0 from the first volume and bit 1 from the second. */
__attribute__((noreturn)) u32 collision_1210(void *a0, void *a1) {
    (void)a0;
    (void)a1;
    __asm__ __volatile__(
        "addiu $a2, $a1, 0x8\n\t"
        "lh    $a3, 0x4($a0)\n\t"
        "lh    $t0, 0x4($a2)\n\t"
        "addiu $t1, $a0, 0xC\n\t"
        "lh    $t2, 0x0($a1)\n\t"
        "lh    $t3, 0x0($t1)\n\t"
        "subu  $a3, $a3, $t0\n\t"
        "lh    $v0, 0x4($a1)\n\t"
        "subu  $t3, $t2, $t3\n\t"
        "lh    $v1, 0x4($t1)\n\t"
        "and   $a3, $a3, $t3\n\t"
        "lh    $t3, 0x0($a0)\n\t"
        "lh    $t4, 0x0($a2)\n\t"
        "lh    $t5, 0x2($a0)\n\t"
        "subu  $v1, $v0, $v1\n\t"
        "lh    $a2, 0x2($a2)\n\t"
        "and   $a3, $a3, $v1\n\t"
        "subu  $v1, $t5, $a2\n\t"
        "lh    $a1, 0x2($a1)\n\t"
        "lh    $t5, 0x2($t1)\n\t"
        "subu  $t3, $t3, $t4\n\t"
        "and   $t3, $t3, $v1\n\t"
        "subu  $t5, $a1, $t5\n\t"
        "lh    $v1, 0xA($a0)\n\t"
        "and   $t3, $t3, $t5\n\t"
        "lh    $t6, 0x6($t1)\n\t"
        "and   $a3, $a3, $t3\n\t"
        "subu  $t0, $v1, $t0\n\t"
        "lh    $t3, 0xA($t1)\n\t"
        "lh    $v1, 0x6($a0)\n\t"
        "lh    $a0, 0x8($a0)\n\t"
        "subu  $t2, $t2, $t6\n\t"
        "and   $t0, $t0, $t2\n\t"
        "subu  $a0, $a0, $a2\n\t"
        "subu  $t2, $v0, $t3\n\t"
        "lh    $a2, 0x8($t1)\n\t"
        "subu  $t3, $v1, $t4\n\t"
        "and   $a0, $t3, $a0\n\t"
        "subu  $a1, $a1, $a2\n\t"
        "and   $t0, $t0, $t2\n\t"
        "and   $a0, $a0, $a1\n\t"
        "and   $a0, $t0, $a0\n\t"
        "sra   $a1, $a3, 31\n\t"
        "sra   $a0, $a0, 31\n\t"
        "andi  $v0, $a1, 0x1\n\t"
        "andi  $a0, $a0, 0x2\n\t"
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "or    $v0, $v0, $a0\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$a0", "$a1", "$a2", "$a3",
          "$v0", "$v1", "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6");
}