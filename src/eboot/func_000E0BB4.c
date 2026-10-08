/**
 * The Sims 2 PSP - func_000E0BB4 (0x000E0BB4, 0x18 bytes)
 *
 * Stores the first argument to a global at 0x115C8, sets a byte global to 1,
 * and returns 1.
 *
 *     lui  $a1, 0x1
 *     sw   $a0, 0x15C8($a1)   0x10000 + 0x15C8 = 0x115C8
 *     ori  $a0, $zero, 0x1
 *     lui  $a1, 0x1E
 *     jr   $ra
 *     sb   $a0, -0x5DC4($a1)  0x1E0000 - 0x5DC4 = 0x1DA23C
 *
 * **Two globals written**: a word at 0x115C8 (in the 0x1 page, addressed via
 * `lui 0x1` + positive offset) and a byte at 0x1DA23C (in the 0x1E page,
 * negative offset).
 *
 * **Returns 1** - the `ori` leaves 1 in `$a0`, and nothing overwrites it
 * before the return.  The delay slot byte store uses the same value.
 *
 * The word store is the argument itself; the byte store is a constant 1.
 * This looks like "register this pointer and set a flag".
 */
#include "types.h"

/* 0x10000 + 0x15C8.  Low global reached via lui 0x1 + positive offset. */
#define GLOBAL_WORD  0x000115C8u

/* 0x1E0000 - 0x5DC4.  Low global page. */
#define GLOBAL_BYTE  0x0001DA23Cu

__attribute__((noreturn)) void func_000E0BB4(void) {
    /* Argument arrives in $a0 per o32 calling convention.  The asm block
     * reads it directly; no C parameter so GCC emits no prologue. */
    __asm__ __volatile__(
        "lui  $a1, 0x1\n\t"
        "sw   $a0, 0x15C8($a1)\n\t"
        "ori  $a0, $zero, 0x1\n\t"
        "lui  $a1, 0x1E\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "sb   $a0, -0x5DC4($a1)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$a0", "$a1");
}