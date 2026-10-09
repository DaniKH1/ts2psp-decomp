/**
 * The Sims 2 PSP - func_001779E4 (0x001779E4, 0xC bytes)
 *
 *     lui   $v0, %hi(str_gameObjectBehavior_1D60)
 *     jr    $ra
 *       addiu $v0, $v0, %lo(str_gameObjectBehavior_1D60)
 *
 * Returns the address of the string `"gameObjectBehavior"`.
 *
 * **It returns the *second* copy of that string.**  `func_00154938`
 * returns `str_gameObjectBehavior` at `0x1BF9FC` and this returns
 * `str_gameObjectBehavior_1D60` at `0x1C1D60` - **equal text, different
 * address, two functions.**  The linker did not merge them, so the two
 * class-name getters that answer the same question return pointers that
 * compare unequal.
 *
 * This one is entry 1 of the table at `0x1E5D60`, the class the string
 * registry's first entry belongs to.  **Entry 2 of that same table is
 * `func_0017FEBC`, which returns `"playFidget"`** - and entry 2 of
 * `sym_001E56A8` is a different function again.  So the pair at slots 1
 * and 2 is a base name and an own name: slot 1 answers "what family am
 * I" and slot 2 answers "what am I".
 *
 * **That is what makes the two slots adjacent and per-class**, and it is
 * why a class that changes nothing about its behaviour still has two
 * entries that differ from its sibling's.
 */
#include "types.h"

__attribute__((noreturn)) void func_001779E4(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "lui   $v0, %%hi(str_gameObjectBehavior_1D60)\n\t"
        "jr    $ra\n\t"
        "addiu $v0, $v0, %%lo(str_gameObjectBehavior_1D60)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}