/**
 * The Sims 2 PSP - func_000805D4 (0x000805D4, 0x8 bytes)
 *
 *     jr    $ra
 *       addiu $v0, $a0, 0x8
 *
 * Returns `this + 8`.
 *
 * **This is a base-pointer accessor, not a field getter**, and it is the
 * clearest of its kind in the accessor tier.  A getter reads a word at a
 * fixed offset; **this hands back a pointer into the object**, so a
 * caller can treat the sub-object at +8 as an object in its own right
 * and call accessors on it.
 *
 * The other instance of this in the module is negative - `func_0009D11C`
 * reads a signed half-word adjustment out of a thunk and *adds* it to
 * `this` before a `jalr`.  Together they are the two ends of the
 * multiple-inheritance idiom: one adjustment arrives from a vtable entry,
 * this one is hard-coded.  **Since the offset is a compile-time constant
 * here, the sub-object is at a fixed place in this class rather than at
 * a position computed per-object.**
 */
#include "types.h"

__attribute__((noreturn)) void func_000805D4(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "addiu $v0, $a0, 0x8\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}