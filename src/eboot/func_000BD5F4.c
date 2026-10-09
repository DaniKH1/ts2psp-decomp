/**
 * The Sims 2 PSP - func_000BD5F4 (0x000BD5F4, 0x8 bytes)
 *
 *     jr    $ra
 *       lw    $v0, 0x24($a0)
 *
 * A field getter: returns the word at offset 0x24 of the object.
 *
 * **There are thirteen of these in the module, all identical.**  They sit
 * at func_000BD5F4, func_000BD768, func_000BD8CC, func_000BDA40,
 * func_000BDBC4, func_000BDDE4, func_000BE008, func_000BE3A0,
 * func_000BE628, func_000BE8B8, func_000BEE9C, func_000C1490 and
 * func_000C1498 - consecutive addresses through 0x000BE8B8, then a gap
 * to 0x000C1490.  **That spacing is a per-class getter block**: one
 * class's inline accessors emitted together, with the last two landing
 * after a gap where something larger was placed.
 *
 * `lw` and not `lw` + `nop`: the load is in the delay slot, so the
 * function is a leaf with no frame.  Note it does *not* `or $v0, $v0,
 * $zero` - the load result goes straight out.
 */
#include "types.h"

__attribute__((noreturn)) void func_000BD5F4(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "lw    $v0, 0x24($a0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}