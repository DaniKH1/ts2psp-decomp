/**
 * The Sims 2 PSP - func_000B9D2C (0x000B9D2C, 0x8 bytes)
 *
 *     jr    $ra
 *       ori   $v0, $zero, 0x1
 *
 * A default virtual method that returns 1.
 *
 * **Together with func_000B9D34 at the next slot it is one of only two
 * methods in the shared vtable that answer "yes".**  Of the 26 slots in
 * `sym_001EA3E8`, twenty-three are identical across all three sibling
 * classes: these two return 1, sixteen return 0, one is void
 * (`func_0018F668`), and four have real bodies.  So the base class
 * offers a predicate interface in which almost everything is false and
 * exactly two things are true - the usual shape of a set of capability
 * flags where the derived classes here override none of them.
 */
#include "types.h"

__attribute__((noreturn)) void func_000B9D2C(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "ori   $v0, $zero, 0x1\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}