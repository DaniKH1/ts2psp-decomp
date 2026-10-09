/**
 * The Sims 2 PSP - func_0017FEBC (0x0017FEBC, 0xC bytes)
 *
 *     lui   $v0, %hi(str_playFidget)
 *     jr    $ra
 *       addiu $v0, $v0, %lo(str_playFidget)
 *
 * Returns the address of the string `"playFidget"` - **and that makes it
 * this class's own name, where slot 1 of the same table returns its
 * family's.**
 *
 * Entry 1 of the table at `0x1E5D60` is `func_001779E4`, which returns
 * `"gameObjectBehavior"`; entry 2 is this one, returning `"playFidget"`.
 * **Two adjacent name slots, one inherited and one overridden**, and the
 * same pair of positions appears in every table that has them - so this
 * is the per-class name getter rather than part of the shared base.
 *
 * `playFidget` is one of the 115 names in the registry at `0x1C1D60`
 * alongside `complex`, `character`, `player`, `mplayer`, `fplayer` and a
 * hundred Sims, NPCs and props.  **Nothing in that registry is an engine
 * term**, so `playFidget` is a game-content type name and this method is
 * how the object knows which one it is.
 *
 * **The three-register shape is forced by returning a pointer in `$v0`:
 * there is no literal pool for this ABI**, so the address is built with
 * `lui`/`addiu` in the `jr` delay slot.  Twelve bytes, the same as
 * `func_00154938` and `func_001779E4`, which return the other two
 * addresses involved.
 */
#include "types.h"

__attribute__((noreturn)) void func_0017FEBC(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "lui   $v0, %%hi(str_playFidget)\n\t"
        "jr    $ra\n\t"
        "addiu $v0, $v0, %%lo(str_playFidget)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}