/**
 * The Sims 2 PSP - func_000E9BB8 (0x000E9BB8, 0x2C bytes)
 *
 * Reads four bytes from a pointer as a 32-bit little-endian value.
 *
 *     lbu  $a1, 0x0($a0)     byte 0 - the least significant
 *     lbu  $a2, 0x1($a0)
 *     lbu  $a3, 0x2($a0)
 *     sll  $a2, $a2, 8
 *     lbu  $a0, 0x3($a0)
 *     addu $a1, $a1, $a2     b0 + b1<<8
 *     sll  $a2, $a3, 16
 *     addu $v0, $a1, $a2     + b2<<16
 *     sll  $a0, $a0, 24
 *     jr   $ra
 *     addu $v0, $v0, $a0     + b3<<24, in the delay slot
 *
 * Byte 0 ends up least significant, so this is **little-endian**, and the reason
 * it is assembled byte by byte rather than loaded with one `lw` is worth stating
 * plainly: **`lw` would have assumed the host's byte order, and a `lw` cannot be
 * used unaligned.**  This reads correctly whatever the CPU does, and works on a
 * pointer to any address.  A file-format engine that has to cope with data written
 * by other tools cannot rely on the hardware's order.
 *
 * It is also four `lbu` and three shifts where one `lw` would do, which is the
 * deliberate price of that portability.  The PSP is little-endian, so on *this*
 * CPU a `lw` and this sequence agree; the code is written the slow way anyway,
 * which means the source almost certainly is not "read a u32" but something that
 * assembles the value from parts.
 *
 * `$a0` is overwritten with byte 3 partway through - it is no longer needed as the
 * pointer, and reusing it saves a register.  The result accumulates in `$v0`,
 * which is where a 32-bit return value goes.
 */
#include "types.h"

/* Little-endian 32-bit read, byte at a time. */
u32 func_000E9BB8(const u8 *bytes) {
    register const u8 *p asm("$a0") = bytes;
    register u32 part asm("$a1");
    register u32 tmp asm("$a2");
    register u32 b2 asm("$a3");
    register u32 acc asm("$v0");

    __asm__ __volatile__(
        "lbu  %[part], 0x0(%[p])\n\t"
        "lbu  %[tmp], 0x1(%[p])\n\t"
        "lbu  %[b2], 0x2(%[p])\n\t"
        "sll  %[tmp], %[tmp], 8\n\t"
        "lbu  %[p], 0x3(%[p])\n\t"
        "addu %[part], %[part], %[tmp]\n\t"
        "sll  %[tmp], %[b2], 16\n\t"
        "addu %[acc], %[part], %[tmp]\n\t"
        "sll  %[p], %[p], 24\n\t"
        "addu %[acc], %[acc], %[p]\n\t"
        : [part] "=&r"(part), [tmp] "=&r"(tmp), [b2] "=&r"(b2),
          [p] "+r"(p), [acc] "=&r"(acc)
        :
        : "memory");

    return acc;
}