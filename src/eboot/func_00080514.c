/**
 * The Sims 2 PSP - func_00080514 (0x00080514, 0x8 bytes)
 *
 *     jr    $ra
 *       lw    $v0, 0x1C($a0)
 *
 * A field getter returning the word at offset 0x1C, and one of six
 * identical +0x1C getters in the module.
 *
 * **It is the second member of the accessor block that runs from
 * func_000804B8 to func_000805D4.**  Read the whole block:
 *
 *     func_000804B8   bit 3 of 0x4($a0)      flag query
 *     func_00080514   lw $v0, 0x1C($a0)     getter
 *     func_0008054C   lw $v0, 0x20($a0)     getter
 *     func_00080584   lw $v0, 0x18($a0)     getter
 *     func_0008058C   lw $v0, 0x18($a0)     getter, duplicate
 *     func_000805D4   addiu $v0, $a0, 0x8   base-pointer adjustment
 *
 * So this class has readable words at **0x18, 0x1C and 0x20**, a flags
 * word at **0x04**, and an accessor that hands back a sub-object at
 * **+8**.  The two adjacent 0x18 getters are the duplicated-inlining
 * shape described in func_0004E5CC - **and having one right next to the
 * block is what confirms the rule rather than leaving it a guess.**
 *
 * `func_000805DC`, 0x104 bytes, starts immediately after the block and
 * dereferences 0x2C($s1) + 0x18, so the object is at least 0x30 bytes.
 */
#include "types.h"

__attribute__((noreturn)) void func_00080514(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "lw    $v0, 0x1C($a0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}