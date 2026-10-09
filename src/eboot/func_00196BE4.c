/**
 * The Sims 2 PSP - func_00196BE4 (0x00196BE4, 0x10 bytes)
 *
 *     lui   $a0, 0x7F7F
 *     ori   $a0, $a0, 0xFFFF
 *     jr    $ra
 *       mtc1  $a0, $f0
 *
 * Returns the constant `0x7F7FFFFF` in `$f0`.
 *
 * **That is `FLT_MAX`** - 3.4028235e38, the largest finite
 * single-precision float - built with the `lui`/`ori` pair rather than a
 * literal pool entry, because there is no pool in this ABI for a value
 * returned in an FPU register.  **So this is a virtual method that
 * returns a constant float, and it is entry 0 of the three-table family
 * at `0x1EC528`, `0x1ECD88` and `0x1EDB78`.**
 *
 * Those three tables begin with the same four entries:
 *
 *   [0]  func_00196BE4   this one, FLT_MAX
 *   [1]  func_00171268   returns 0
 *   [2]  func_00171280   returns 1
 *   [3]  func_00196C04   a thunk dispatch, 44 bytes
 *
 * **A class whose first virtual is `return FLT_MAX` and whose next two
 * are `false` and `true` is an interface built out of constants** - the
 * two booleans are the "is X" and "is not X" pair, and the float is a
 * limit the caller compares against rather than a value anything reads.
 * That these are slots 1 and 2 immediately apart is the same
 * "two answers by default" pattern the `sym_001EA3E8` family opens with,
 * and the same one `func_0018F6xx` stubs fill elsewhere.
 *
 * The shared entries are the base; where the three tables differ is
 * decided by the runs after these four, and those are not function
 * pointers, so the divergence is data and not a further shared method.
 */
#include "types.h"

__attribute__((noreturn)) void func_00196BE4(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "lui   $a0, 0x7F7F\n\t"
        "ori   $a0, $a0, 0xFFFF\n\t"
        "jr    $ra\n\t"
        "mtc1  $a0, $f0\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}