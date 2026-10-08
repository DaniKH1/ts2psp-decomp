/**
 * The Sims 2 PSP - func_001A9CBC (0x001A9CBC, 0x18 bytes)
 *
 * Clears two flag bits and stores a float.
 *
 *     lw    $a1, 0x18($a0)
 *     addiu $a2, $zero, -0x3001
 *     swc1  $f12, 0x1C($a0)
 *     and   $a1, $a1, $a2
 *     jr    $ra
 *     sw    $a1, 0x18($a0)
 *
 * **`self->flags_18 &= ~0x3000; self->float_1C = v;`**  - clears bits 12 and 13
 * together, and stores whatever is in `$f12`.
 *
 * **`$f12` is read but never written**, which makes this the one accessor in the
 * cluster whose float operand is not a constant.  On this ABI the first
 * floating-point argument arrives in `$f12`, so this is a function that takes a
 * float and assigns it to the field - the `lui`/`mtc1` pair that materialises
 * -1.0f in `func_001A9C98` and `func_001A9CD4` is simply absent here, and the
 * incoming value is stored instead.  Written as `void f(void *self, f32 v)`, the
 * value has to be pinned to `$f12` for the block, since nothing else in the
 * function mentions it and GCC would otherwise be free to choose the register.
 *
 * **This is the six-instruction form.**  `tools/flag_accessors.py` separates it from
 * the nine-instruction ones for exactly this reason: it is the shape that tells you
 * the field is assigned from a parameter, and folding the two together would lose
 * that.
 *
 * `-0x3001` is `0xFFFFCFFF`, which is `~0x3000` - the same off-by-one-the-eye reading
 * as its neighbours, and the reason the mask is not the literal the bit pattern
 * suggests.
 */
#include "types.h"

/** Clear bits 12 and 13 (0x3000) of the flags word at offset 0x18 of the first
 *  argument, and store the second argument into the float at offset 0x1C. */
__attribute__((noreturn)) void func_001A9CBC(void *a0, f32 v) {
    /* Pinned so that `%[v]` is `$f12`, which is where the value already is. */
    register f32 pinned asm("$f12") = v;
    (void)a0;
    __asm__ __volatile__(
        "lw    $a1, 0x18($a0)\n\t"
        "addiu $a2, $zero, -0x3001\n\t"
        "swc1  %[v], 0x1C($a0)\n\t"
        "and   $a1, $a1, $a2\n\t"
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "sw    $a1, 0x18($a0)\n\t"
        ".set reorder\n\t"
        : : [v] "f" (pinned)
        : "memory", "$a1", "$a2");
}