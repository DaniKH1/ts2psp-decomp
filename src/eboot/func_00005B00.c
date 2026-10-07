/**
 * The Sims 2 PSP - func_00005B00 (0x00005B00, 0x08 bytes)
 *
 *     jr    $ra
 *     or    $v0, $zero, $zero
 *
 * **This is `return 0`, and 110 functions in the module are nothing else.**
 * `tools/duplicate_bodies.py` is the census.  It is the second-largest duplicate body
 * in the module, after the 162 empty ones, and it is worth understanding what a
 * hundred-and-ten copies of `return 0` means.
 *
 * **It is not padding and it is not a stub.**  A stub would be `jr $ra` with a `nop` -
 * that is the 162-function group, the callees the linker inserted.  This one moves a
 * value into the return register, so the original compiler generated it from a source
 * function that had a return type and returned a literal zero.  110 real functions in a
 * 7,500-function module have exactly that shape.
 *
 * **The likely reading is a class hierarchy of default answers.**  A virtual method
 * whose base implementation is "no" - `IsVisible()` returning false, `CanEdit()`
 * returning false, `IsDirty()` returning false - compiles to this, and then every
 * subclass that does not override it keeps its own copy in the vtable.  The alternative
 * is 110 hand-written predicates that happen to be constant, which is possible but would
 * be strange: nobody writes `return false;` by hand 110 times, but a compiler emits it
 * that often whenever a base class says no.
 *
 * **What the callers do with the zero is measurable, and the answer is split.**  43
 * in-module branches to this group: 17 use all four bytes, 9 test the register against
 * zero with `beqz`, 12 ignore it, 3 copy it.  The zero tests are safe here - unlike
 * `func_00151240`, this function writes the whole word, so there is nothing above the
 * low byte to be wrong.  `tools/return_width.py` distinguishes the two cases rather
 * than issuing one blanket warning, because a `beqz` on a whole-word zero is not the
 * same thing as a `beqz` on a value with three garbage bytes above it.
 *
 * **`or $v0, $zero, $zero` rather than `move $v0, $zero`** is the same instruction with
 * the opcode's preferred mnemonic.  psp-gcc emits the `move` spelling, so this needs asm.
 */
#include "types.h"

__attribute__((noreturn)) s32 func_00005B00(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "or    $v0, $zero, $zero\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$v0");
}