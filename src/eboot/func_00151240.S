/**
 * The Sims 2 PSP - func_00151240 (0x00151240, 0x14 bytes)
 *
 * Stores a zero byte on the stack, reads it back as a word, and returns it.
 *
 *     addiu $sp, $sp, -0x10
 *     sb   $zero, 0x0($sp)
 *     lw   $v0, 0x0($sp)
 *     jr   $ra
 *     addiu $sp, $sp, 0x10
 *
 * **Stores one zero byte, reads back as a word.**  The upper 3 bytes
 * are whatever was on the stack. All 64 functions with this shape
 * exhibit the same behavior.
 */

#include "types.h"

.global func_00151240
.type func_00151240, @function

func_00151240:
    addiu $sp, $sp, -0x10
    sb   $zero, 0x0($sp)
    lw   $v0, 0x0($sp)
    jr   $ra
    addiu $sp, $sp, 0x10