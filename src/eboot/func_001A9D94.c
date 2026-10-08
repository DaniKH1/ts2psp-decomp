/**
 * The Sims 2 PSP - func_001A9D94 (0x001A9D94, 0x14 bytes)
 *
 * Sets one bit in a flags word held at offset 0x18 of the first argument.
 *
 *     lw    $a1, 0x18($a0)
 *     lui   $a2, 0x4
 *     or    $a1, $a1, $a2
 *     jr    $ra
 *     sw    $a1, 0x18($a0)
 *
 * **`self->flags_18 |= 0x40000;`**  - sets bit 18.  Identical in shape to
 * `func_001A9D54` twenty-four bytes earlier, which sets bit 20 of the same word.
 *
 * The two are setters for two different boolean members of the same class.  Bit 20
 * is the interesting one because it kept all three of its accessors - setter
 * (`func_001A9D54`), clearer (`func_001A9D68`) and getter (`func_001A9D80`) - while
 * bit 18 kept only this setter and bit 17 (`func_001A9ACC`) only its getter.
 * `tools/flag_accessors.py` is the census: six functions, four rows.
 *
 * **Reading the asymmetry as evidence about the linker, not about the class.**  A
 * `bool` member whose accessor was inlined at every call site would leave only the
 * out-of-line copies, and which of the three directions that is depends on how
 * each was spelled at its call sites.  It is not evidence that bit 18 is
 * write-only, and nothing in the module says it is.
 */
#include "types.h"

/** Set bit 18 (0x40000) of the flags word at offset 0x18 of the first argument. */
__attribute__((noreturn)) void func_001A9D94(void *a0) {
    (void)a0;
    __asm__ __volatile__(
        "lw    $a1, 0x18($a0)\n\t"
        "lui   $a2, 0x4\n\t"
        "or    $a1, $a1, $a2\n\t"
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "sw    $a1, 0x18($a0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$a1", "$a2");
}