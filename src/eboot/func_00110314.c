/**
 * The Sims 2 PSP - func_00110314 (0x00110314, 0x20 bytes)
 *
 * Appends an eight-byte record with a tag of 2 and a word payload.
 *
 *     lw   $a2, 0x8($a0)
 *     ori  $a3, $zero, 0x2
 *     sw   $a1, 0x4($a2)
 *     sw   $a3, 0x0($a2)
 *     lw   $a1, 0x8($a0)
 *     addiu $a1, $a1, 0x8
 *     jr   $ra
 *     sw   $a1, 0x8($a0)
 *
 * **`*(u32 *)(cursor + 4) = arg; *(u32 *)cursor = 2; cursor += 8;`**
 *
 * **This is `func_0010FFF4` with the tag changed from 3 to 2 and the payload changed
 * from a float to a word.**  Same eight-byte record, same layout - payload in the high
 * word, tag in the low - same cursor reload, same advance by eight.  **So the record
 * format has at least two tag values and two payload types, and the format is the
 * constant while both of those are not.**
 *
 * **The reload of `$a1` is not optional here, and in `func_0010FFF4` it is.**  The
 * payload arrives in `$a1` and the cursor is then reloaded into `$a1` itself, which
 * destroys it - but by then the payload has already been stored, so nothing is lost.
 * `func_0010FFF4` carries its payload in `$f12`, which is a different register class
 * and cannot be clobbered by a reload.  **So the same instruction sequence is correct
 * for both because the two payloads live in different register files**, and the reload
 * is doing double duty: preserving the cursor for the final store while happening to
 * reuse the argument register.
 *
 * **The payload is stored before the tag**, so the record's low word - the tag - is
 * written second.  Nothing depends on it; `func_0010FFF4` does the same, and
 * `func_0018A650` writes its leading word before its three floats.  **Every one of
 * these three functions writes the record's first word first**, which is not the order
 * a reader would guess and is worth noting as a shared habit rather than three
 * coincidences.
 */
#include "types.h"

/** Append a tag-2 record carrying `$a1` to the stream and advance its cursor by 8.
 *  @param self In $a0: the stream; its +0x08 word is the cursor, read and updated.
 *  @param arg  In $a1: the word stored in the record's second word. */
__attribute__((noreturn)) void func_00110314(void *self) {
    (void)self;
    __asm__ __volatile__(
        "lw   $a2, 0x8($a0)\n\t"
        "ori  $a3, $zero, 0x2\n\t"
        "sw   $a1, 0x4($a2)\n\t"
        "sw   $a3, 0x0($a2)\n\t"
        "lw   $a1, 0x8($a0)\n\t"
        "addiu $a1, $a1, 0x8\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "sw   $a1, 0x8($a0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$a0", "$a1", "$a2", "$a3");
}