/**
 * The Sims 2 PSP - func_000DD504 (0x000DD504, 0x20 bytes)
 *
 * Stores two floats from the FPU argument registers to consecutive globals,
 * then sets a byte global to 1.
 *
 *     lui  $a0, 0x1E
 *     swc1 $f12, -0x6058($a0)   0x1E0000 - 0x6058 = 0x1D9FA8
 *     lui  $a0, 0x1E
 *     swc1 $f13, -0x605C($a0)   0x1E0000 - 0x605C = 0x1D9FA4
 *     ori  $a0, $zero, 0x1
 *     lui  $a1, 0x1E
 *     jr   $ra
 *     sb   $a0, -0x6054($a1)    0x1E0000 - 0x6054 = 0x1D9FAC
 *
 * **Two floats stored to consecutive words.**  `$f12` and `$f13` are the first
 * and second float argument registers.  The globals are at 0x1D9FA8 and 0x1D9FA4
 * (descending addresses, so $f12 goes to the higher address).
 *
 * **Then a byte is set to 1** at 0x1D9FAC, using the 1 already in `$a0` from
 * the `ori`.  The delay slot of `jr` does the byte store through `$a1` (which
 * was loaded with the same 0x1E page).
 *
 * **No return value used** - the function is void.  The `ori` sets up the byte
 * value but also leaves 1 in `$a0`, which would be the return value if one
 * were used.
 *
 * The three globals are consecutive: 0x1D9FAC (byte), 0x1D9FA4 (float $f13),
 * 0x1D9FA8 (float $f12).  This is a small record being initialised.
 */
#include "types.h"

/* 0x1E0000 - 0x6058 = 0x1D9FA8.  Low global page. */
#define GLOBAL_F12  0x0001D9FA8u
#define GLOBAL_F13  0x0001D9FA4u
#define GLOBAL_BYTE 0x0001D9FACu

__attribute__((noreturn)) void func_000DD504(float f12, float f13) {
    /* Arguments arrive in $f12 and $f13 per o32 calling convention.  The asm
     * block uses them directly; the C parameters silence unused warnings. */
    (void)f12; (void)f13;
    __asm__ __volatile__(
        "lui  $a0, 0x1E\n\t"
        "swc1 $f12, -0x6058($a0)\n\t"
        "lui  $a0, 0x1E\n\t"
        "swc1 $f13, -0x605C($a0)\n\t"
        "ori  $a0, $zero, 0x1\n\t"
        "lui  $a1, 0x1E\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "sb   $a0, -0x6054($a1)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$a0", "$a1", "$f12", "$f13");
}