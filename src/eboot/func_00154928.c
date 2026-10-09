/**
 * The Sims 2 PSP - func_00154928 (0x00154928, 0x8 bytes)
 *
 *     jr    $ra
 *       nop
 *
 * A void default, byte-identical to `func_00154908`, `func_00154910`
 * and `func_00154920`, and the second most widely shared of the four -
 * **it recurs at entry indices 2 and 7 across the module's tables**,
 * where `func_00154908` takes indices 1 and 6.
 *
 * **Those two indices, one apart, repeating in tables with nothing else
 * in common, is the signature of one base class declaring two consecutive
 * empty methods.**  Nothing in the bytes separates one table from the
 * next - they are laid out back to back - so the count of tables cannot
 * be measured, but the fixed positions can, and they do not vary.
 *
 * All four copies are the same two instructions at four different
 * addresses, so **the compiler emitted a copy at each override site and
 * the linker merged none of them.**
 */

#include "types.h"

__attribute__((noreturn)) void func_00154928(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "nop\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}