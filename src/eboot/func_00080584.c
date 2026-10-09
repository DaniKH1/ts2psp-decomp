/**
 * The Sims 2 PSP - func_00080584 (0x00080584, 0x8 bytes)
 *
 *     jr    $ra
 *       lw    $v0, 0x18($a0)
 *
 * A field getter returning the word at offset 0x18, one of three
 * identical +0x18 getters.
 *
 * **func_0008058C is 8 bytes after this one and has an identical body.**
 * That is the duplicated-inlining signature - see func_0004E5CC - and
 * here it sits directly beside a per-class accessor block, so it is not
 * a separate class's getter that happens to match.  **One getter was
 * emitted twice because two callers needed it and the compiler could
 * not share the address.**
 *
 * Read with func_00080514, func_0008054C and func_000805D4 this class
 * exposes 0x04 (as a flag word), 0x18, 0x1C, 0x20 and a sub-object at
 * +8.
 */
#include "types.h"

__attribute__((noreturn)) void func_00080584(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "lw    $v0, 0x18($a0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}