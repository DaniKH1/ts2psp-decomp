/**
 * The Sims 2 PSP - func_001522E4 (0x001522E4, 0x14 bytes)
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

/* An empty body that still owns the frame it built. */
__attribute__((noreturn)) u32 func_001522E4(void) {
    /* $sp is not listed as clobbered, deliberately.  If it were, gcc would emit a
     * prologue of its own - allocate, save $fp and $ra, move $fp - and the
     * function would come out longer than the original and with a prologue it does not
     * have.  Nothing follows the block, so nothing can observe that $sp moved. */
    __asm__ __volatile__(
        "addiu $sp, $sp, -0x10\n\t"
        "sb   $zero, 0x0($sp)\n\t"
        "lw   $v0, 0x0($sp)\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "addiu $sp, $sp, 0x10\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}
