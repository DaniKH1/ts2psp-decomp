/**
 * The Sims 2 PSP - func_0007E024 (0x0007E024, 0x30 bytes)
 *
 * Copies a two-float vector out of a fixed global into four consecutive fields of
 * the object, duplicated.
 *
 *     lui   $a1, 0x1D            0x1D0000
 *     addiu $a1, $a1, 0x489C    0x1D489C - a global, not an argument
 *     lwc1  $f12, 0x0($a1)
 *     swc1  $f12, 0x0($a0)
 *     lwc1  $f12, 0x4($a1)
 *     move  $v0, $a0             return the receiver, in the middle of the copy
 *     swc1  $f12, 0x4($a0)
 *     lwc1  $f12, 0x0($a1)       the same load again
 *     swc1  $f12, 0x8($a0)
 *     lwc1  $f12, 0x4($a1)       and again
 *     jr    $ra
 *     swc1  $f12, 0xC($a0)
 *
 * **Both source floats are loaded twice.**  There are four loads where two would
 * do, and the same two addresses are read again at offsets 6 and 8 of the
 * instruction stream.  That is not what a compiler does when it needs the value
 * twice - it would keep it in a second float register.  So the duplication is in
 * the source: it reads the source pair into the destination pair, and then reads it
 * again into the next destination pair.  Written as two copies of the same struct
 * member it re-loads; written as one copy with a wider destination it would not.
 * What the source probably looked like is two assignments from the same global,
 * which is why the duplication survives into the machine code.
 *
 * The destination gets `{ x, y, x, y }` - four consecutive floats.  Two plausible
 * shapes fit and this function cannot separate them: an array of two 2-float
 * entries, or a 4-float destination fed from a 2-float source.  The second would be
 * a widening write and would normally copy through one register, which is exactly
 * what does *not* happen, so the first is the better reading: the global at
 * 0x1D489C holds one 2-float value and this function fills two slots from it.
 *
 * The source is a **fixed global** and not the argument, so `$a1` is built from
 * scratch and the function is a setter with one parameter rather than a copy with
 * two.  Same shape as func_000FE624 and func_000FFBE8.
 *
 * `move $v0, $a0` sits in the middle, not in the delay slot.  Returning the receiver
 * from a constructor is the chaining convention the other `func_0002D630`-shaped
 * functions use, and here it is just early - the return value does not depend on
 * anything the four stores do.  So the whole body cannot be left to C: the copy
 * has to be pinned at instruction 6, between two stores, which is what putting it
 * in the asm does.  The last store is left to C so the delay slot gets filled by
 * it, which is where CodeWarrior put it.
 *
 * Two further things had to be pinned and neither is obvious.  The final load
 * cannot be left to C at all: the block above writes `$f12` four times, but the
 * compiler cannot see that, so a C load picks `$f0` and costs an extra register.
 * The load stays in the block and the store reads an uninitialised hard `$f12`
 * instead - which is the only way to say "use the value that is already there".
 */
#include "types.h"

typedef struct Vec2f {
    float x;   /* 0x0 */
    float y;   /* 0x4 */
} Vec2f;

typedef struct Slot {
    Vec2f slot[2];
} Slot;

void *func_0007E024(void *self) {
    register void *node asm("$a0") = self;
    /* `$a1` is built by the block, so it is an output; it has to be "+&r" rather
     * than "+r" because the block also has to be told it is read back, and
     * `$v0` is an earlyclobber output for the return value the block writes. */
    register u32 source asm("$a1");
    register void *result asm("$v0");

    __asm__ __volatile__(
        "lui   %[s], 0x1D\n\t"
        "addiu %[s], %[s], 0x489C\n\t"
        "lwc1  $f12, 0x0(%[s])\n\t"
        "swc1  $f12, 0x0(%[n])\n\t"
        "lwc1  $f12, 0x4(%[s])\n\t"
        "move  $v0, %[n]\n\t"
        "swc1  $f12, 0x4(%[n])\n\t"
        "lwc1  $f12, 0x0(%[s])\n\t"
        "swc1  $f12, 0x8(%[n])\n\t"
        "lwc1  $f12, 0x4(%[s])\n\t"
        : [s] "+&r"(source), [n] "+r"(node), [r] "=&r"(result)
        :
        : "memory", "hi", "lo");

    /* The store and the return are left to C, so the delay slot gets the final
     * store.  The block must not end on the delay-slot instruction itself: the
     * symbol would come out short and the next function would shift. */
    register Slot *slots asm("$a0") = node;
    /* An uninitialised hard register, reading the `$f12` the block just loaded
     * into.  Letting C do the load instead puts it in `$f0` and costs an extra
     * register, because the block's use of `$f12` is invisible to the compiler. */
    register float last asm("$f12");
    slots->slot[1].y = last;
    return result;
}