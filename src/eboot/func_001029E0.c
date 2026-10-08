/**
 * The Sims 2 PSP - func_001029E0 (0x001029E0, 0x8 bytes)
 *
 *     jr $ra
 *     nop
 *
 * **This function does nothing.**  Two instructions, eight bytes, no memory
 * touched, no register read: `jr $ra` with a `nop` in the delay slot returns
 * whatever the caller passed in.
 *
 * ## Six functions in this renderer that compute nothing
 *
 * This is one of four byte-identical eight-byte stubs in the PSP renderer
 * region, and it is the target of `func_0010224C`:
 *
 *     func_001029E0   jr $ra / nop          <- this one
 *     func_001029E8   jr $ra / nop
 *     func_00102228   jr $ra / nop
 *     func_001022B0   jr $ra / nop
 *     func_00102230   frame; jal func_001029E8; frame   -> calls a stub
 *     func_0010224C   frame; jal func_001029E0; frame   -> calls a stub
 *
 * **Taken together the six functions form a closed subgraph that produces no
 * value at all**, and that is a stronger statement than any one of them makes
 * on its own.  Two readings are available and nothing in the module chooses
 * between them: they are stub entries that something calls for its side effect
 * of not being null, or they are bodies that were compiled out - the renderer
 * is shared with the console build and a feature the PSP does not have would
 * leave an empty function rather than no function at all.
 *
 * **The module cannot tell them apart, and this file does not.**  What it does
 * establish is the shape: eight bytes, `jr $ra`, and a `nop` that is not dead
 * - it is the delay slot, and it has to be written.
 */
#include "types.h"

/** Returns immediately without doing anything.
 *  The caller keeps whatever it passed in `$v0`; no argument is read. */
__attribute__((noreturn)) void func_001029E0(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "jr $ra\n\t"
        "nop\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}