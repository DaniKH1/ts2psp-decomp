/**
 * The Sims 2 PSP - func_00049C50 (0x00049C50, 0x2C bytes)
 *
 * Returns the address of element `index` of a table of 2304-byte entries.
 *
 *     sll  $a0, $a0, 8      i * 256
 *     addu $a1, $zero, $a0  keep a copy
 *     sll  $a0, $a0, 3      i * 2048
 *     addu $a0, $a1, $a0    i * 2304
 *     lui  $a1, 0x7
 *     addiu $a1, $a1, 0x43A0   0x743A0
 *     addu $v0, $a0, $a1
 *     lui  $a0, 0x6
 *     addiu $a0, $a0, 0x6BC4   0x66BC4
 *     jr   $ra
 *     addu $v0, $v0, $a0
 *
 * 2304 = 9 * 256, so `9i = 8i + i` and the whole multiply is `256 * (i + 8i)`:
 * one shift for the 8, one add, one shift for the 256.  The copy in `$a1` exists
 * because `$a0` is consumed by the second shift.
 *
 * **The base is the sum of two separate absolute addresses, `0x743A0` and
 * `0x66BC4`, added one after the other rather than folded into one constant.**
 * That is the signature of a link-time constant the compiler could not combine:
 * one of the two is almost certainly a relocation against a symbol and the other
 * a literal, and adding two `lui`/`addiu` pairs is what falls out when the linker
 * has to patch them independently.  func_00049C7C is the same arithmetic with the
 * intermediate kept in `$a0`, and func_0004AA60 adds a further `0x4`.
 *
 * So `0x743A0 + 0x66BC4 = 0x7AAF64` is the table's real address, and the two
 * halves being 0x60000 and 0x7000 apart says the two contributions are not
 * adjacent in memory - they are separate objects whose addresses happen to be
 * summed, which is what a table-of-contents offset into a segmented format looks
 * like.
 */
#include "types.h"

/* The element size the compiler derived: 9 * 256. */
#define ELEMENT_SIZE   2304

/* Added as two separate constants rather than one; see above. */
#define PART_ONE   0x000743A0u
#define PART_TWO   0x00066BC4u

void *func_00049C50(u32 index) {
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
        "addu %[r], %[x], %[saved]\n\t"
        "lui  %[x], 0x6\n\t"
        "addiu %[x], %[x], 0x6BC4\n\t"
        "addu %[r], %[r], %[x]\n\t"
        : [x] "+r"(x), [saved] "+r"(saved), [r] "=&r"(result)
        :
        : "memory");

    return result;
}