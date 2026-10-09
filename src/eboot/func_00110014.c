/**
 * The Sims 2 PSP - func_00110014 (0x00110014, 0x28 bytes)
 *
 *     mtc1       $a1, $f12
 *     ori        $a2, $zero, 0x3
 *     cvt.s.w    $f12, $f12
 *     lw         $a1, 0x8($a0)
 *     sw         $a2, 0x0($a1)
 *     swc1       $f12, 0x4($a1)
 *     lw         $a1, 0x8($a0)
 *     addiu      $a1, $a1, 0x8
 *     jr         $ra
 *       sw        $a1, 0x8($a0)
 *
 * Pushes one entry onto a growable array held at 0x8($a0): the word 3
 * followed by $a1 converted from int to float.  Then it advances the
 * array pointer by 8 - one word plus one float, a 4-byte-aligned
 * element with no padding.
 *
 * **The pointer is reloaded rather than kept.**  The second `lw $a1,
 * 0x8($a0)` re-reads the field that `swc1` and `sw` have just written
 * through, and the store back sits in the delay slot of the `jr`, so
 * this is tail-call-shaped: no frame is pushed because nothing needs
 * one.
 *
 * `mtc1`/`cvt.s.w` between two other instructions is the ordinary
 * int-to-float pair - the `mtc1` must land a move-delay slot before the
 * `cvt`, and here the `ori` fills it.
 */
#include "types.h"

__attribute__((noreturn)) void func_00110014(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "mtc1    $a1, $f12\n\t"
        "ori     $a2, $zero, 0x3\n\t"
        "cvt.s.w $f12, $f12\n\t"
        "lw      $a1, 0x8($a0)\n\t"
        "sw      $a2, 0x0($a1)\n\t"
        "swc1    $f12, 0x4($a1)\n\t"
        "lw      $a1, 0x8($a0)\n\t"
        "addiu   $a1, $a1, 0x8\n\t"
        "jr      $ra\n\t"
        "sw      $a1, 0x8($a0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}