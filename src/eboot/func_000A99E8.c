/**
 * The Sims 2 PSP - func_000A99E8 (0x000A99E8, 0x18 bytes)
 *
 * Loads a pointer from offset 0 of the argument. If it is NULL, returns 1.
 * If non-NULL, subtracts 0x18 from it, returns whether the result is 0
 * (i.e. the pointer was exactly 0x18).
 *
 *     lw   $a1, 0x0($a0)
 *     ori  $a0, $zero, 0x0
 *     bnel $a1, $zero, . + 4 + (0x1 << 2)
 *     addiu $a0, $a1, -0x18
 *     jr   $ra
 *     sltiu $v0, $a0, 0x1
 *
 * **Branchless logic for `ptr == 0 || ptr == 0x18`**.  The `bnel` skips the
 * `addiu` when the pointer is NULL, leaving `$a0` = 0.  The `sltiu` then
 * returns 1 for `$a0` == 0 (NULL case) or `$a0` == 0 (after `addiu` when
 * ptr was 0x18).  For any other pointer, `addiu` produces non-zero and
 * `sltiu` returns 0.
 *
 * So the function returns true iff the loaded pointer is NULL or exactly
 * 0x18.  The 0x18 is a magic address or a specific small allocation.
 */
#include "types.h"

__attribute__((noreturn)) u32 func_000A99E8(void *self) {
    (void)self;
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "lw   $a1, 0x0($a0)\n\t"
        "ori  $a0, $zero, 0x0\n\t"
        "bnel $a1, $zero, 1f\n\t"
        "addiu $a0, $a1, -0x18\n\t"
        "1:\n\t"
        "jr   $ra\n\t"
        "sltiu $v0, $a0, 0x1\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$a0", "$a1", "$v0");
}