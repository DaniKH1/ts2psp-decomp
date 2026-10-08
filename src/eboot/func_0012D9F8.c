/**
 * The Sims 2 PSP - func_0012D9F8 (0x0012D9F8, 0x58 bytes)
 *
 * Loads sixteen floats into four vector quads and stores them back to the same
 * addresses.
 *
 *     lv.s  S200, 0x0($a0)      ... sixteen, one per lane, in quad order
 *     lv.s  S233, 0x3C($a0)
 *     sv.q  R200, 0x0($a0)
 *     sv.q  R201, 0x10($a0)
 *     sv.q  R202, 0x20($a0)
 *     sv.q  R203, 0x30($a0)
 *     jr    $ra
 *     nop
 *
 * **A round trip through the vector unit whose effect on memory is nil.**
 *
 * Sixteen `lv.s` loads fill all four lanes of each of four quads - S200 through S203
 * are one quad, S210 through S213 the next, and so on - and the four `sv.q` stores put
 * each quad back where it came from.  **Every float read is written back unchanged, so
 * a byte comparison of the sixteen words before and after finds no difference.**  No
 * arithmetic happens in between: there is no `vadd`, no `vmul`, nothing at all between
 * the last load and the first store, and the return's delay slot is a plain `nop`.
 *
 * **So this is not a computation, and the question is what it is for.**  Three
 * possibilities and no way to choose between them from the bytes: it exists to touch the
 * memory and the vector cache, it is a template or a stub that the compiler emitted
 * before the operation using it was removed, or it is a no-op by accident.  **What can
 * be said is the shape, and the shape is unusual enough to be worth naming** - it is
 * the vector-unit counterpart of `func_000C3470`'s spill storm, twenty instructions
 * where zero would do, and the two are opposite cases: that one moves sixteen floats
 * through 0xC0 bytes of stack three times over because psp-gcc would not keep them in
 * registers, and this one keeps them in registers and does nothing with them.
 *
 * **`sv.q` is the only store form used.**  Sixteen scalars are loaded one lane at a
 * time because that is the only scalar load the vector unit has - `lv.s` loads one float
 * into one lane - and then one quad store writes all four back.  **So the function pays
 * sixteen loads to avoid four:** four `lv.q` would have loaded the same sixteen floats
 * in four instructions.  **The compiler chose the long way, and the reason is that the
 * source named sixteen separate floats** rather than four quads; a source-level
 * `float[16]` would have lowered to `lv.q` four times.
 *
 * That is the one place in this module where the vector unit is used *worse* than
 * necessary, and it is worth pairing with the fact that `lv.s` at stride 4 filling
 * lanes in order is exactly what a scalar loop unrolled by the compiler looks like.
 *
 * The four quads are R200, R201, R202, R203 and nothing else in the module's
 * 542 vector instructions uses those four together - `tools/vector_unit.py` puts this
 * function seventh of thirty-two by vector-instruction count.
 *
 * **One caveat about the listing this file was transcribed from.**  The first version
 * of the comment put the fourth `sv.q` in the return's delay slot, and the first
 * attempt at the assembly did too - which built 84 bytes against an original of 88, one
 * instruction short, and was wrong about the function.  `tools/c_shapes.py` had printed
 * it that way; `tools/disasm_range.py` prints four stores and then `jr $ra` and `nop`,
 * which is the twenty-two words the size says.  **The project's standing rule is to
 * trust `disasm_range.py` over a shape listing, and this is the case that rule exists
 * for**: the shape tool's rendering is plausible, readable, and one instruction wrong.
 */
#include "types.h"

/** Load sixteen floats through the vector unit and store them back unchanged.
 *  @param p In $a0: sixteen floats, clobbering quads R200 to R203. */
__attribute__((noreturn)) void func_0012D9F8(void *p) {
    (void)p;
    __asm__ __volatile__(
        "lv.s  S200, 0x0($a0)\n\t"
        "lv.s  S210, 0x4($a0)\n\t"
        "lv.s  S220, 0x8($a0)\n\t"
        "lv.s  S230, 0xC($a0)\n\t"
        "lv.s  S201, 0x10($a0)\n\t"
        "lv.s  S211, 0x14($a0)\n\t"
        "lv.s  S221, 0x18($a0)\n\t"
        "lv.s  S231, 0x1C($a0)\n\t"
        "lv.s  S202, 0x20($a0)\n\t"
        "lv.s  S212, 0x24($a0)\n\t"
        "lv.s  S222, 0x28($a0)\n\t"
        "lv.s  S232, 0x2C($a0)\n\t"
        "lv.s  S203, 0x30($a0)\n\t"
        "lv.s  S213, 0x34($a0)\n\t"
        "lv.s  S223, 0x38($a0)\n\t"
        "lv.s  S233, 0x3C($a0)\n\t"
        "sv.q  R200, 0x0($a0)\n\t"
        "sv.q  R201, 0x10($a0)\n\t"
        "sv.q  R202, 0x20($a0)\n\t"
        "sv.q  R203, 0x30($a0)\n\t"
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "nop\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}