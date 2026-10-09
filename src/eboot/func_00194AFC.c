/**
 * The Sims 2 PSP - func_00194AFC (0x00194AFC, 0x8 bytes)
 *
 *     jr    $ra
 *       sw    $a1, 0x14($a0)
 *
 * A field setter: stores $a1 at offset 0x14 of the object.
 *
 * **It is the only setter in the accessor block that starts at
 * func_00194AA8**, and it is the piece that explains the gap:
 *
 *     func_00194AA8   lw $v0, 0x0($a0)      getter
 *     func_00194AB0   lw $v0, 0x4($a0)      getter
 *     func_00194AB8   lw $v0, 0x18($a0)     getter
 *     func_00194AC0   lw $v0, 0x1C($a0)     getter
 *     ...
 *     func_00194AFC   sw $a1, 0x14($a0)     setter
 *
 * So the class exposes 0x14 read-write through this one function, and
 * the three getters around it have no setters of their own - they are
 * read-only fields.  **A getter without a setter in the same block is a
 * field the class exposes for reading only**, which is the closest
 * these eight bytes come to saying what a field is for.
 *
 * The store is in the delay slot of the `jr`, so the function returns
 * whatever `$v0` held on entry - it writes no return value.
 */
#include "types.h"

__attribute__((noreturn)) void func_00194AFC(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "sw    $a1, 0x14($a0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}