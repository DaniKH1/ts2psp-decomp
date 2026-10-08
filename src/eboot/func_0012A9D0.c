/**
 * The Sims 2 PSP - func_0012A9D0 (0x0012A9D0, 0x34 bytes)
 *
 * `syncSkeleton_22B8`, with the return value in the delay slot.
 *
 *     lv.s    S200, 0x0($a0)      four floats from the first operand
 *     lv.s    S230, 0xC($a0)
 *     lv.s    S201, 0x0($a1)      four floats from the second
 *     lv.s    S231, 0xC($a1)
 *     vqmul.q R100, R200, R201
 *     svr.q   R100, 0x0($a0)
 *     svl.q   R100, 0xC($a0)
 *     jr      $ra
 *     move    $v0, $a0
 *
 * **`out = a * b`, component-wise**, on the vector unit.  The mnemonic is `vqmul.q`;
 * **whether it clamps is not established from the bytes**, because this module contains
 * no saturating vector instruction at all - see `syncSkeleton_22B8.c`, which spells out
 * what the module's own naming does and does not show.
 *
 * This and `syncSkeleton_22B8` are the same function differing in exactly one
 * instruction, and both are fifty-two bytes.  The thirteenth instruction - the one
 * in `jr $ra`'s delay slot - is `nop` there and `move $v0, $a0` here.  Nothing else
 * differs: same operands, same `vqmul.q`, same staggered `svr.q`/`svl.q` pair at the
 * same offsets, same length.
 *
 * **So this is the cheapest pair of related functions in the module.**  The pair
 * `func_00123588` / `func_00123560` differs by eight bytes and three instructions of
 * genuinely different work; `func_00110314` / `func_0010FFF4` differs by a constant and
 * a payload type; **this pair differs only in what the function hands back, and the
 * return costs nothing because the delay slot was going to be a `nop` anyway.**
 *
 * **They are also in different translation units** - `syncSkeleton` and the unlabelled
 * unit `func_0012A9D0` is in - **and that is worth saying carefully, because it is a
 * naming fact and not a unit-boundary one.**  `updateNodeGraph_0E48.c` records the
 * opposite caution at length: a missing symbol name is not evidence about a header.
 * Here both functions are byte-identical modulo one instruction, **so whatever produced
 * them was available to both, and the names only tell us the linker kept one of them.**
 *
 * **Only the operand registers differ**: `$a0` and `$a1` here against `$a1` and `$a2`
 * in the sibling, because this one writes through its first argument.  That is a real
 * difference in the calling convention between the two and not a naming artefact.
 *
 * The `svr.q`/`svl.q` staggering is the same unresolved question as in
 * `syncSkeleton_22B8.c`: encoded immediates 3 and 13, printed as 0 and 0xC, twelve
 * bytes apart for two halves of one quad.  `tools/vfpu_split_store.py` finds five
 * instances in the module and every one uses those same two offsets, **so the module
 * cannot say which half each store writes and this file does not guess.**
 */
#include "types.h"

/** Write `a * b` through `$a0`, component-wise, and return `$a0`.
 *  @param out In $a0: receives four floats, through a staggered store pair, and is
 *              returned.
 *  @param b   In $a1: four floats. */
__attribute__((noreturn)) void *func_0012A9D0(void *out, void *b) {
    (void)out;
    (void)b;
    __asm__ __volatile__(
        "lv.s    S200, 0x0($a0)\n\t"
        "lv.s    S210, 0x4($a0)\n\t"
        "lv.s    S220, 0x8($a0)\n\t"
        "lv.s    S230, 0xC($a0)\n\t"
        "lv.s    S201, 0x0($a1)\n\t"
        "lv.s    S211, 0x4($a1)\n\t"
        "lv.s    S221, 0x8($a1)\n\t"
        "lv.s    S231, 0xC($a1)\n\t"
        "vqmul.q R100, R200, R201\n\t"
        "svr.q   R100, 0x0($a0)\n\t"
        "svl.q   R100, 0xC($a0)\n\t"
        ".set noreorder\n\t"
        "jr      $ra\n\t"
        "move    $v0, $a0\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$v0");
}