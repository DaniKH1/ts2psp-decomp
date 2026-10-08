/**
 * The Sims 2 PSP - func_00102230 (0x00102230, 0x1C bytes)
 *
 *     addiu $sp, $sp, -0x10
 *     sw    $ra, 0x0($sp)
 *     jal   func_001029E8
 *     nop
 *     lw    $ra, 0x0($sp)
 *     jr    $ra
 *     addiu $sp, $sp, 0x10
 *
 * A standard save-`$ra` / call / restore-`$ra` wrapper around `func_001029E8`.
 *
 * **The callee is itself `jr $ra; nop`, so this function also does nothing.**
 * That is the only fact about it that the bytes give directly, and it is worth
 * stating plainly rather than describing the wrapper as if it were doing work:
 * the frame is built, one call is made, the frame is torn down, and the call
 * returns without having touched anything.  The 0x10-byte frame saves only
 * `$ra` and holds nothing else.
 *
 * ## What this is not
 *
 * It is not a tail call.  `jal` rather than `jalr` on `$t9`, and `$ra` is saved
 * before the call and reloaded after it, so the caller's own return address is
 * preserved across the call - **which is what makes it safe to call a function
 * that might have been expected to return something.**  A tail call would not
 * have needed the frame at all.
 *
 * Its sibling `func_0010224C` is the same seven instructions with a 0x20-byte
 * frame and `func_001029E0` as the callee.  **The two differ in frame size and
 * in nothing else**, and since both callees are empty the frame size is the
 * only difference there is.
 */
#include "types.h"

/** Calls `func_001029E8`, which does nothing, and returns. */
__attribute__((noreturn)) void func_00102230(void) {
    __asm__ __volatile__(
        "addiu $sp, $sp, -0x10\n\t"
        "sw    $ra, 0x0($sp)\n\t"
        ".set noreorder\n\t"
        "jal   func_001029E8\n\t"
        "nop\n\t"
        "lw    $ra, 0x0($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x10\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}