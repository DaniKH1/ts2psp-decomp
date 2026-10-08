/**
 * The Sims 2 PSP - func_0014EAAC (0x0014EAAC, 0x48 bytes)
 *
 * Writes two tag bytes and a 24-bit value into a byte-indexed stream.
 *
 *     lw   $t5, 0x8($t0)
 *     sll  $a0, $a1, 1
 *     addu $t7, $a0, $a1
 *     lui  $v1, 0xFF
 *     addiu $t8, $t7, 0x90
 *     ori  $t9, $v1, 0xFFFF
 *     addiu $a1, $t7, 0x91
 *     and  $t4, $a3, $t9
 *     addiu $t1, $t5, 0x4
 *     sll  $a3, $t8, 24
 *     sll  $t3, $a1, 24
 *     or   $t6, $a3, $t4
 *     or   $a2, $t3, $t4
 *     addiu $t2, $t1, 0x4
 *     sw   $t6, 0x0($t5)
 *     sw   $t2, 0x8($t0)
 *     jr   $ra
 *     sw   $a2, 0x0($t1)
 *
 * **`stream[index] = (byte_90 << 24) | payload; stream[index + 1] = (byte_91 << 24) |
 * payload; cursor += 8;`**
 *
 * **Two byte fields at 0x90 and 0x91 of a three-byte record**, indexed by
 * `index * 3` - `sll 1` then `addu` is a multiply by three, the same spelling
 * `func_000F93E8` uses for a stride of 28.  The tag byte is put in the *top* byte of
 * a word by `<< 24`, which works because only the low eight bits of the index
 * survive and the shift discards the rest; the payload occupies the low three
 * bytes.
 *
 * **So each record contributes a one-byte tag and the call supplies a 24-bit
 * value**, and the two are written as two complete words.  The stream pointer in
 * `*(t0 + 8)` moves on by eight, which is two words, and nothing here reads the
 * cursor - it is written once and never loaded - so the caller cannot tell from
 * this function alone whether the stream is a byte array being word-aligned or a
 * word array.
 *
 * **`and $t4, $a3, $t9` is a no-op.**  `$t9` is 0xFFFFFFFF, so masking a 32-bit value
 * by it returns the value unchanged.  The original wrote the mask out and applied
 * it anyway, which is the plainest form of dead arithmetic: there is nothing here
 * that a mask of all ones could alter.
 *
 * **The mask itself takes two instructions where one would do** - `lui $v1, 0xFF`
 * then `ori $t9, $v1, 0xFFFF` to build 0xFFFFFFFF, instead of `ori $t9, $zero, -1`,
 * whose sign-extended immediate reaches the same value.  **But this is not the
 * dead-`lui` pattern** the file could have claimed: here the `lui` is *live*, it is
 * an operand of the `ori`.  The two functions already recorded - `func_0009674C`'s
 * `lui $a2, 0x0` and the three `lui 0x2B00`s in `renderMeshInstances_1060` - are
 * different, because there the high half is loaded and then overwritten before
 * anything reads it.  Two functions with a discarded `lui`, and here a retained but
 * redundant one; counting them together would make the first pattern look commoner
 * than it is.
 *
 * **`$a3` is overwritten by a shift and that is fine** - the payload was already
 * copied out by the `and` into `$t4`, so the third argument's register is free to
 * hold the first tag byte.  Both tag bytes then get shifted in consecutive
 * instructions before either `or`, so `$t6` and `$a2` are formed together from two
 * freshly computed tags and the one payload.
 *
 * `$t0` is a third argument the C signature does not describe - the function reads
 * and writes it and never initialises it, so the caller is passing a pointer by
 * convention.
 */
#include "types.h"

/** Write two words built from tag bytes and a payload into the stream described by
 *  the first argument, and advance that stream's cursor by eight.
 *  @param stream In $t0: its word at offset 8 is the write cursor, read and updated.
 *  @param index   In $a1: which three-byte record; its bytes at +0x90 and +0x91 are
 *                 the tag bytes.
 *  @param payload In $a3: the low 24 bits of both words. */
__attribute__((noreturn)) void func_0014EAAC(void) {
    __asm__ __volatile__(
        "lw   $t5, 0x8($t0)\n\t"
        "sll  $a0, $a1, 1\n\t"
        "addu $t7, $a0, $a1\n\t"
        "lui  $v1, 0xFF\n\t"
        "addiu $t8, $t7, 0x90\n\t"
        "ori  $t9, $v1, 0xFFFF\n\t"
        "addiu $a1, $t7, 0x91\n\t"
        "and  $t4, $a3, $t9\n\t"
        "addiu $t1, $t5, 0x4\n\t"
        "sll  $a3, $t8, 24\n\t"
        "sll  $t3, $a1, 24\n\t"
        "or   $t6, $a3, $t4\n\t"
        "or   $a2, $t3, $t4\n\t"
        "addiu $t2, $t1, 0x4\n\t"
        "sw   $t6, 0x0($t5)\n\t"
        "sw   $t2, 0x8($t0)\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "sw   $a2, 0x0($t1)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$a0", "$a1", "$a3", "$v1",
          "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7", "$t8", "$t9");
}