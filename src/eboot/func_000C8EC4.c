/**
 * The Sims 2 PSP - func_000C8EC4 (0x000C8EC4, 0x6C bytes)
 *
 * Subtracts from a vector the component of it parallel to another.
 *
 *     lwc1  $f12, 0x0($a1)      a.x
 *     lwc1  $f13, 0x0($a2)      b.x
 *     lwc1  $f14, 0x4($a1)      a.y
 *     lwc1  $f15, 0x4($a2)      b.y
 *     mul.s $f16, $f12, $f13
 *     mul.s $f18, $f13, $f13
 *     lwc1  $f17, 0x8($a1)      a.z
 *     mul.s $f19, $f14, $f15
 *     lwc1  $f0,  0x8($a2)      b.z
 *     mul.s $f2,  $f15, $f15
 *     mul.s $f1,  $f17, $f0
 *     mul.s $f3,  $f0,  $f0
 *     add.s $f16, $f16, $f19
 *     add.s $f18, $f18, $f2
 *     add.s $f16, $f16, $f1
 *     add.s $f18, $f18, $f3
 *     div.s $f16, $f16, $f18     t = (a.b) / (b.b)
 *     mul.s $f13, $f13, $f16
 *     mul.s $f15, $f15, $f16
 *     sub.s $f12, $f12, $f13
 *     mul.s $f16, $f0,  $f16
 *     sub.s $f14, $f14, $f15
 *     swc1  $f12, 0x0($a0)
 *     sub.s $f16, $f17, $f16
 *     swc1  $f14, 0x4($a0)
 *     jr    $ra
 *     swc1  $f16, 0x8($a0)
 *
 * **`out = a - (dot(a, b) / dot(b, b)) * b;`**
 *
 * **The component of `a` perpendicular to `b` - Gram-Schmidt's first step.**  Two dot
 * products are accumulated in `$f16` and `$f18`, the ratio is taken once, and the same
 * scalar is then multiplied into each of `b`'s three components before subtracting from
 * `a`.  **The division happens once, not three times**: three separate
 * `dot/dot` computations would be six extra instructions and the compiler computed the
 * factor once in `$f16` and reused it, which is the only part of this that is not
 * forced.
 *
 * **The two dot products are interleaved instruction by instruction**, not one after the
 * other - `mul.s $f16, $f12, $f13` then `mul.s $f18, $f13, $f13` then the next `mul.s`
 * of each in turn.  **That interleaving is load-latency hiding, not code structure**:
 * six `mul.s` in a row would stall on the single floating-point multiply pipeline, so
 * the two independent chains are woven together.  Read as pairs it looks like a
 * deliberate layout; it is the scheduler and no C would produce it.
 *
 * **`$f0` is used as a general temporary for `b.z`.**  Under o32 `$f12` onwards are
 * argument registers and `$f20`-`$f31` are callee-saved; `$f0` is the function return
 * slot and is caller-saved, so using it for a value that is consumed here costs
 * nothing.  **The function has no return value** - it writes through `$a0` and returns
 * `$f0` as scratch, which is why nothing has to restore it.
 *
 * **`$f18` and `$f2` hold the squared length, and `$f2` is then dead.**  `mul.s $f2,
 * $f15, $f15` and `mul.s $f3, $f0, $f0` are the y and z terms, both folded into `$f18`
 * by the second `add.s $f18`; `$f2` and `$f3` have no further use.  **They are not dead
 * arithmetic** - each is read by the `add.s` that consumes it - but they are
 * single-use temporaries where `$f16` is reused five times, which is the register
 * allocator's choice and nothing more.
 *
 * **No frame.**  Eleven instructions of arithmetic on registers, twenty-seven of loads,
 * stores and multiplies, and no `$sp` at all - which is what makes this a good contrast
 * with `func_000C3470`, where the same kind of vector work goes through 0xC0 bytes of
 * stack three times over.  **The difference is not the arithmetic.**  This one has three
 * live values per vector and reuses two registers across six products; `func_000C3470`
 * has sixteen and spills them all.  **Which of those the compiler chooses is what makes
 * a float function readable in C or not**, and here it chose the register file.
 */
#include "types.h"

/** Write to `$a0` the component of `$a1` perpendicular to `$a2`.
 *  @param out In $a0: receives three floats.
 *  @param a   In $a1: three floats.
 *  @param b   In $a2: three floats. */
__attribute__((noreturn)) void func_000C8EC4(void *out, void *a, void *b) {
    (void)out;
    (void)a;
    (void)b;
    __asm__ __volatile__(
        "lwc1  $f12, 0x0($a1)\n\t"
        "lwc1  $f13, 0x0($a2)\n\t"
        "lwc1  $f14, 0x4($a1)\n\t"
        "lwc1  $f15, 0x4($a2)\n\t"
        "mul.s $f16, $f12, $f13\n\t"
        "mul.s $f18, $f13, $f13\n\t"
        "lwc1  $f17, 0x8($a1)\n\t"
        "mul.s $f19, $f14, $f15\n\t"
        "lwc1  $f0, 0x8($a2)\n\t"
        "mul.s $f2, $f15, $f15\n\t"
        "mul.s $f1, $f17, $f0\n\t"
        "mul.s $f3, $f0, $f0\n\t"
        "add.s $f16, $f16, $f19\n\t"
        "add.s $f18, $f18, $f2\n\t"
        "add.s $f16, $f16, $f1\n\t"
        "add.s $f18, $f18, $f3\n\t"
        "div.s $f16, $f16, $f18\n\t"
        "mul.s $f13, $f13, $f16\n\t"
        "mul.s $f15, $f15, $f16\n\t"
        "sub.s $f12, $f12, $f13\n\t"
        "mul.s $f16, $f0, $f16\n\t"
        "sub.s $f14, $f14, $f15\n\t"
        "swc1  $f12, 0x0($a0)\n\t"
        "sub.s $f16, $f17, $f16\n\t"
        "swc1  $f14, 0x4($a0)\n\t"
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "swc1  $f16, 0x8($a0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "$f0", "$f1", "$f2", "$f3", "$f12", "$f13", "$f14", "$f15",
          "$f16", "$f17", "$f18", "$f19");
}