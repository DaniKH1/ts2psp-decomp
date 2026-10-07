/**
 * The Sims 2 PSP - func_0012EB30 (0x0012EB30, 0x20 bytes)
 *
 * Returns the address of element `index` of an array of 164-byte elements that
 * lives at the fixed module address `0x1DEE08`.
 *
 *     sll  $a1, $a0, 5      i * 32
 *     addu $a0, $a0, $a1    i * 33
 *     sll  $a0, $a0, 2      i * 132
 *     addu $v0, $a1, $a0    i * 164
 *     lui  $a0, 0x1E
 *     addiu $a0, $a0, -0x11F8    0x1DEE08
 *     jr   $ra
 *     addu $v0, $v0, $a0    ... and the return is index * 164 + that
 *
 * Third derivation of the same strength reduction, and it completes the picture
 * from the other two:
 *
 *     92  = 31 * 4 - 32            from a subtraction
 *     168 = 3 * 56                 from a doubling
 *     164 = 4 * 33 + 32            from a shift and an add
 *
 * All three cost four instructions and avoid `mult`.  164 = 4 * 41 and 41 is one
 * above a power of two, so `4*(32+1)*i + 32i` falls out of two `sll`s and one
 * `addu` - the compiler chose the decomposition that left it with the fewest
 * operations, and there is still no formula to apply, only a per-constant search.
 *
 * **The array is at an absolute address with no reference to the object.**  There
 * is no `this` argument at all; the base comes from `lui`/`addiu` alone.  So
 * unlike func_00097B54, where the array sat at offset 0x88 inside an object passed
 * in, this is a single global array indexed by a plain integer.  Combined with
 * func_00049C50 and func_00049C7C, which address the same stride-2304 array at
 * `0x743A0 + 0x66BC4`, the engine keeps several of these.
 *
 * `0x1DEE08` is in the same 0x1E page as the globals found so far
 * (`0x1DAA88`, `0x1E52A0`, `0x1DA398`), which makes that page the module's
 * writable data.
 */
#include "types.h"

/* The element size the compiler derived: 164. */
#define ELEMENT_SIZE   164

/* The array's address: lui 0x1E (0x1E0000) + addiu -0x11F8 = 0x1DEE08. */
#define ARRAY_AT       0x0001DEE08u

void *func_0012EB30(u32 index) {
    /* `$a0` holds the index for the first four instructions and the address page
     * for the last two, so it is one variable rather than two - GCC rejects two
     * outputs in the same hard register even when they are used at different
     * times. */
    register u32 x asm("$a0") = index;
    register u32 scaled asm("$a1");
    register void *result asm("$v0");

    __asm__ __volatile__(
        "sll  %[s], %[x], 5\n\t"
        "addu %[x], %[x], %[s]\n\t"
        "sll  %[x], %[x], 2\n\t"
        "addu %[r], %[s], %[x]\n\t"
        "lui  %[x], 0x1E\n\t"
        "addiu %[x], %[x], -0x11F8\n\t"
        "addu %[r], %[r], %[x]\n\t"
        : [x] "+r"(x), [s] "+r"(scaled), [r] "=&r"(result)
        :
        : "memory");

    return result;
}