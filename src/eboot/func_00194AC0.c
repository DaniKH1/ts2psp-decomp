/**
 * The Sims 2 PSP - func_00194AC0 (0x00194AC0, 0x8 bytes)
 *
 *     jr    $ra
 *       lw    $v0, 0x1C($a0)
 *
 * A field getter returning the word at offset 0x1C, and the last of the
 * accessor block at func_00194AA8.
 *
 * Read the four together and the class layout starts to close:
 *
 *     0x00  getter        func_00194AA8
 *     0x04  getter        func_00194AB0
 *     0x14  setter        func_00194AFC
 *     0x18  getter        func_00194AB8
 *     0x1C  getter        func_00194AC0
 *
 * **Six of the object's words are pinned and 0x08 to 0x10 are not.**
 * Whether those three words are a pointer, a count or padding is not
 * something the getters say - but a count would explain why nothing
 * generates a getter for them.
 */
#include "types.h"

__attribute__((noreturn)) void func_00194AC0(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "lw    $v0, 0x1C($a0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}