/**
 * The Sims 2 PSP - func_00097200 (0x00097200, 0x10 bytes)
 *
 * Publishes one value in two places: a global and a field of the object.
 *
 *     lui   $a2, 0x1              0x10000
 *     sw    $a1, -0x361C($a2)     0xC9E4 = the second argument
 *     jr    $ra
 *     sw    $a1, 0x80($a0)        and field 0x80 of the first, in the delay slot
 *
 * **The same value goes to a global and to an instance field**, which is the shape
 * of a "current selection" or "active object" handle: the object that knows its own
 * state also has to be reachable without being passed around.  0x80 is far enough
 * into the object that it is not a vtable entry - whatever field it is, it is state
 * rather than behaviour.
 *
 * Both stores use `$a1`, so the value is computed once by the caller and never
 * touched here.  That is why the function is four instructions: it is a setter with
 * one assignment written twice.
 *
 * Writing it in C as two assignments gives the same thing only if the compiler
 * keeps the second store in the delay slot, which is what leaving the last one to C
 * is for.  It also has to be the *second* store left to C: the global one first,
 * because that is the order the original has, and GCC will not reorder a volatile
 * block against a store it can see.
 *
 * 0xC9E4 is in the page below the module's data - the reference project's
 * `mhp2g` puts its globals at 0x0C0000 and up - so this is a different kind of
 * global from the 0x1Ex ones the other setters in this set write.  Nothing else
 * found so far reaches that page, which makes it the sole evidence so far that the
 * module has more than one globals region.
 */
#include "types.h"

/* The global, addressed off the page register the block builds:
 * 0x10000 - 0x361C = 0xC9E4. */
#define PUBLISHED   0xC9E4

typedef struct Object {
    u8   pad_000[0x80];
    u32  current;   /* 0x80 - the same value as the global */
} Object;

void func_00097200(Object *self, u32 value) {
    register Object *node asm("$a0") = self;
    register u32 val asm("$a1") = value;
    register u32 page asm("$a2");

    /* Only the page is built here; the store to the global has to come after, so
     * it is the one left to C along with the field store that follows it. */
    __asm__ __volatile__(
        "lui   %[p], 0x1\n\t"
        : [p] "=&r"(page)
        :
        : "memory", "hi", "lo");

    /* The offset is negative and 16-bit, so `page - 0x361C` folds into the `sw`
     * rather than costing an `addiu` to form the address first. */
    *(u32 *)(page - 0x361C) = val;
    node->current = val;
}