/**
 * The Sims 2 PSP - func_000B9D34 (0x000B9D34, 0x8 bytes)
 *
 *     jr    $ra
 *       ori   $v0, $zero, 0x1
 *
 * The second of the two "yes" answers, immediately after
 * func_000B9D2C in the shared part of the vtable.
 *
 * **These two returning 1 sit at slots +0x10 and +0x18 - the first two
 * methods after the constructor.**  Everything from +0x20 up to +0xC8
 * that these three classes share is either a real body or an answer of
 * 0 or nothing.  So the interface leads with the two things that are
 * true by default and answers "no" to the other fifteen questions.
 *
 * Whether the pair are related to each other - a "is X / is not X" pair
 * around one query, or two independent flags - is not decided by these
 * eight bytes.
 *
 * **It is also slot +0x01C of the second vtable family** - the eleven
 * records at a 0xD8 stride from `sym_001EAA78` to `sym_001EB560` - and
 * there **seven of the eleven put nothing else in that slot.**  Each of
 * the other four records has its own body, so the slot has five
 * distinct values in total: this one, `func_000BE138` (0xE8 bytes),
 * `func_000BE4D4` (0x6C), `func_000BE75C` (0x6C) and `func_000BFEB8`
 * (0x7C).  No two of the four are shared.
 *
 * **Three of those four call this function before branching on the
 * result**, and since this function returns 1 unconditionally, the
 * `beqz $v0` that follows is never taken.  The label it targets is
 * therefore unreachable.  Only `func_000BFEB8` does not call it, and it
 * is the one body with a real test in it (`bne $a0, $a3`) rather than a
 * call.
 *
 * So the five-way choice at +0x01C is **seven classes declining to
 * override, three classes overriding with a dead default call, and one
 * class overriding with an actual test** - and the distinction between
 * those last two groups is exactly the distinction the bytes make.
 */
#include "types.h"

__attribute__((noreturn)) void func_000B9D34(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "ori   $v0, $zero, 0x1\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}