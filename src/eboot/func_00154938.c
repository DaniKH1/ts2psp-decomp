/**
 * The Sims 2 PSP - func_00154938 (0x00154938, 0xC bytes)
 *
 *     lui   $v0, %hi(str_gameObjectBehavior)
 *     jr    $ra
 *       addiu $v0, $v0, %lo(str_gameObjectBehavior)
 *
 * Returns the address of the string `"gameObjectBehavior"` - and that
 * makes it **the type-name getter of the game's object model.**
 *
 * It sits at the end of the run of shared defaults: four `jr $ra` / `nop`
 * copies (`func_00154908`, `func_00154910`, `func_00154920`,
 * `func_00154928`) and one `return 0` (`func_00154930`) immediately
 * before it.  **A virtual method that returns a class name, in the base
 * class the rest of the model inherits, is how a serialisation or
 * reflection system finds out what an object is.**
 *
 * **And the strings that follow it in `.rodata` are that system's
 * registry.**  Starting at `0x1C1D60` there are 115 consecutive
 * name-like strings and not one engine term among them:
 *
 *   complex  character  player  mplayer  fplayer
 *   oscardelfuego  anniehowell  hoothowell  virginyafeng
 *   nightbeast  circebeaker  lokibeaker  pt9  lazlocurious
 *   bedsinglemoderate  magicmirror  carmagazine  plumbobcar
 *   moneytree  cowthulustage  cow  loading  g_hud  ring  date
 *
 * Five abstract names - `complex`, `character`, `player`, and `mplayer` /
 * `fplayer`, the male and female player classes - then a run of **Sims
 * and NPCs**, then a run of **props and objects**.  **Searching the
 * block for `texture`, `mesh`, `bone`, `anim`, `sound`, `camera`,
 * `shader`, `node`, `graph`, `geom` or `vert` returns nothing**, so the
 * registry names game content types and nothing else: the rendering and
 * geometry types are named elsewhere, or not by string at all.
 *
 * **`str_gameObjectBehavior` appears twice in the binary** - here at
 * `0x1BF9FC`, and again as the registry's first entry at `0x1C1D60`.  The
 * linker did not merge them, so **the getter's result and the registry's
 * first entry are different pointers to equal text, and a pointer
 * comparison between the two would fail.**  Which is authoritative is not
 * decidable here; the duplication itself is.
 */

#include "types.h"

__attribute__((noreturn)) void func_00154938(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "lui   $v0, %%hi(str_gameObjectBehavior)\n\t"
        "jr    $ra\n\t"
        "addiu $v0, $v0, %%lo(str_gameObjectBehavior)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}