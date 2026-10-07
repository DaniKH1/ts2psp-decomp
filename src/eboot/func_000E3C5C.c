/**
 * The Sims 2 PSP - func_000E3C5C (0x000E3C5C, 0x10 bytes)
 *
 * Stores the incoming float into the object's field at 0x54 and sets the same
 * flag byte at 0x40 to 1.
 *
 * The same "set this field and record that it is set" as func_000E3C4C, with the
 * field at 0x54 instead of 0x50.  See that function for the reading and for why
 * the constant 1 is built in asm: written in C, GCC hoists it above the float
 * store and puts it in $v0, and the original has it in $a1 in the middle of the
 * block.
 */
#include "types.h"

typedef struct FloatField {
    u8 pad[0x40];
    u8 present;    /* 0x40 */
    u8 pad2[0x13];
    f32 value;     /* 0x54 */
} FloatField;

/* self->value = value; self->present = 1; */
void func_000E3C5C(FloatField *self, f32 value) {
    register f32 v asm("$f12") = value;
    register u32 one asm("$a1");

    __asm__ __volatile__(
        "swc1 %[v], 0x54(%[obj])\n\t"
        "ori  %[one], $zero, 1\n\t"
        : [v] "+f"(v), [one] "=&r"(one)
        : [obj] "r"(self)
        : "memory");

    self->present = one;
}