/**
 * The Sims 2 PSP - func_000D6AFC (0x000D6AFC, 0x14 bytes)
 *
 * Copies one word out of a doubly-indirected global into the caller's object.
 *
 *     lui   $a1, 0x1E
 *     lw    $a1, -0x6160($a1)     0x1F9EA0 holds a pointer
 *     lw    $a1, 0x0($a1)        ... to the word that is the value
 *     jr    $ra
 *     sw    $a1, 0x0($a0)         field at 0x0 of the argument = that word
 *
 * **Two dereferences and only one argument.**  `0x1F9EA0` is a pointer to a
 * pointer, so the value comes from somewhere the module decides at run time rather
 * than from a table this function can name.  The single argument is only a
 * destination.
 *
 * So this is a getter with a settable target: something has already worked out
 * which object is current - a selection, an active sprite, a loaded level - and
 * this function copies that reference into whatever asks for it.  `$a0` is never
 * read, only written through, which is the same "argument used only as an output"
 * shape as func_000DF534 and the `unused` parameter of func_0018DDAC.
 *
 * The store goes in the delay slot, so it is left to C with the loaded value read
 * back out of `$a1` - the alternative, letting C do the load, puts it in a different
 * register and costs a `move`.
 */
#include "types.h"

/* The global holding the pointer, built as lui 0x1E + addiu -0x6160. */
#define CURRENT   0x0001F9EA0u

typedef struct Ref {
    void *current;   /* 0x0 */
} Ref;

void func_000D6AFC(Ref *self) {
    register Ref *node asm("$a0") = self;
    register void *value asm("$a1");

    __asm__ __volatile__(
        "lui   %[v], 0x1E\n\t"
        "lw    %[v], -0x6160(%[v])\n\t"
        "lw    %[v], 0x0(%[v])\n\t"
        : [v] "=&r"(value)
        :
        : "memory", "hi", "lo");

    /* Left to C so it lands in the return's delay slot, reading the value out of
     * the register the block put it in rather than reloading through the global. */
    node->current = value;
}