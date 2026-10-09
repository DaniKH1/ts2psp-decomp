/**
 * The Sims 2 PSP - func_00154908 (0x00154908, 0x8 bytes)
 *
 *     jr    $ra
 *       nop
 *
 * A virtual method whose default does nothing and returns nothing.
 *
 * **This is the most widely shared function in the module's object
 * model.**  It appears in **52 of the 289 vtables** found in `.data`,
 * and three of its neighbours - `func_00154910`, `func_00154920` and
 * `func_00154928` - are the same two instructions and appear in 53, 47
 * and 47 tables.  **Four byte-identical void defaults, together in four
 * hundred vtable slots.**
 *
 * That they are separate functions rather than one shared function is
 * the interesting part.  A single `f(){}` in the base class would have
 * one address in every table; **four separate addresses means four
 * distinct inlined copies**, which is what happens when the compiler
 * emits each class's override of an empty method rather than pointing at
 * a shared definition.
 *
 * The cluster around them is the shared base of the whole hierarchy:
 *
 *   func_00154908  8 bytes  void         52 tables
 *   func_00154910  8 bytes  void         53 tables
 *   func_00154920  8 bytes  void         47 tables
 *   func_00154928  8 bytes  void         47 tables
 *   func_00154930  8 bytes  returns 0    47 tables
 *   func_00154938 12 bytes  returns a string  21 tables
 *
 * and **`func_00154938` is the one that names the type system**: it
 * returns the address of `"gameObjectBehavior"`.
 */
#include "types.h"

__attribute__((noreturn)) void func_00154908(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "nop\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}