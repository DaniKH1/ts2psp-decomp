/**
 * The Sims 2 PSP - func_0009674C (0x0009674C, 0x34 bytes)
 *
 * A constructor: six fields set, and the object pointer returned.
 *
 *     lui  $a1, 0x0
 *     addiu $a1, $a1, 0x17BC
 *     sw   $a1, 0x20($a0)
 *     ori  $a2, $zero, 0x2
 *     lui  $a1, 0x1D
 *     sw   $a2, 0x0($a0)
 *     addiu $a1, $a1, -0x7180
 *     sw   $a1, 0x4($a0)
 *     sw   $zero, 0x14($a0)
 *     sw   $zero, 0x18($a0)
 *     sw   $zero, 0x1C($a0)
 *     jr   $ra
 *     move $v0, $a0
 *
 * **`self->field_00 = 2; self->field_04 = 0x18E80; self->field_14 = 0;
 * self->field_18 = 0; self->field_1C = 0; self->field_20 = 0x17BC; return this;`**
 *
 * **The `lui 0x0` is dead.**  `0x17BC` fits a signed 16-bit immediate, so the
 * `addiu` alone would do, and the `lui` is there because the source's constant went
 * through the same `%hi`/`%lo` path as the one at +4 - which does need both, since
 * 0x18E80 does not fit.  CodeWarrior paired them and did not fold the zero away.
 *
 * That is the same shape as the dead channel in `func_000706A8` and worth pairing it
 * with: there the arithmetic was dead, here the *high half* is, and in both the
 * compiler had a cheaper spelling available and did not take it.  Written as
 * `0x17BC` in one `addiu` the function would be 24 bytes.
 *
 * **+4 is a pointer into `.data` and +20 a small count.**  0x18E80 is 101,504 and
 * lies inside `.data`, which starts at 0x1D1B00 - so it is a reference to a global
 * rather than to code, unlike the 0x0EB850 cluster in `func_000F93E8`.  The 2 at +0
 * looks like a version or a kind tag; the three consecutive zeroed words at +14, +18
 * and +1C are three fields of the same kind initialised together.
 *
 * The `move $v0, $a0` in the delay slot is the `return this` that a C++ constructor
 * compiles to, and `$a0` is clobbered so the pointer has to be named rather than
 * left in the incoming register.
 */
#include "types.h"

/* The global at offset +4, as the original spells it: `lui 0x1D` / `addiu -0x7180`. */
#define GLOBAL_HI 0x1D
#define GLOBAL_LO (-0x7180)

/** Initialise six fields of the object and return it.
 *  @param self In $a0: the object to initialise.
 *  @return     `self`. */
__attribute__((noreturn)) void *func_0009674C(void *self) {
    (void)self;
    __asm__ __volatile__(
        "lui  $a1, 0x0\n\t"
        "addiu $a1, $a1, 0x17BC\n\t"
        "sw   $a1, 0x20($a0)\n\t"
        "ori  $a2, $zero, 0x2\n\t"
        "lui  $a1, %[hi]\n\t"
        "sw   $a2, 0x0($a0)\n\t"
        "addiu $a1, $a1, %[lo]\n\t"
        "sw   $a1, 0x4($a0)\n\t"
        "sw   $zero, 0x14($a0)\n\t"
        "sw   $zero, 0x18($a0)\n\t"
        "sw   $zero, 0x1C($a0)\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "move $v0, $a0\n\t"
        ".set reorder\n\t"
        : : [hi] "i" (GLOBAL_HI), [lo] "i" (GLOBAL_LO)
        : "memory", "$v0", "$a0", "$a1", "$a2");
}