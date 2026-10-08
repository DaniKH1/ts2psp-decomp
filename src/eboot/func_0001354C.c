/**
 * The Sims 2 PSP - func_0001354C (0x0001354C, 0x1C bytes)
 *
 * Standard prologue/epilogue with a function call.
 *
 *     addiu $sp, $sp, -0x20
 *     sw    $ra, 0x10($sp)
 *     jal   func_0001352C
 *     lw    $a0, 0x128($a0)
 *     lw    $ra, 0x10($sp)
 *     jr    $ra
 *     addiu $sp, $sp, 0x20
 *
 * **Calls func_0001352C with modified $a0, returns its result.**
 * Pattern from func_0000BEC0 and func_00025594: whole body in asm,
 * jr $ra inside block, delay slot on jr is the stack restore.
 * .set noreorder around jal to keep the lw in its delay slot.
 * .set noreorder around jr to keep the addiu in its delay slot.
 */
#include "types.h"

__attribute__((noreturn)) u32 func_0001354C(void) {
    __asm__ __volatile__(
        "addiu $sp, $sp, -0x20\n\t"
        "sw    $ra, 0x10($sp)\n\t"
        ".set noreorder\n\t"
        "jal   func_0001352C\n\t"
        "lw    $a0, 0x128($a0)\n\t"
        ".set reorder\n\t"
        "lw    $ra, 0x10($sp)\n\t"
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}