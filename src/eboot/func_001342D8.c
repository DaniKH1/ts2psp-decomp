/**
 * The Sims 2 PSP - func_001342D8 (0x001342D8, 0x40 bytes)
 *
 * Appends a 16-bit value to a growable byte buffer, **high byte first**, and
 * advances a length counter twice - once per byte.
 *
 *     addiu $a2, $a0, 0x14        the counter's address, computed once
 *     lw    $a3, 0x0($a2)
 *     srl   $t0, $a1, 8            the high byte
 *     addiu $t1, $a3, 0x1
 *     sw    $t1, 0x0($a2)
 *     lw    $t1, 0x8($a0)         the buffer
 *     addu  $a3, $t1, $a3          ... plus the old length
 *     sb    $t0, 0x0($a3)          the high byte goes in
 *     lw    $a3, 0x0($a2)          the counter, read back
 *     andi  $a1, $a1, 0xFF         the low byte
 *     addiu $t0, $a3, 0x1
 *     sw    $t0, 0x0($a2)
 *     lw    $a0, 0x8($a0)          the buffer, read back
 *     addu  $a0, $a0, $a3
 *     jr    $ra
 *     sb    $a1, 0x0($a0)          the low byte goes in
 *
 * **The structure is `{ void *data at 0x08, u32 length at 0x14 }`** - and the two fields
 * are twelve bytes apart, so there is a `capacity` or a `reserved` field in between at
 * 0x0C.  Both members are re-loaded rather than kept in a register across the first
 * store: `lw $a3, 0x0($a2)` appears twice and `lw $t1/$a0, 0x8($a0)` twice.  A compiler
 * with the value in a register would have used it; the re-reads are the signature of
 * CodeWarrior being conservative about aliasing, since `data` is a pointer it cannot
 * prove does not point back into the structure.
 *
 * **High byte first is the whole content of this function.**  The argument is a 16-bit
 * quantity and it lands in the buffer most-significant byte first, which is *not* how a
 * little-endian machine would store a `u16`.  So this is not a `u16` being written -
 * it is a byte buffer receiving two explicitly chosen bytes, and the order is a decision
 * the source made.  Network byte order, a text encoding, or a debug dump all fit; what
 * does not fit is "the compiler wrote a short".
 *
 * **The length goes up by one per byte rather than by two at the end.**  That is
 * consistent with the source being two calls to an 8-bit append rather than one call to
 * a 16-bit one - the 16-bit version would increment once and store two bytes with a
 * `sh`, which costs two instructions fewer than what is here.  So the shape that fits is
 * a `push_back(u8)` inlined twice, and the compiler never merged them because the byte
 * order differs from what a `sh` would produce.
 *
 * **This shape appears nowhere else in the module.**  A scan for the tell - a `srl` of an
 * argument by 8 feeding a byte store - finds eight functions, and only this one is
 * small enough to be the append itself; the other seven are unrelated code that happens
 * to shift an argument.  So the big-endian append is a single site, not a convention.
 */
#include "types.h"

typedef struct Buffer {
    u32   reserved[2];  /* 0x00 .. 0x07 */
    u8   *data;         /* 0x08 - the bytes */
    u32   spare;        /* 0x0C - capacity or reserved */
    u32   length;       /* 0x14 - how many bytes are in use */
} Buffer;

/* Deliberate on a function that does return: it tells GCC not to emit an epilogue,
 * because the block below already contains the `jr $ra` and the instruction in its
 * delay slot. */
__attribute__((noreturn)) void func_001342D8(Buffer *buf, u16 value) {
    /* Inputs in their original registers: $a0 = buf, $a1 = value.
     * All working registers are listed as clobbers so GCC knows they're modified. */
    (void)buf; (void)value;
    __asm__ __volatile__(
        "addiu $a2, $a0, 0x14\n\t"
        "lw    $a3, 0x0($a2)\n\t"
        "srl   $t0, $a1, 8\n\t"
        "addiu $t1, $a3, 0x1\n\t"
        "sw    $t1, 0x0($a2)\n\t"
        "lw    $t1, 0x8($a0)\n\t"
        "addu  $a3, $t1, $a3\n\t"
        "sb    $t0, 0x0($a3)\n\t"
        "lw    $a3, 0x0($a2)\n\t"
        "andi  $a1, $a1, 0xFF\n\t"
        "addiu $t0, $a3, 0x1\n\t"
        "sw    $t0, 0x0($a2)\n\t"
        "lw    $a0, 0x8($a0)\n\t"
        "addu  $a0, $a0, $a3\n\t"
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "sb    $a1, 0x0($a0)\n\t"
        ".set reorder\n\t"
        :
        : "r"(buf), "r"(value)
        : "memory", "$a2", "$a3", "$t0", "$t1");
}