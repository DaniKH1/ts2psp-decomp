/**
 * The Sims 2 PSP - drawing_042C (0x1BB6AC, 0x54 bytes)
 *
 *     addiu $sp, $sp, -0x30
 *     sw $s0, 0x20($sp)
 *     or $s0, $a0, $zero
 *     addiu $a0, $sp, 0x10
 *     addiu $a1, $sp, 0x14
 *     addiu $a2, $sp, 0x18
 *     sw $ra, 0x24($sp)
 *     jal func_00101B08
 *     addiu $a3, $sp, 0x1C
 *     lw $a0, 0x18($sp)
 *     lw $a1, 0x1C($sp)
 *     mtc1 $a0, $f12
 *     cvt.s.w $f12, $f12
 *     mtc1 $a1, $f13
 *     cvt.s.w $f13, $f13
 *     swc1 $f12, 0x0($s0)
 *     swc1 $f13, 0x4($s0)
 *     lw $s0, 0x20($sp)
 *     lw $ra, 0x24($sp)
 *     jr $ra
 *     addiu $sp, $sp, 0x30
 *
 * drawing: call func_00101B08 with four pointers into this frame and use the two it writes back.
 */

#include "types.h"

__attribute__((noreturn)) void drawing_042C(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x30\n\t"
        "sw $s0, 0x20($sp)\n\t"
        "or $s0, $a0, $zero\n\t"
        "addiu $a0, $sp, 0x10\n\t"
        "addiu $a1, $sp, 0x14\n\t"
        "addiu $a2, $sp, 0x18\n\t"
        "sw $ra, 0x24($sp)\n\t"
        "jal func_00101B08\n\t"
        "addiu $a3, $sp, 0x1C\n\t"
        "lw $a0, 0x18($sp)\n\t"
        "lw $a1, 0x1C($sp)\n\t"
        "mtc1 $a0, $f12\n\t"
        "cvt.s.w $f12, $f12\n\t"
        "mtc1 $a1, $f13\n\t"
        "cvt.s.w $f13, $f13\n\t"
        "swc1 $f12, 0x0($s0)\n\t"
        "swc1 $f13, 0x4($s0)\n\t"
        "lw $s0, 0x20($sp)\n\t"
        "lw $ra, 0x24($sp)\n\t"
        "jr $ra\n\t"
        "addiu $sp, $sp, 0x30\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
