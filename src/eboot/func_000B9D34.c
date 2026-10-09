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