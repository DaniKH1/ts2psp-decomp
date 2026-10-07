/**
 * The Sims 2 PSP - func_0009C394 (0x0009C394, 0x10 bytes)
 *
 * Adds the incoming float argument to the object's field at offset 0x8, in
 * place.
 *
 * The same statement as func_0009C384 on the next field.  See that function for
 * what the register arrangement means and for why the load and the add are in
 * asm while the store is left to C: GCC picks `$f0` as the destination of the
 * sum and psp-gcc will not be talked out of it by a register binding, but if the
 * store is left in C the compiler schedules it into `jr $ra`'s delay slot,
 * which is where the original has it.
 *
 * The three at 0x0009C384, 0x0009C394 and 0x0009C3A4 are the same accumulate on
 * the three float fields of one object, so this reads as a setter per field:
 * set the accumulated value of component N.
 */
#include "types.h"

typedef struct Accum {
    f32 first;    /* 0x0 */
    f32 second;   /* 0x4 */
    f32 third;    /* 0x8 */
} Accum;

/* self->third += delta */
void func_0009C394(Accum *self, f32 delta) {
    register f32 addend asm("$f12") = delta;

    __asm__ __volatile__(
        "lwc1  $f13, 0x8(%[obj])\n\t"
        "add.s %[acc], $f13, %[acc]\n\t"
        : [acc] "+f"(addend)
        : [obj] "r"(self)
        : "$f13", "memory");

    self->third = addend;
}