/**
 * The Sims 2 PSP - func_000F9C78 (0x000F9C78, 0x1C bytes)
 *
 * Indexes an array of pointers and reads one field of what it finds.
 *
 *     lw   $a1, 0x10($a0)
 *     lw   $a0, 0x18($a0)
 *     sll  $a1, $a1, 2
 *     addu $a0, $a0, $a1
 *     lw   $a0, 0x0($a0)
 *     jr   $ra
 *     lw   $v0, 0x54($a0)
 *
 * **`return ((*(*(u32 ***)self))[self->index])->field_54;`**  - three dereferences
 * deep, reached in six instructions.
 *
 * **The array's stride is four and it is spelled `sll 2`, as everywhere else in this
 * module.**  `sll $a1, $a1, 2` is the third instance in this file's neighbourhood and
 * the same choice as `sortAndCullScene_1080`'s record stride and `func_000BF49C`'s
 * index.  **The module has no `mult` for any of these**, and psp-gcc's constant
 * multiply for a power of two is a shift, so this is the codegen and not a decision -
 * but it is worth writing down because the alternative spelling (`addiu $a1, $a1,
 * $a1` twice) costs two instructions and does not appear.
 *
 * **The two loads are of different kinds and it matters which.**  `lw $a1, 0x10($a0)`
 * and `lw $a0, 0x18($a0)` both read from the same object eight bytes apart: the first
 * is the index, the second is the array's base.  **Then `$a0` is overwritten by the
 * dereference**, so the object's own pointer is dead from that point on and the
 * function holds two unrelated pointers by the end - the index in `$a1` and the
 * element in `$a0`.  Nothing needs the object again.
 *
 * **The result is loaded in the delay slot**, which is the only thing that can be
 * there: `$a0` holds the element pointer and the load reads through it, so the `lw` has
 * to be the instruction after `jr $ra` or the pointer is not yet computed.  Under
 * `.set reorder` the assembler would fill the slot with the preceding `lw $a0, 0x0($a0)`
 * instead - one instruction earlier, but then the field load would read from the
 * *array slot* rather than from the element.
 *
 * There is no bounds check.  The index is used as given, so a caller that passes an
 * index past the end reads whatever is there; the three dereferences mean the value at
 * `field_54` is four levels of pointer from the argument.
 */
#include "types.h"

/** Read the field at offset 0x54 of the indexed element of an array of pointers.
 *  @param self In $a0: the object; +0x10 is the index and +0x18 the array base.
 *  @return     The field, in `$v0`. */
__attribute__((noreturn)) long func_000F9C78(void *self) {
    (void)self;
    __asm__ __volatile__(
        "lw   $a1, 0x10($a0)\n\t"
        "lw   $a0, 0x18($a0)\n\t"
        "sll  $a1, $a1, 2\n\t"
        "addu $a0, $a0, $a1\n\t"
        "lw   $a0, 0x0($a0)\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "lw   $v0, 0x54($a0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "$v0", "$a0", "$a1");
}