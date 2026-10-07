/**
 * The Sims 2 PSP - func_0013B2D0 (0x0013B2D0, 0x38 bytes)
 *
 * Fills four caller's out-parameters and returns 0.
 *
 *     lui   $t0, 0x1E
 *     lw   $t0, 0x2E8($t0)        value A, out of the global at 0x1E02E8
 *     lui  $t1, 0x1E
 *     sw   $t0, 0x0($a0)          *out1 = A
 *     lw   $a0, 0x2EC($t1)        value B, out of the global at 0x1E02EC
 *     lui  $t0, 0x1E
 *     sw   $a0, 0x0($a1)          *out2 = B
 *     addiu $a0, $t0, 0x2F0       the address 0x1E02F0
 *     sw   $a0, 0x0($a2)          *out3 = &that
 *     lui  $a0, 0x1E
 *     addiu $a0, $a0, 0x12F0      the address 0x1E12F0
 *     sw   $a0, 0x0($a3)          *out4 = &that
 *     jr   $ra
 *     move $v0, $zero             return 0
 *
 * **Two of the four outputs are addresses and two are values.**  0x1E02F0 and
 * 0x1E12F0 are passed out as pointers; 0x1E02E8 and 0x1E02EC are dereferenced and
 * passed out as words.  Those two pairs are 4 and 4 + 0x1000 bytes apart
 * respectively, so this is handing back one tightly packed group of four words and
 * two things 4 KB apart - two different tables being exposed together, not one.
 *
 * **The return is a constant 0.**  A function that fills out-parameters and always
 * returns 0 is either a getter pair that cannot fail or a `void` with a return
 * value the author left behind.  There is no branch and no test, so there is
 * nothing to fail, and the first reading is the more useful one: the caller sees a
 * status it can ignore.
 *
 * This is the first function here that is *entirely* out-parameters - it reads none
 * of its four arguments and writes to all of them.  That is why `$a0` is
 * overwritten mid-body at instruction 5: the register is free once the first output
 * has been stored, so the compiler reuses it rather than keeping a fifth register.
 * Which also means the four pointers are the whole interface, and the return value
 * is decoration.
 *
 * 0x1E02F0 is four bytes past the global at 0x1E02EC that it just read, so the word
 * at 0x1E02F0 is a fourth entry of the same table whose *value* this function hands
 * out by address rather than by value.  Reading a table element and handing back a
 * pointer to it are two different ways of exposing one table, and this does both in
 * four instructions.
 */
#include "types.h"

/* The two globals read by value. */
#define TABLE_A   0x0001E02E8u
#define TABLE_B   0x0001E02ECu

/* The two globals handed out by address. */
#define OUT_C     0x0001E02F0u
#define OUT_D     0x0001E12F0u

s32 func_0013B2D0(void **out_a, void **out_b, void **out_c, void **out_d) {
    /* `$a0` is the first output, then a scratch, then the third, then the fourth -
     * so `$a0` is in-out and the rest are inputs. */
    register void **out1 asm("$a0") = out_a;
    register void **out3 asm("$a2") = out_c;
    register void **out4 asm("$a3") = out_d;
    register u32 page_a asm("$t0");
    register u32 page_b asm("$t1");

    __asm__ __volatile__(
        "lui   %[pa], 0x1E\n\t"
        "lw    %[pa], 0x2E8(%[pa])\n\t"
        "lui   %[pb], 0x1E\n\t"
        "sw    %[pa], 0x0(%[o1])\n\t"
        "lw    %[o1], 0x2EC(%[pb])\n\t"
        "lui   %[pa], 0x1E\n\t"
        "sw    %[o1], 0x0(%[o2])\n\t"
        "addiu %[o1], %[pa], 0x2F0\n\t"
        "sw    %[o1], 0x0(%[o3])\n\t"
        "lui   %[o1], 0x1E\n\t"
        "addiu %[o1], %[o1], 0x12F0\n\t"
        "sw    %[o1], 0x0(%[o4])\n\t"
        : [pa] "=&r"(page_a), [pb] "=&r"(page_b), [o1] "+&r"(out1)
        : [o2] "r"(out_b), [o3] "r"(out3), [o4] "r"(out4)
        : "memory", "hi", "lo");

    /* The constant zero return is left to C so the `move` lands in the delay
     * slot.  Nothing the block wrote is needed afterwards, so there is no risk of
     * the compiler hoisting this above the stores. */
    return 0;
}