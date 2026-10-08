/**
 * The Sims 2 PSP - func_0010233C (0x0010233C, 0x64 bytes)
 *
 *     addiu $sp, $sp, -0x10
 *     lui   $a0, %hi(sym_001DB014)
 *     addiu $a0, $a0, %lo(sym_001DB014)
 *     lbu   $a0, 0x2D($a0)
 *     sw    $ra, 0x0($sp)
 *     beqz  $a0, 1f
 *     nop
 *     jal   func_00101AE8
 *     nop
 *     lwc1  $f12, 0x0($v0)
 *     lwc1  $f13, 0x4($v0)
 *     trunc.w.s $f12, $f12
 *     lui   $a0, %hi(sym_001DB12C)
 *     trunc.w.s $f13, $f13
 *     lw    $a1, %lo(sym_001DB12C)($a0)
 *     mfc1  $a2, $f12
 *     sra   $a2, $a2, 1
 *     mfc1  $a3, $f13
 *     sh    $a2, 0xC($a1)
 *     lw    $a0, %lo(sym_001DB12C)($a0)
 *     sra   $a3, $a3, 1
 *     sh    $a3, 0xE($a0)
 *   1:
 *     lw    $ra, 0x0($sp)
 *     jr    $ra
 *     addiu $sp, $sp, 0x10
 *
 * **If the byte at `sym_001DB014`+`0x2D` is set, take two floats from
 * `func_00101AE8`'s result, truncate and halve them, and store them as two
 * 16-bit fields at offsets `0xC` and `0xE` of the render object.**
 *
 *     if (flag) { p = get_size(); render->half_w = (s16)(trunc(p->x) >> 1);
 *                            render->half_h = (s16)(trunc(p->y) >> 1); }
 *
 * ## Offsets 0xC and 0xE are a pair, and this function is the evidence
 *
 * Two consecutive `sh` at `0xC` and `0xE`, both fed a truncated float that has
 * just been halved the same way, **is a half-resolution width and height stored
 * as two signed 16-bit halves of a 32-bit field.**  That is much stronger than
 * the "half-width field" reading that `func_00102280.c` could only offer on its
 * own - it writes `0xE` and cannot see its partner, whereas this function writes
 * both and reads them from one call.
 *
 * **So the pair at 0xC/0xE is a resolution at half scale, and both writers agree
 * on the transform: truncate to integer, then arithmetic-shift right by one.**
 * `func_00102280` halves a field it reads from elsewhere; this one halves a float
 * it just truncated.
 *
 * ## `trunc.w.s` before the shift changes what the shift means
 *
 * The order is truncate-then-halve, not halve-then-truncate, and for a negative
 * value those differ:
 *
 *     trunc(-3.7) = -3,  -3 >> 1 = -2
 *     (-3.7 / 2)  = -1.85, truncated on store = -1
 *
 * **The instruction sequence therefore rounds negative odd sizes toward negative
 * infinity twice over** - once at the truncation and once at the shift - and a
 * size of `-1` or `-2` would come out of a half-width field.  `trunc.w.s` is a
 * trap on out-of-range input, which is why the result is a signed half-word and
 * not an unsigned one; **whether the engine ever passes a negative dimension is
 * not something these bytes answer.**
 *
 * ## Why the object pointer is reloaded between the two stores
 *
 * `lw $a1, %lo(sym_001DB12C)($a0)` for the first store, then `lw $a0,
 * %lo(sym_001DB12C)($a0)` for the second - **the global is read twice from the
 * same base register**, because `$a0` has to survive as the address for the
 * reload while `$a1` carries the value.  Reading `sym_001DB12C` twice when it
 * cannot have changed is the same defensive pattern as in `func_00102280.c`, and
 * **in both cases it is what lets the second store use a fresh base register
 * without having to shuffle `$a1`.**
 */
#include "types.h"

/** If `sym_001DB014`'s byte at `0x2D` is set, store the halved, truncated
 *  dimensions from `func_00101AE8` at offsets `0xC` and `0xE` of the object at
 *  `sym_001DB12C`. */
__attribute__((noreturn)) void func_0010233C(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x10\n\t"
        "lui   $a0, %%hi(sym_001DB014)\n\t"
        "addiu $a0, $a0, %%lo(sym_001DB014)\n\t"
        "lbu   $a0, 0x2D($a0)\n\t"
        "sw    $ra, 0x0($sp)\n\t"
        "beqz  $a0, 1f\n\t"
        "nop\n\t"
        "jal   func_00101AE8\n\t"
        "nop\n\t"
        "lwc1  $f12, 0x0($v0)\n\t"
        "lwc1  $f13, 0x4($v0)\n\t"
        "trunc.w.s $f12, $f12\n\t"
        "lui   $a0, %%hi(sym_001DB12C)\n\t"
        "trunc.w.s $f13, $f13\n\t"
        "lw    $a1, %%lo(sym_001DB12C)($a0)\n\t"
        "mfc1  $a2, $f12\n\t"
        "sra   $a2, $a2, 1\n\t"
        "mfc1  $a3, $f13\n\t"
        "sh    $a2, 0xC($a1)\n\t"
        "lw    $a0, %%lo(sym_001DB12C)($a0)\n\t"
        "sra   $a3, $a3, 1\n\t"
        "sh    $a3, 0xE($a0)\n\t"
        "1:\n\t"
        "lw    $ra, 0x0($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x10\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}