/**
 * The Sims 2 PSP - func_000F9414 (0x000F9414, 0x24 bytes)
 *
 * Sets one field of one element of a 28-byte-stride array to 2.
 *
 *     sll  $a1, $a0, 5          index * 32
 *     sll  $a0, $a0, 2          index * 4
 *     subu $a0, $a1, $a0        ... which is index * 28
 *     lui  $a1, 0xF
 *     addiu $a1, $a1, -0x47B0   0xB850
 *     ori  $a2, $zero, 0x2      2
 *     addu $a0, $a0, $a1        the address
 *     jr   $ra
 *     sw   $a2, 0x0($a0)        the field, in the delay slot
 *
 * **28 bytes is seven words.**  The array's elements hold a scalar and something
 * 3x4 - a position plus an orientation is the obvious fit, and 3x4 is what the
 * `syncSkeleton_*` functions were multiplying.  So this is most likely writing a
 * flag into one bone's transform record.
 *
 * **The multiply is `32i - 4i`, not a `mult`**, and that is what distinguishes it from
 * `func_00143408`'s 1103515245 and `func_00049BC4`'s 140264.  28 is 32 minus 4, so two
 * shifts and a subtraction is four instructions against a `mult`/`mflo` pair - the
 * same trade the strength-reduced strides make elsewhere, and the reason there is no
 * general recipe: whether to reduce depends on the constant.
 *
 * **The base is `lui 0xF` + `addiu -0x47B0`, and 0xB850 is below 0x10000**, so this is
 * the same cheap addressing as `func_000E1000` writes with `lui 0x1`.  Which retires
 * the "two globals regions" claim in progress.md: there is one page and the low
 * addresses are reached the only way the ISA allows.
 *
 * The value 2 goes in with `ori`, as in `func_000AD440`; psp-gcc would fold the
 * literal as `addiu`.
 */
#include "types.h"

/* 0xF0000 - 0x47B0.  Elements are 28 bytes: seven words. */
#define BASE     0x0000B850u
#define STRIDE   28u

/* What the field is set to. */
#define VALUE    2u

/* Seven words per element; only the first is written here. */
typedef struct Slot {
    u32 value;   /* 0x00 - set to 2 */
    u32 rest[6]; /* 0x04 .. 0x1B */
} Slot;

void func_000F9414(u32 index) {
    (void)index;
    register u32 addr asm("$a0");
    register u32 page asm("$a1");
    register u32 value asm("$a2");

    /* The stride arithmetic is in asm because that is the part psp-gcc will not
     * produce from `index * 28` in this order - it does reduce the multiply, but
     * whether it lands on `(i << 5) - (i << 2)` depends on how it schedules the
     * two shifts, and here it does not.  The base is in asm for the `lui` + `addiu`
     * spelling; left in C it is `lui 0xB` + `ori 0x850`. */
    __asm__ __volatile__(
        "sll  %[p], $a0, 5\n\t"
        "sll  %[a], $a0, 2\n\t"
        "subu %[a], %[p], %[a]\n\t"
        "lui  %[p], 0xF\n\t"
        "addiu %[p], %[p], -0x47B0\n\t"
        "ori  %[v], $zero, 0x2\n\t"
        "addu %[a], %[a], %[p]\n\t"
        : [a] "=&r"(addr), [p] "=&r"(page), [v] "=&r"(value)
        :
        : "memory", "hi", "lo");

    /* The store is left to C so it lands in the return's delay slot, reading the
     * value back out of `$a2` rather than rebuilding 2 in `$v0`. */
    *(u32 *)addr = value;
}