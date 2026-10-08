/**
 * The Sims 2 PSP - func_00102E6C (0x00102E6C, 0x2C bytes)
 *
 * Copies four floats to a fixed address.
 *
 *     lwc1  $f12, 0x0($a0)
 *     lui   $a1, 0xF
 *     swc1  $f12, -0x3818($a1)
 *     lwc1  $f12, 0x4($a0)
 *     addiu $a1, $a1, -0x3818
 *     swc1  $f12, 0x4($a1)
 *     lwc1  $f12, 0x8($a0)
 *     swc1  $f12, 0x8($a1)
 *     lwc1  $f12, 0xC($a0)
 *     jr    $ra
 *     swc1  $f12, 0xC($a1)
 *
 * **`*(f32 *)0x0EC7E8 = src->x; ... + 0xC; ... + 0x10; ... + 0x14;`**
 *
 * **The address is 0x0EC7E8 and the pair of halves sits across the first store.**
 *
 *     lui   $a1, 0xF           ->  0x000F0000
 *     swc1  $f12, -0x3818($a1) ->  the offset is the low half, in the store
 *     addiu $a1, $a1, -0x3818  ->  and here again, forming the base for the rest
 *
 * **So the `%hi`/`%lo` pair is split between a store's offset field and an `addiu`, with
 * the first float stored before the base exists.**  This is the pattern that made
 * `base_of` in `tools/stride_table.py` miss it for one revision - the tool wants an
 * `addiu` completing a `lui`, and there is one, but only after a store has already used
 * the half.  `func_00102D34` has the same shape in integer form, with
 * `sw $a1, 0x2168($t0)` before `addiu $a1, $t0, 0x2168`.
 *
 * **0x0EC7E8 is 128 bytes past 0x0EC768, which `func_00102C84` writes three words to,
 * and both land in the same nominal function** - `func_000EC5E8`, at +0x180 and +0x200.
 * **That pairing is the strongest evidence this project has for these addresses being a
 * data region rather than a patch target**: two independent initialisers, in different
 * translation units, writing into one nominal function body a hundred and twenty-eight
 * bytes apart.  It is still not proof, and the limit is the same as before - splat's
 * sizes are "distance to the next label" and there is no label at either address, so
 * `func_000EC5E8`'s nominal body may simply be swallowing a data block that the symbol
 * map does not name.
 *
 * **Finding it needed `code_writers.py` to look at the float opcodes.**  Its
 * `store_bases` listed seven integer load and store opcodes and omitted `lwc1` and
 * `swc1`, so a function that writes nothing *but* floats to an address in the code
 * section named no load or store base at all and was rejected before the address was
 * ever considered.  **With `0x31` and `0x39` added the census goes from 1,024
 * functions at 455 bases to 1,458 at 675** - a fourth of the total appearing from one
 * two-line change.
 *
 * **The four `lwc1`/`swc1` pairs are the base-pointer form with no base pointer for the
 * first one**, and this is a `tools/base_pointer.py` member: one `addiu` and four
 * stores against four stores with immediate offsets.  The count says which is more
 * common overall (687 functions run stores through a formed base); it does not say why
 * this function chose it, and `func_00055974` - next page of the queue - does the same
 * kind of copy in both spellings within one body.
 */
#include "types.h"

/** Copy four floats from `$a0` to the fixed address 0x0EC7E8.
 *  @param src In $a0: four consecutive floats. */
__attribute__((noreturn)) void func_00102E6C(void *src) {
    (void)src;
    __asm__ __volatile__(
        "lwc1  $f12, 0x0($a0)\n\t"
        "lui   $a1, 0xF\n\t"
        "swc1  $f12, -0x3818($a1)\n\t"
        "lwc1  $f12, 0x4($a0)\n\t"
        "addiu $a1, $a1, -0x3818\n\t"
        "swc1  $f12, 0x4($a1)\n\t"
        "lwc1  $f12, 0x8($a0)\n\t"
        "swc1  $f12, 0x8($a1)\n\t"
        "lwc1  $f12, 0xC($a0)\n\t"
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "swc1  $f12, 0xC($a1)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$a0", "$a1", "$f12");
}