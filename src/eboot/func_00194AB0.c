/**
 * The Sims 2 PSP - func_00194AB0 (0x00194AB0, 0x8 bytes)
 *
 *     jr    $ra
 *       lw    $v0, 0x4($a0)
 *
 * A field getter returning the word at offset 4 of the object, and one
 * of nine identical +0x04 getters.
 *
 * **It is 8 bytes after func_00194AA8, the last of the eight +0x00
 * getters.**  A neighbouring pair with *different* offsets is one class's
 * getter block rather than a duplicated inlining - and this address has
 * more of them: func_00194AB8 reads +0x18 and func_00194AC0 reads +0x1C.
 *
 * So func_00194AA8, func_00194AB0, func_00194AB8 and func_00194AC0 are
 * **one class's accessors at offsets 0x00, 0x04, 0x18 and 0x1C**, with a
 * 0x14 gap where the fields between them have no accessor.  That fixes
 * four field offsets of this class and shows that 0x08 through 0x14 are
 * reached by other code.
 */
#include "types.h"

__attribute__((noreturn)) void func_00194AB0(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "lw    $v0, 0x4($a0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}