/**
 * The Sims 2 PSP - func_000706A8 (0x000706A8, 0x4C bytes)
 *
 * Packs two channels of a packed word into a 16-bit value with the top bit set.
 *
 *     lui  $a1, 0xFF
 *     and  $a1, $a0, $a1
 *     andi $a2, $a0, 0xFF00
 *     srl  $a1, $a1, 19
 *     srl  $a2, $a2, 11
 *     andi $a2, $a2, 0xFFFF
 *     sll  $a1, $a1, 10
 *     andi $a0, $a0, 0xFF
 *     ori  $a3, $zero, 0x8000
 *     andi $a1, $a1, 0xFFFF
 *     sll  $a2, $a2, 5
 *     srl  $a0, $a0, 3
 *     andi $a2, $a2, 0xFFFF
 *     addu $a1, $a3, $a1
 *     andi $a0, $a0, 0xFFFF
 *     addu $a1, $a1, $a2
 *     addu $v0, $a1, $a0
 *     jr   $ra
 *     andi $v0, $v0, 0xFFFF
 *
 * **`return 0x8000 | (((a0 >> 3) & 0x1F)) | ((((a0 >> 11) & 0x1F) << 5));`**
 *
 * Both channels are reduced from eight bits to five by a right shift of 3, the
 * upper one is placed at bit 5, and the constant 0x8000 goes in at the top - which
 * is how the PSP's video formats carry "this pixel has an alpha bit" as a flag in
 * bit 15 rather than as an eight-bit channel.
 *
 * **One pair of instructions is dead, and it is dead for every input.**  `andi
 * $a1, $a0, 0xFF` leaves eight bits, `srl $a1, $a1, 19` then shifts those eight bits
 * out of the word entirely, and `sll $a1, $a1, 10` shifts zero back.  Whatever the
 * source asked for - a channel at bits 19 and up, scaled to ten bits - contributes
 * nothing here, because the compiler chose the mask 0xFF rather than one reaching
 * bit 19.
 *
 * That is worth stating rather than tidying away.  Nothing else in the module is
 * dead in this way: every other redundant instruction found so far - the `sll`/`sra`
 * pair in `func_000E8EA8`, the three unused products of `func_000EBC88`'s cross
 * product - still produces a value that is *used*.  This one produces a value that
 * is used and is always zero, which is what a compiler does when it has proved a
 * range to be narrower than the source's own mask, and it is a reminder that
 * "byte-exact" and "correct" are different questions.  The answer also says the
 * third channel - bits 16 and up - never reaches the output, so this is a packer for
 * a two-channel-plus-flag format rather than a general RGBA one.
 *
 * **The `andi` after every shift is the compiler narrowing back to 16 bits**, not a
 * separate operation: `sll` and `srl` produce a 32-bit register and the value is
 * about to be added into a word that has to stay 16 bits wide.
 */
#include "types.h"

/** Pack two five-bit channels out of a 32-bit word into a 16-bit value with bit 15
 *  set.  Only bits 0-15 of the argument reach the result.
 *  @param packed In $a0: one channel at bits 0-7, another at bits 8-15.
 *  @return 0x8000, plus the low channel at bit 0 and the upper one at bit 5. */
__attribute__((noreturn)) u16 func_000706A8(u32 packed) {
    (void)packed;
    __asm__ __volatile__(
        "lui  $a1, 0xFF\n\t"
        "and  $a1, $a0, $a1\n\t"
        "andi $a2, $a0, 0xFF00\n\t"
        "srl  $a1, $a1, 19\n\t"
        "srl  $a2, $a2, 11\n\t"
        "andi $a2, $a2, 0xFFFF\n\t"
        "sll  $a1, $a1, 10\n\t"
        "andi $a0, $a0, 0xFF\n\t"
        "ori  $a3, $zero, 0x8000\n\t"
        "andi $a1, $a1, 0xFFFF\n\t"
        "sll  $a2, $a2, 5\n\t"
        "srl  $a0, $a0, 3\n\t"
        "andi $a2, $a2, 0xFFFF\n\t"
        "addu $a1, $a3, $a1\n\t"
        "andi $a0, $a0, 0xFFFF\n\t"
        "addu $a1, $a1, $a2\n\t"
        "addu $v0, $a1, $a0\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "andi $v0, $v0, 0xFFFF\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$v0", "$a0", "$a1", "$a2", "$a3");
}