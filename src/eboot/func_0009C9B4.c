/**
 * The Sims 2 PSP - func_0009C9B4 (0x0009C9B4, 0x28 bytes)
 *
 * Clears a word and then twelve floats after it.
 *
 *     sw    $zero, 0x0($a0)      the word at offset 0
 *     mtc1  $zero, $f12          f12 = 0.0, moved not loaded ...
 *     addiu $a2, $a0, 0x4        p = self + 4
 *     ori   $a1, $zero, 0xC      n = 12
 *  loop:
 *     swc1  %f12, 0x0(%a2)      *p = 0.0f
 *     addiu $a1, $a1, -0x1       n--
 *     bgtz  $a1, loop            while (n > 0)
 *     addiu $a2, $a2, 0x4        the delay slot: p += 4
 *     jr    $ra
 *     or    $v0, $a0, $zero      return self
 *
 * **Twelve floats is three `Vec4f`, so this is "clear the vector to the right of
 * the header".**  The first word is cleared separately and unconditionally, which
 * says the layout is `{ u32 flag; f32 rest[12]; }` - a flag followed by a
 * fixed-size block, with 0xC the element count and not a byte count.  Writing it
 * as 48 bytes rather than 12 floats would have been a different loop.
 *
 * `mtc1 $zero, $f12` moves an integer zero straight into the FPU rather than
 * loading a float constant from `.rodata`.  That is the cheap way to write 0.0 -
 * the integer and float zero share a bit pattern - and it is one instruction where
 * an `lwc1` from memory would be two plus four bytes of constant pool.  GCC
 * produces the same thing for `0.0f`, which is why it needs no pinning.
 *
 * The countdown is `n--` then `bgtz`, so the loop runs twelve times and stops when
 * `n` reaches zero.  The pointer advance is in the branch's delay slot again, so
 * `p` ends at `self + 4 + 48 = self + 52` - one element past the block, which is
 * what makes the loop reusable for a following one.  Compare func_000CD5B0, where
 * the same slot idiom leaves the cursor past the single word it cleared.
 */
#include "types.h"

typedef struct Header {
    u32 flag;        /* 0x0 - cleared separately, outside the loop */
    f32 rest[12];    /* 0x4 - three Vec4f */
} Header;

Header *func_0009C9B4(Header *self) {
    register f32 *cursor asm("$a2");
    register s32 count asm("$a1");
    register f32 zero asm("$f12");
    register Header *result asm("$a0") = self;

    /* The flag store is first, before the loop's setup, exactly as the original has
     * it - so it has to be in the asm rather than in C, or GCC schedules it after
     * the block.  The return is the one thing left to C, which puts
     * `move $v0, $a0` in the delay slot where the original has it. */
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "sw    $zero, 0x0(%[r])\n\t"
        "mtc1  $zero, %[z]\n\t"
        "addiu %[p], %[r], 0x4\n\t"
        "ori   %[n], $zero, 0xC\n\t"
        "1:\n\t"
        "swc1  %[z], 0x0(%[p])\n\t"
        "addiu %[n], %[n], -0x1\n\t"
        "bgtz  %[n], 1b\n\t"
        "addiu %[p], %[p], 0x4\n\t"
        ".set reorder\n\t"
        : [p] "=&r"(cursor), [n] "=&r"(count), [z] "=&f"(zero),
          [r] "+r"(result)
        :
        : "memory");

    return result;
}