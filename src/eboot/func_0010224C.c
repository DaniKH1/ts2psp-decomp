/**
 * The Sims 2 PSP - func_0010224C (0x0010224C, 0x1C bytes)
 *
 *     addiu $sp, $sp, -0x20
 *     sw    $ra, 0x10($sp)
 *     jal   func_001029E0
 *     nop
 *     lw    $ra, 0x10($sp)
 *     jr    $ra
 *     addiu $sp, $sp, 0x20
 *
 * A standard save-`$ra` / call / restore-`$ra` wrapper around `func_001029E0`,
 * and the same seven instructions as `func_00102230`.
 *
 * **The callee is `jr $ra; nop`, so this function does nothing either.**
 *
 * ## The one difference from its sibling, and it is real but small
 *
 * The two wrappers are identical apart from the frame size and the callee:
 *
 *     func_00102230   0x10 frame, $ra at +0x0,  jal func_001029E8
 *     func_0010224C   0x20 frame, $ra at +0x10, jal func_001029E0
 *
 * **The 0x20-byte frame saves `$ra` at `0x10` rather than at `0x0`, and holds
 * sixteen bytes that nothing in the function touches.**  That is the standard
 * shape for a frame that must stay 8-mod-16 aligned - a callee is not required
 * to keep its own 16-byte alignment in this ABI, so the caller does it - but
 * **the frame here saves one register, so the extra sixteen bytes are
 * alignment padding and not a local anything lost.**  Saying "it allocates a
 * 32-byte frame" would be right; saying why would need the alignment rule to
 * be stated, and this file only notes that nothing uses the space.
 *
 * The callee is at `func_001029E0` and not `func_001029E8` - **one word
 * difference**, and it is the whole reason these two functions exist separately
 * rather than one calling the other.
 */
#include "types.h"

/** Calls `func_001029E0`, which does nothing, and returns. */
__attribute__((noreturn)) void func_0010224C(void) {
    __asm__ __volatile__(
        "addiu $sp, $sp, -0x20\n\t"
        "sw    $ra, 0x10($sp)\n\t"
        ".set noreorder\n\t"
        "jal   func_001029E0\n\t"
        "nop\n\t"
        "lw    $ra, 0x10($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}