/**
 * The Sims 2 PSP - func_00194AB8 (0x00194AB8, 0x8 bytes)
 *
 *     jr    $ra
 *       lw    $v0, 0x18($a0)
 *
 * A field getter returning the word at offset 0x18, and one of three
 * identical +0x18 getters.
 *
 * **It is the third member of the block that starts at func_00194AA8**:
 *
 *     func_00194AA8   lw $v0, 0x0($a0)
 *     func_00194AB0   lw $v0, 0x4($a0)
 *     func_00194AB8   lw $v0, 0x18($a0)
 *     func_00194AC0   lw $v0, 0x1C($a0)
 *
 * Four consecutive 8-byte functions, all getters, all distinct offsets.
 * **This is the clearest per-class accessor block in the module**: the
 * class has fields at 0x00, 0x04, 0x18 and 0x1C, and the 0x14 bytes
 * between 0x04 and 0x18 are reached by other code rather than by a
 * getter of their own.  func_00194AFC then does `sw $a1, 0x14($a0)`,
 * **which puts a setter in the same block and fills in the gap.**
 */
#include "types.h"

__attribute__((noreturn)) void func_00194AB8(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "lw    $v0, 0x18($a0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}