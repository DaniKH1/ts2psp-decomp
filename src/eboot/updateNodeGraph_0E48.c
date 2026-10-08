/**
 * The Sims 2 PSP - updateNodeGraph_0E48 (0x001B66C8, 0x24 bytes)
 *
 * Copies a three-float vector out of a nested structure.
 *
 *     lw   $a0, 0x8($a0)
 *     addiu $a0, $a0, 0x1C
 *     lwc1 $f12, 0x0($a0)
 *     swc1 $f12, 0x0($a1)
 *     lwc1 $f12, 0x4($a0)
 *     swc1 $f12, 0x4($a1)
 *     lwc1 $f12, 0x8($a0)
 *     jr   $ra
 *     swc1 $f12, 0x8($a1)
 *
 * **`*(f32[3] *)out = *(f32[3] *)((char *)self->ptr_08 + 0x1C);`**  - the three-float
 * copy of `func_00150988`, except that the source is a pointer *inside* the object
 * rather than the object itself.
 *
 * The offset is 0x1C, not 0x30 or 0x8, and that is the whole difference between this
 * function and the four-member copy family at 0x150988, 0x193358, 0x1965E8 and
 * 0x196608: same eight-instruction shape, source reached one dereference deeper.
 * Four of that family take a pointer straight from `$a0`; this one loads `$a0` from
 * offset 8 of `$a0` first and adds 0x1C to *that*.  A member vector in a nested
 * object, where the other four are members of the object itself.
 *
 * **What this file cannot claim, and nearly did.**  The four neighbours are called
 * `func_00150988` and the like, which are *synthetic* names - address-derived,
 * because the original symbol table does not carry a name for them
 * (`tools/orig_names.py` finds none).  This function is called
 * `updateNodeGraph_0E48`, which is a *surviving* CodeWarrior name: the linker kept it,
 * and there are 28 symbols in the module called `updateNodeGraph`.
 *
 * **So it is true that this one is named and the other four are not, and it is not
 * true that the four therefore share a translation unit.**  Silence in a symbol table
 * is not evidence of anything, and `asm/eboot/` splits one `.s` per function, so the
 * build cannot show the unit boundary either.  An earlier draft of this comment read
 * the naming difference as a header boundary - "the four that share a shape share a
 * unit, and the one whose header is elsewhere has its own unit" - and that is an
 * argument from a missing name to a real fact, which is backwards.  **What can be said
 * is narrower and still worth saying: five functions share this shape, and only one
 * of the five came through the link with a name.**
 *
 * Nor does `updateNodeGraph` name a class here - it is a function name that 28
 * symbols share, which is consistent with a class accessor, a virtual-table entry and
 * a plain static all at once.
 *
 * **`.set noreorder` around the return.**  The last `lwc1` feeds the `swc1` in the
 * delay slot, and under `.set reorder` the assembler would hoist the `lwc1` into the
 * slot - which would store the value read from offset 4 at offset 8.
 */
#include "types.h"

/** Copy the three floats at offset 0x1C of the object the first argument points at.
 *  @param self In $a0: a pointer to the container is read from offset 8.
 *  @param out  In $a1: receives the three floats. */
__attribute__((noreturn)) void updateNodeGraph_0E48(void *self, void *out) {
    __asm__ __volatile__(
        "lw   $a0, 0x8($a0)\n\t"
        "addiu $a0, $a0, 0x1C\n\t"
        ".set noreorder\n\t"
        "lwc1 $f12, 0x0($a0)\n\t"
        "swc1 $f12, 0x0($a1)\n\t"
        "lwc1 $f12, 0x4($a0)\n\t"
        "swc1 $f12, 0x4($a1)\n\t"
        "lwc1 $f12, 0x8($a0)\n\t"
        "jr   $ra\n\t"
        "swc1 $f12, 0x8($a1)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$a0", "$a1", "$f12");
}