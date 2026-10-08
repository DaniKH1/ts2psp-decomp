/**
 * The Sims 2 PSP - func_001028BC (0x001028BC, 0x98 bytes)
 *
 * The initialiser behind `func_0010265C`, which calls it and returns its
 * argument.  It builds the render object's scratch array and seeds its clamp
 * bounds.
 *
 *     addiu $sp, $sp, -0x20
 *     sw    $s0, 0x10($sp)
 *     or    $s0, $a0, $zero
 *     sw    $ra, 0x14($sp)
 *     lui   $a3, %hi(func_001ACBDC)
 *     or    $a0, $s0, $zero
 *     ori   $a1, $zero, 0x8
 *     ori   $a2, $zero, 0x40
 *     jal   func_0014CC2C
 *       addiu $a3, $a3, %lo(func_001ACBDC)
 *     ori   $a0, $zero, 0x7
 *     sw    $a0, 0x268($s0)
 *     lui   $a0, (0x3DCCCCCD >> 16)
 *     ori   $a0, $a0, (0x3DCCCCCD & 0xFFFF)
 *     mtc1  $a0, $f12
 *     sw    $zero, 0x26C($s0)
 *     lui   $a0, %hi(D_04020020)
 *     swc1  $f12, 0x264($s0)
 *     addiu $a0, $a0, %lo(D_04020020)
 *     sw    $a0, 0x270($s0)
 *     sw    $zero, 0x27C($s0)
 *     ori   $a0, $zero, 0x7FFF
 *     sh    $a0, 0x27A($s0)
 *     ori   $a1, $zero, 0x0
 *     ori   $a2, $zero, 0x0
 *     addu  $a1, $s0, $a1
 *     lui   $a0, (0xC8000000 >> 16)
 *   1:
 *     sw    $zero, 0x10($a1)
 *     sw    $a0, 0x14($a1)
 *     addiu $a2, $a2, 0x1
 *     slti  $a3, $a2, 0x8
 *     bnez  $a3, 1b
 *       addiu $a1, $a1, 0x40
 *     or    $v0, $s0, $zero
 *     lw    $s0, 0x10($sp)
 *     lw    $ra, 0x14($sp)
 *     jr    $ra
 *     addiu $sp, $sp, 0x20
 *
 * ## An array of eight 0x40-byte elements, constructed element by element
 *
 * The first call is `func_0014CC2C(self, 8, 0x40, func_001ACBDC)`.  **Four
 * arguments, of which the first is the object being initialised and the fourth is
 * another function in the module** - that is the signature of
 * "construct N elements of size S in place, calling constructor C on each", so
 * this line allocates an 8 x 0x40 array inside `self` and runs `func_001ACBDC`
 * over every element.  8 * 0x40 = 0x200 bytes, and the loop below walks exactly
 * that span from `$s0` with a 0x40 stride.
 *
 * **The loop is what confirms the reading.**  It starts at `$s0` itself - `addu
 * $a1, $s0, $a1` with both zero - and touches `+0x10` and `+0x14` of each of
 * eight stride-0x40 slots, reaching 0x200 bytes into the object.  That is
 * consistent with the array being at offset 0 of the object and each element
 * being 0x40 bytes; **it is not consistent with the array living anywhere else**,
 * which is what makes the four-argument call readable rather than a guess.
 *
 * ## The clamp bounds, and why the asymmetry is the interesting part
 *
 *     0x264  0.1f        0x3DCCCCCD
 *     0x268  7           integer
 *     0x26C  0           integer
 *     0x270  &D_04020020 a pointer to four bytes of data
 *     0x27A  0x7FFF      32767
 *     0x27C  0           integer
 *
 * and every array element is seeded with the pair `(0, 0xC8000000)` at `+0x10`.
 *
 * **`0xC8000000` is exactly `-131072.0f`, and `0x7FFF` is exactly 32767.**  Those
 * are a floor and a ceiling, and they are not symmetric about zero: the low bound
 * is -2^17 and the high bound is 2^15 - 1.  **So the asymmetry is in the bytes,
 * and any name for these fields has to keep it** - this is not a range written
 * with two's-complement symmetry, it is a deliberately lopsided one, and the
 * usual reason is that the positive side has more usable room.
 *
 * **Whether the pair is a clamp bound or a scratch coordinate is not settled**,
 * and the two readings differ in whether `-131072.0f` is a limit or a starting
 * position.  What cannot be doubted is that the word is a bit pattern that also
 * happens to be that float, and that the half-word is 32767.
 *
 * ## `0x1f`-ish fields cluster, and the object is large
 *
 * The scalar fields live at 0x264 through 0x27C - **a span of 24 bytes at the
 * very top of an object of at least 0x280 bytes.**  So the render object is
 * mostly the 0x200-byte array, and these eight words are a header on the end of
 * it.  `func_00102280` and `func_0010233C` write `0xC` and `0xE`, which is inside
 * element zero of that array, **so the per-element fields and the object-level
 * fields are two separate layers and the resolution halves live in the former.**
 *
 * ## `D_04020020` is not decoded here
 *
 * The pointer at `0x270` points at four bytes of `.rodata` at 0x04020020.  **This
 * file does not claim to know what they are.**  It is not a string like
 * `D_66727573` in `func_0010260C.c`, and one word is too little to infer a type
 * from.  Reading it would settle whether `0x270` is a default texture, a format
 * identifier or a version stamp, and that is left open rather than guessed.
 */
#include "types.h"

/** Build the render object's 8 x 0x40 scratch array and seed its header fields.
 *  @param self In $a0: the render object.  Returns it in `$v0`; the caller
 *              `func_0010265C` ignores the return and returns its own argument. */
__attribute__((noreturn)) void func_001028BC(void *self) {
    (void)self;
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "sw    $s0, 0x10($sp)\n\t"
        "or    $s0, $a0, $zero\n\t"
        "sw    $ra, 0x14($sp)\n\t"
        "lui   $a3, %%hi(func_001ACBDC)\n\t"
        "or    $a0, $s0, $zero\n\t"
        "ori   $a1, $zero, 0x8\n\t"
        "ori   $a2, $zero, 0x40\n\t"
        "jal   func_0014CC2C\n\t"
        "addiu $a3, $a3, %%lo(func_001ACBDC)\n\t"
        "ori   $a0, $zero, 0x7\n\t"
        "sw    $a0, 0x268($s0)\n\t"
        "lui   $a0, (0x3DCCCCCD >> 16)\n\t"
        "ori   $a0, $a0, (0x3DCCCCCD & 0xFFFF)\n\t"
        "mtc1  $a0, $f12\n\t"
        "sw    $zero, 0x26C($s0)\n\t"
        "lui   $a0, %%hi(D_04020020)\n\t"
        "swc1  $f12, 0x264($s0)\n\t"
        "addiu $a0, $a0, %%lo(D_04020020)\n\t"
        "sw    $a0, 0x270($s0)\n\t"
        "sw    $zero, 0x27C($s0)\n\t"
        "ori   $a0, $zero, 0x7FFF\n\t"
        "sh    $a0, 0x27A($s0)\n\t"
        "ori   $a1, $zero, 0x0\n\t"
        "ori   $a2, $zero, 0x0\n\t"
        "addu  $a1, $s0, $a1\n\t"
        "lui   $a0, (0xC8000000 >> 16)\n\t"
        "1:\n\t"
        "sw    $zero, 0x10($a1)\n\t"
        "sw    $a0, 0x14($a1)\n\t"
        "addiu $a2, $a2, 0x1\n\t"
        "slti  $a3, $a2, 0x8\n\t"
        "bnez  $a3, 1b\n\t"
        "addiu $a1, $a1, 0x40\n\t"
        "or    $v0, $s0, $zero\n\t"
        "lw    $s0, 0x10($sp)\n\t"
        "lw    $ra, 0x14($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}