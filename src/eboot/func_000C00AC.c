/**
 * The Sims 2 PSP - func_000C00AC (0x000C00AC, 0x1C bytes)
 *
 *     addiu $sp, $sp, -0x20
 *     sw    $ra, 0x10($sp)
 *     jal   func_000CB950
 *       lw    $a0, 0x24($a0)
 *     lw    $ra, 0x10($sp)
 *     jr    $ra
 *     addiu $sp, $sp, 0x20
 *
 * Forwards the object at 0x24 of its argument to func_000CB950 and
 * returns whatever that call leaves in `$v0`.
 *
 * **Exactly one of the eleven vtables points here: the last,
 * `sym_001EB560`.**  The other ten leave `func_0018F668`, the void
 * default, in place - the same stub that was already promoted from the
 * *other* vtable family.  So the choice at this slot is "forward to
 * func_000CB950" against "do nothing", and it is a 10-against-1 split
 * rather than an even one.
 *
 * That makes this slot a **behaviour flag rather than an algorithm**:
 * the interface declares it and one derived class implements it.
 * `sym_001EB560` is also the only record that overrides slot +0x01C
 * with a body that contains a real test, and the only one whose +0x064
 * answers 0 rather than returning its argument - **it is the single
 * class in this family that does not leave the interface alone.**
 *
 * **The argument is reloaded in the delay slot, not set up earlier** -
 * `lw $a0, 0x24($a0)` needs the original `$a0`, which is still live at
 * the `jal`, so the load goes after it.  The function is otherwise a
 * pure forwarder: it saves `$ra` and nothing else, because `$a0` is
 * caller-saved and `$v0` is the callee's.
 */
#include "types.h"

__attribute__((noreturn)) void func_000C00AC(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x20\n\t"
        "sw    $ra, 0x10($sp)\n\t"
        "jal   func_000CB950\n\t"
        "lw    $a0, 0x24($a0)\n\t"
        "lw    $ra, 0x10($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}