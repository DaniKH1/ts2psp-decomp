/**
 * The Sims 2 PSP - func_00154938 (0x00154938, 0xC bytes)
 *
 *     lui   $v0, %hi(str_gameObjectBehavior)
 *     jr    $ra
 *       addiu $v0, $v0, %lo(str_gameObjectBehavior)
 *
 * Returns the address of the string `"gameObjectBehavior"` - and that
 * makes it **the type-name getter of the whole object model.**
 *
 * It sits at the end of the run of shared defaults: four `jr $ra` /
 * `nop` voids and one `return 0` immediately before it, spanning 47 to
 * 53 vtables each out of the 289 in `.data`.  **A virtual method that
 * returns a class name, sitting in the base class that fifty classes
 * inherit unchanged, is how a serialisation or reflection system finds
 * out what an object is.**
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
 * Five abstract names, then a run of **Sims and NPCs**, then a run of
 * **props and objects**.  `mplayer` and `fplayer` are the male and
 * female player classes; `complex` and `character` are the bases above
 * them.  **Searching the block for `texture`, `mesh`, `bone`, `anim`,
 * `sound`, `camera`, `shader`, `node`, `graph` or `geom` returns
 * nothing** - so this registry names game content types and nothing
 * else, and the rendering types are named elsewhere or not at all.
 *
 * **`str_gameObjectBehavior` appears twice in the binary** - here at
 * `0x1BF9FC`, and again as the first entry of the registry at
 * `0x1C1D60`.  Two copies of one string in `.rodata` means the linker
 * did not merge them, so the getter's return value and the registry's
 * first entry are **different pointers to equal text** - and a
 * comparison between them would fail.
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