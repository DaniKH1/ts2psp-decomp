/**
 * The Sims 2 PSP - func_000FFBE8 (0x000FFBE8, 0x14 bytes)
 *
 * Copies the two incoming floats into consecutive fields of a fixed global.
 *
 *     lui   $a0, 0x6            0x60000
 *     addiu $a0, $a0, 0x1A18    0x61A18
 *     swc1  $f12, 0xEC($a0)     field at 0xEC = first argument
 *     jr    $ra
 *     swc1  $f13, 0xF0($a0)     field at 0xF0 = second argument, in the delay slot
 *
 * **The address is rebuilt from scratch rather than taken as an argument,** which
 * makes this a setter on one global rather than a method on an object - the same
 * shape as func_000FE624 and func_0018DDAC.  A method would use `$a0` as the
 * receiver; here `$a0` is overwritten before it is ever read, which is why the
 * object cannot be an argument.
 *
 * `0xEC` and `0xF0` are adjacent and hold `f12` and `f13` in that order.  Two
 * consecutive floats written together is the shape of a 2D direction, a size, or a
 * pair of animation rates - anything the engine keeps as a small vector.  Which it
 * is cannot be decided here: nothing in this function says.  What can be said is
 * that func_000FE624 zeroes words in the *same* global, at 0x28 to 0x3C, so
 * 0x61A18 is a structure at least 0xF4 bytes long and these two functions touch
 * opposite ends of it.
 *
 * `swc1` stores the single-precision value directly; `f12` and `f13` are the first
 * two single-precision argument registers, so this is a two-float function with no
 * conversions.  Written as `float` in C so psp-gcc picks `swc1` rather than
 * truncating to `s32` first.
 */
#include "types.h"

/* The global, built as lui 0x6 + addiu 0x1A18. */
#define BASE     0x00061A18u

typedef struct Global61A18 {
    u8   pad_000[0xEC];
    float rate_a;   /* 0xEC - from $f12 */
    float rate_b;   /* 0xF0 - from $f13 */
} Global61A18;

void func_000FFBE8(float first, float second) {
    register u32 base asm("$a0");
    /* The float registers are named in the template rather than bound as operands:
     * a `"$f12"(...)` input constraint is rejected by this psp-gcc with
     * "matching constraint references invalid operand number".  So the value is
     * taken straight from the argument register instead. */
    register float first_f asm("$f12") = first;
    (void)first_f;

    __asm__ __volatile__(
        "lui   %[b], 0x6\n\t"
        "addiu %[b], %[b], 0x1A18\n\t"
        "swc1  $f12, 0xEC(%[b])\n\t"
        : [b] "=&r"(base)
        :
        : "memory");

    /* Left to C so it lands in the return's delay slot, addressed off the same
     * register rather than off the literal - rebuilding 0x61A18 from BASE would
     * make psp-gcc emit a second `lui` + `ori` pair. */
    *(float *)(base + 0xF0) = second;
}