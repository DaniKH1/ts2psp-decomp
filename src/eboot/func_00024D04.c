/**
 * The Sims 2 PSP - func_00024D04 (0x00024D04, 0x30 bytes)
 *
 * A constructor: three pointers and a zero, and the object pointer returned.
 *
 *     lui  $a1, 0x1E
 *     addiu $a1, $a1, 0x4110
 *     sw   $a1, 0x0($a0)
 *     lui  $a1, 0x1E
 *     addiu $a1, $a1, 0x41B0
 *     sw   $a1, 0x4($a0)
 *     lui  $a1, 0x1E
 *     addiu $a1, $a1, 0x41C0
 *     sw   $a1, 0x8($a0)
 *     sw   $zero, 0x10($a0)
 *     jr   $ra
 *     move $v0, $a0
 *
 * **`self->vtable = 0x1E4110; self->a = 0x1E41B0; self->b = 0x1E41C0;
 * self->count = 0; return this;`**
 *
 * **Three pointers into `.rodata`, sixteen bytes apart.**  0x1E41B0 and 0x1E41C0
 * are adjacent records, and 0x1E4110 is 0xA0 bytes before the first - `.rodata` runs
 * from 0x1BF880 to 0x1D1ABC, so all three are inside it.  A C++ object with three
 * pointers to read-only tables at the front and a count at +0x10 is the shape of a
 * cache or a lookup: three constant inputs, one accumulating output.
 *
 * **The three `lui`s are all the same and none is folded away**, which is the
 * register allocator's choice - `$a1` is written and stored four times, and the
 * high half would have to be re-`lui`'d after each store anyway if it were kept in
 * another register.  Reusing `$a1` costs one `lui` per field; using a second
 * register costs a `mov`.  This is the same reasoning as the `not $a0, $a1` in the
 * alignment helpers, where reusing the input register was worth a whole
 * instruction.
 *
 * **The store order is +0, +4, +8 and the zero last**, which is the source order
 * with the zero deferred - a fourth field at +0xC is not written at all, so if the
 * object has one it is left as whatever the allocator handed over.
 */
#include "types.h"

/** Initialise four fields of the object and return it.
 *  @param self In $a0: the object to initialise.
 *  @return     `self`. */
__attribute__((noreturn)) void *func_00024D04(void *self) {
    (void)self;
    __asm__ __volatile__(
        "lui  $a1, 0x1E\n\t"
        "addiu $a1, $a1, 0x4110\n\t"
        "sw   $a1, 0x0($a0)\n\t"
        "lui  $a1, 0x1E\n\t"
        "addiu $a1, $a1, 0x41B0\n\t"
        "sw   $a1, 0x4($a0)\n\t"
        "lui  $a1, 0x1E\n\t"
        "addiu $a1, $a1, 0x41C0\n\t"
        "sw   $a1, 0x8($a0)\n\t"
        "sw   $zero, 0x10($a0)\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "move $v0, $a0\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$v0", "$a0", "$a1");
}