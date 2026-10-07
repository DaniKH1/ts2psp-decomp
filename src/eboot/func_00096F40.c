/**
 * The Sims 2 PSP - func_00096F40 (0x00096F40, 0x14 bytes)
 *
 * Zeroes three consecutive floats through the second argument.
 *
 *     mtc1  $zero, $f12
 *     swc1  $f12, 0x0($a1)
 *     swc1  $f12, 0x4($a1)
 *     jr    $ra
 *     swc1  $f12, 0x8($a1)
 *
 * **The zero comes from the FPU, not from `$zero`.**  A word store of `$zero` would be
 * one instruction per float and needs no `mtc1` at all - `sw $zero, 0x0($a1)` works
 * perfectly well for the bit pattern of 0.0f.  That the original moves zero *into a
 * float register* and stores from there says the value is a float in the source, not an
 * integer being used as a bit pattern: the compiler was asked to store `0.0f` three
 * times and it materialised the float once and reused it.
 *
 * **Three of them, and the third is in the delay slot.**  Offsets 0x0, 0x4 and 0x8 are
 * a 12-byte run, so this is a three-float sub-structure - the obvious fit is a position
 * or a normal, cleared in one go.  `tools/duplicate_bodies.py` says four functions
 * share this exact body, so it is a small helper used in four places rather than
 * something inlined away each time.
 *
 * **The first argument is unused.**  Only `$a1` is touched, so as with
 * `func_00082134` this is a free function taking an output pointer, not a method.
 *
 * **Why `mtc1` is in the asm but the stores are not.**  The stores could be left to C if
 * GCC would use the float register, but it will not: given a `float` assignment it
 * emits its own `mtc1` from whatever register it likes.  The whole body is in asm
 * because the last store has to land in the return's delay slot anyway, and splitting
 * the block to save two lines of C would cost more in fighting the scheduler than it
 * saves.
 */
#include "types.h"

/* A three-float sub-structure. */
typedef struct Triple {
    float a;   /* 0x00 */
    float b;   /* 0x04 */
    float c;   /* 0x08 */
} Triple;

__attribute__((noreturn)) void func_00096F40(s32 unused, Triple *out) {
    (void)unused;
    /* `$f12` is the o32 first-float-argument register and is the one the original uses
     * as the carrier; naming it here keeps the block from being reordered around the
     * stores that read it. */
    /* `.set noreorder` covers the `mtc1` too, and that is the whole reason it is here.
     * Under `reorder` gas sees `mtc1 $f12` followed by a `swc1` that reads `$f12`,
     * treats it as a coprocessor load interlock, and inserts a `nop` - the original has
     * none, so the store is four bytes late and the whole function shifts. */
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "mtc1 $zero, $f12\n\t"
        "swc1 $f12, 0x0(%[t])\n\t"
        "swc1 $f12, 0x4(%[t])\n\t"
        "jr   $ra\n\t"
        "swc1 $f12, 0x8(%[t])\n\t"
        ".set reorder\n\t"
        : : [t] "r"(out)
        : "memory", "$f12", "$f13");
}