/**
 * The Sims 2 PSP - func_0009D5BC (0x0009D5BC, 0x20 bytes)
 *
 * The same 92-byte stride as func_0009D658, storing two floats instead of one
 * word.
 *
 *     sll  $a2, $a1, 5
 *     subu $a1, $a2, $a1
 *     sll  $a1, $a1, 2
 *     subu $a1, $a1, $a2        the stride, 92
 *     addu $a0, $a0, $a1        cursor = base + index * 92
 *     swc1 $f12, 0xA4($a0)      the value at A4 first ...
 *     jr   $ra
 *     swc1 $f12, 0xA0($a0)      ... then at A0, in the delay slot
 *
 * The float arrives in `$f12`, the first float-argument register, and is the same
 * value written to two adjacent offsets.  **The order is the reverse of memory
 * order** - 0xA4 is stored before 0xA0 - which means the compiler was working from
 * a two-element source expression whose evaluation order is right to left.  Nothing
 * depends on it, so it is free information about the original source's shape.
 *
 * Two adjacent floats at 0xA0 and 0xA4 with 0xB0 written by the sibling function
 * makes this look like a small vector pair alongside a separate word, and the pair
 * is written high-half-first.  If the two floats are a vector, then `x` at 0xA0
 * and `y` at 0xA4 are being set to the same value - a diagonal fill, which is what
 * an initialiser or a reset would do.
 *
 * See func_0009D658 for the stride arithmetic and for why 92 is a stride in one
 * part of the structure rather than the size of the thing being written.
 */
#include "types.h"

void func_0009D5BC(void *self, u32 index, f32 value) {
    register void *base asm("$a0") = self;
    register u32 i asm("$a1") = index;
    register u32 scaled asm("$a2");
    register f32 v asm("$f12") = value;

    __asm__ __volatile__(
        "sll  %[s], %[i], 5\n\t"
        "subu %[i], %[s], %[i]\n\t"
        "sll  %[i], %[i], 2\n\t"
        "subu %[i], %[i], %[s]\n\t"
        "addu %[b], %[b], %[i]\n\t"
        "swc1 %[v], 0xA4(%[b])\n\t"
        : [i] "+r"(i), [s] "=&r"(scaled), [b] "+r"(base), [v] "+f"(v)
        :
        : "memory");

    *(f32 *)((u8 *)base + 0xA0) = v;
}