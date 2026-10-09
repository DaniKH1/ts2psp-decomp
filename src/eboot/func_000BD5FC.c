/**
 * The Sims 2 PSP - func_000BD5FC (0x000BD5FC, 0x38 bytes)
 *
 *     addiu $sp, $sp, -0x20
 *     sw    $s0, 0x10($sp)
 *     sw    $ra, 0x14($sp)
 *     jal   func_000A7A8C
 *       or    $s0, $a0, $zero
 *     lui   $a0, 0x1F
 *     addiu $a0, $a0, %lo(sym_001EAA78)
 *     sw    $a0, 0x18($s0)
 *     sw    $zero, 0x24($s0)
 *     or    $v0, $s0, $zero
 *     lw    $s0, 0x10($sp)
 *     lw    $ra, 0x14($sp)
 *     jr    $ra
 *     addiu $sp, $sp, 0x20
 *
 * **This is the first member of a five-function group, and the group is
 * the unit - not the getter.**  Eleven such groups run from 0x000BD5F4 to
 * 0x000C1498, each with the same shape:
 *
 *     +0x00   8 bytes    lw $v0, 0x24($a0)     getter
 *     +0x08  56 or 60    this function         registers a chunk tag
 *     +0x40  160         four tag registrations
 *     +0xE0  80-240      per-group work
 *     ...     68-240     per-group work
 *
 * **So the thirteen 0x24 getters are not one class's accessor block.**
 * Each belongs to a different group's first slot, and what distinguishes
 * group 1 from group 2 is the global it names, not the object it reads:
 *
 *     func_000BD5FC  -> sym_001EAA78    func_000BD634 -> sym_001E9E30
 *     func_000BD770  -> sym_001EAB50                    sym_001E9478
 *     func_000BD8D4  -> sym_001EAC28                    sym_001E9300
 *     func_000BDA48  -> sym_001EAD00
 *
 * `func_000BD5FC` and `func_000BD8D4` are **identical except one word** -
 * the `addiu` immediate, 0xAA78 against 0xAC28.  `func_000BD724` and its
 * twin `func_000BD9FC` are **identical including that word**.
 *
 * Read as a whole: this is **one chunk type instantiated per group**, each
 * with its own global for the type's own data and its own globals for the
 * three tags it registers.  The getter at +0x00 reads 0x24 because that is
 * the field the group initialises to 0 right here, on line 7.
 */
#include "types.h"

__attribute__((noreturn)) void func_000BD5FC(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "sw    $s0, 0x10($sp)\n\t"
        "sw    $ra, 0x14($sp)\n\t"
        "jal   func_000A7A8C\n\t"
        "or    $s0, $a0, $zero\n\t"
        "lui   $a0, 0x1F\n\t"
        "addiu $a0, $a0, %%lo(sym_001EAA78)\n\t"
        "sw    $a0, 0x18($s0)\n\t"
        "sw    $zero, 0x24($s0)\n\t"
        "or    $v0, $s0, $zero\n\t"
        "lw    $s0, 0x10($sp)\n\t"
        "lw    $ra, 0x14($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}