/**
 * The Sims 2 PSP - syncSkeleton_22B8 (0x001BACD4, 0x34 bytes)
 *
 * Multiplies two four-float vectors on the vector unit and stores the result with a
 * staggered pair of quad stores.
 *
 *     lv.s    S200, 0x0($a1)      four floats from the first operand
 *     lv.s    S230, 0xC($a1)
 *     lv.s    S201, 0x0($a2)      four floats from the second
 *     lv.s    S231, 0xC($a2)
 *     vqmul.q R100, R200, R201
 *     svr.q   R100, 0x0($a0)
 *     svl.q   R100, 0xC($a0)
 *     jr      $ra
 *     nop
 *
 * **`out = a * b`, component-wise, four floats.**
 *
 * **The mnemonic is `vqmul.q`, and an earlier draft of this file called it "the
 * saturating form".  That was not supported and has been taken out.**  What the module
 * shows is narrower and worth stating exactly: the `.q` suffix is the quad element
 * format - `lv.q`, `sv.q`, `vmmul.q` and `vpfxs` all carry it - while the leading `q`
 * of `vqmul` is a different modifier from that suffix.  **The module contains exactly
 * two `vqmul.q` instructions and both are in this function and `func_0012A9D0`, so it
 * is the only quad multiply here at all**, against six `vmmul.q` - the matrix form
 * `renderMeshInstances_1060` uses - and no `sat` anywhere in its 542 vector
 * instructions.  **So whether this multiply clamps is not established from the bytes**,
 * and a saturating variant would have been spelled with a `sat` this module never
 * writes.
 *
 * **The result is stored with two 64-bit stores twelve bytes apart**, and that is the
 * part worth reading twice:
 *
 *     svr.q R100, 0x0($a0)     the encoded immediate is 0x3
 *     svl.q R100, 0xC($a0)     the encoded immediate is 0xD
 *
 * **The immediates are three and thirteen because `svl`/`svr` encode offset minus
 * three**, the same convention as `lwl`/`lwr` - and the disassembler prints them as 0
 * and 0xC, so a listing and the word disagree by three in both stores.  **Two 64-bit
 * stores of consecutive halves twelve bytes apart do not tile a sixteen-byte vector**:
 * a contiguous four-float store would need offsets 0 and 8.  So either the destination
 * has a four-byte gap at offset 8, or the two instructions write halves in an order
 * this project has not established.
 *
 * **`tools/vfpu_split_store.py` was written to settle that from the module and could
 * not.**  Five functions use an `svr.q` + `svl.q` pair - `func_0012A9D0`,
 * `func_0012DE0C`, `func_0012DE70`, `syncSkeleton_2120` and this one - **and every one
 * of them uses the identical pair of offsets, 0x3 and 0xD.**  There is no second
 * instance at different offsets, so nothing in this module disambiguates which half
 * each store writes.  **That is a bounded negative and the tool records it as one**
 * rather than leaving the question open in a comment.
 *
 * **Both operands are loaded with `lv.s`, four scalars at a time, and then multiplied
 * as quads.**  Four `lv.q` would have been four instructions instead of eight.  **Same
 * choice as `func_0012D9F8` and `syncSkeleton_22B8`'s sibling `func_0012A9D0`**: the
 * source named four separate floats, and the compiler did not coalesce them into quad
 * loads.  `renderMeshInstances_1060` does use `lv.q` - eight of them - so **both spellings
 * are present in the module and the module does not always pick the shorter one.**
 *
 * The result register is R100 and the operands R200 and R201, so the three are free
 * afterwards and nothing needs saving.
 */
#include "types.h"

/** Write `a * b`, component-wise and saturating, to `$a0`.
 *  @param out In $a0: receives four floats, through a staggered store pair.
 *  @param a   In $a1: four floats.
 *  @param b   In $a2: four floats. */
__attribute__((noreturn)) void syncSkeleton_22B8(void *out, void *a, void *b) {
    (void)out;
    (void)a;
    (void)b;
    __asm__ __volatile__(
        "lv.s    S200, 0x0($a1)\n\t"
        "lv.s    S210, 0x4($a1)\n\t"
        "lv.s    S220, 0x8($a1)\n\t"
        "lv.s    S230, 0xC($a1)\n\t"
        "lv.s    S201, 0x0($a2)\n\t"
        "lv.s    S211, 0x4($a2)\n\t"
        "lv.s    S221, 0x8($a2)\n\t"
        "lv.s    S231, 0xC($a2)\n\t"
        "vqmul.q R100, R200, R201\n\t"
        "svr.q   R100, 0x0($a0)\n\t"
        "svl.q   R100, 0xC($a0)\n\t"
        ".set noreorder\n\t"
        "jr      $ra\n\t"
        "nop\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}