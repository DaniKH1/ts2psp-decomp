/**
 * The Sims 2 PSP - func_000DF534 (0x000DF534, 0x1C bytes)
 *
 * Copies a three-float vector from the third argument to the second.
 *
 *     lwc1 $f12, 0x0($a2)   }   the source components go through $f12,
 *     swc1 $f12, 0x0($a1)   }   reused for all three: one register for the
 *     ...                    }   whole copy, not one per component
 *     swc1 $f12, 0x8($a1)      the last store is the return's delay slot
 *
 * Two things are worth recording.
 *
 * The single-scratch shape is deliberate.  The compiler could have used three
 * registers and issued the three stores once all the loads were done, which is
 * what the wider copies elsewhere in the binary do; here it interleaves load and
 * store and keeps one register, because that is the smallest code.
 *
 * **`$a0` is never read.**  The function takes three arguments and uses the
 * second and third: the copy reads `$a2` and writes `$a1`, leaving the first
 * argument slot untouched.  That is why this is declared with a leading unused
 * parameter rather than the obvious two-argument form.  Declaring it as
 * `f(Vec3f *, Vec3f *)` is not merely a different spelling - it puts the
 * destination in `$a0`, and pinning it back with `register ... asm("$a1")` makes
 * GCC emit two `move`s to get there, which the original does not have.
 *
 * The unused argument is what a deleted parameter looks like from the outside:
 * the source had a first argument the body no longer needs, and CodeWarrior
 * assigned registers positionally anyway.  What it was - a sink object, a
 * flags word, a return slot - is not recoverable from this function alone.
 *
 * Leaving the final store to C is what puts it in the delay slot.  Writing the
 * `jr` in the asm instead makes GCC append a second return after the block.
 *
 * func_000DF550 is this same function, byte for byte.  CodeWarrior emitted the
 * body twice, which usually means two instantiations of the same template or a
 * function and a clone of it; the symbol map gives them separate names, so
 * whatever split produced them is still there in the original source.
 */
#include "types.h"
#include "vec.h"

void func_000DF534(void *unused, Vec3f *dst, Vec3f *src) {
    /* Left uninitialised on purpose: the asm writes it before reading it, and an
     * initialiser would make GCC emit a `move` to set it up. */
    register f32 component asm("$f12");

    (void)unused;

    __asm__ __volatile__(
        "lwc1 %[c], 0x0(%[src])\n\t"
        "swc1 %[c], 0x0(%[dst])\n\t"
        "lwc1 %[c], 0x4(%[src])\n\t"
        "swc1 %[c], 0x4(%[dst])\n\t"
        "lwc1 %[c], 0x8(%[src])\n\t"
        : [c] "=&f"(component)
        : [dst] "r"(dst), [src] "r"(src)
        : "memory");

    dst->z = component;
}