/**
 * The Sims 2 PSP - func_000133F0 (0x000133F0, 0x1C bytes)
 *
 * Standard prologue/epilogue with a function call and extra load.
 *
 *     addiu $sp, $sp, -0x20
 *     sw    $ra, 0x10($sp)
 *     jal   func_00073B78
 *     lw    $a0, 0x128($a0)
 *     lw    $v0, 0x18($v0)
 *     lw    $ra, 0x10($sp)
 *     jr    $ra
 *     addiu $sp, $sp, 0x20
 *
 * **Calls func_00073B78 with modified $a0, loads a word from result (offset 0x18).**
 * Pattern from func_0000BEC0 and func_00025594: whole body in asm,
 * jr $ra inside block, delay slot on jr is the stack restore.
 * .set noreorder around jal to keep the lw in its delay slot.
 * .set noreorder around jr to keep the addiu in its delay slot.
 */
#include "types.h"

__attribute__((noreturn)) u32 func_000133F0(void) {
    __asm__ __volatile__(
        "addiu $sp, $sp, -0x20\n\t"
        "sw    $ra, 0x10($sp)\n\t"
        ".set noreorder\n\t"
        "jal   func_00073B78\n\t"
        "lw    $a0, 0x128($a0)\n\t"
        ".set reorder\n\t"
        "lw    $v0, 0x18($v0)\n\t"
        "lw    $ra, 0x10($sp)\n\t"
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}