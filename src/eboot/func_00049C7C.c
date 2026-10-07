/**
 * The Sims 2 PSP - func_00049C7C (0x00049C7C, 0x30 bytes)
 *
 * The same 2304-byte stride as func_00049C50, with the running total kept in
 * `$a0` and a further `0x808` added on return.
 *
 *     sll  $a0, $a0, 8       i * 256
 *     addu $a1, $zero, $a0
 *     sll  $a0, $a0, 3
 *     addu $a0, $a1, $a0     i * 2304
 *     lui  $a1, 0x7
 *     addiu $a1, $a1, 0x43A0     0x743A0
 *     addu $a0, $a0, $a1     + 0x743A0
 *     lui  $a1, 0x6
 *     addiu $a1, $a1, 0x6BC4     0x66BC4
 *     addu $v0, $a0, $a1     + 0x66BC4
 *     jr   $ra
 *     addiu $v0, $v0, 0x808      + 0x808, in the delay slot
 *
 * The stride arithmetic is identical to func_00049C50's; what differs is where the
 * total is accumulated.  There, the sum of `i * 2304` and the first address went
 * straight into `$v0` and the second address was added at the end.  Here the
 * accumulation stays in `$a0` for one step longer and only reaches `$v0` for the
 * last add.  Both are the same computation; the difference is register pressure
 * and where the compiler found a register free.
 *
 * The `0x808` is the interesting one, and what it means is **not** decidable from
 * this function.  The two base addresses sum to `0x743A0 + 0x66BC4 = 0xDAF64`, so
 * this returns `index * 2304 + 0xDB76C`.  Whether `0x808` is a field offset inside
 * each 2304-byte entry or a third separate constant in the base is exactly the kind
 * of thing this function cannot tell us - both readings produce identical
 * arithmetic, and 0x808 is comfortably smaller than the stride so there is no
 * arithmetic constraint to break the tie.  The neighbouring func_0004AA60 adds 0x4
 * instead, which does suggest a family of accessors for specific fields, but that
 * is a guess from the shape rather than something the code establishes.
 *
 * What *is* solid: the entry size is 2304 bytes, large enough to be a whole
 * subsystem's worth of per-object state or a packed vertex buffer with everything
 * inline.  And 0x808 landing in the return's delay slot is the compiler filling the
 * slot with the last piece of arithmetic, as it did in func_0009D5BC.
 */
#include "types.h"

/* The element size the compiler derived: 9 * 256. */
#define ELEMENT_SIZE   2304

void *func_00049C7C(u32 index) {
    register u32 x asm("$a0") = index;
    register u32 saved asm("$a1");
    register void *result asm("$v0");

    __asm__ __volatile__(
        "sll  %[x], %[x], 8\n\t"
        "addu %[saved], $zero, %[x]\n\t"
        "sll  %[x], %[x], 3\n\t"
        "addu %[x], %[saved], %[x]\n\t"
        "lui  %[saved], 0x7\n\t"
        "addiu %[saved], %[saved], 0x43A0\n\t"
        "addu %[x], %[x], %[saved]\n\t"
        "lui  %[saved], 0x6\n\t"
        "addiu %[saved], %[saved], 0x6BC4\n\t"
        "addu %[r], %[x], %[saved]\n\t"
        "addiu %[r], %[r], 0x808\n\t"
        : [x] "+r"(x), [saved] "+r"(saved), [r] "=&r"(result)
        :
        : "memory");

    return result;
}