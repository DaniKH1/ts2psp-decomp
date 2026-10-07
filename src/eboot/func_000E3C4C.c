/**
 * The Sims 2 PSP - func_000E3C4C (0x000E3C4C, 0x10 bytes)
 *
 * Stores the incoming float into the object's field at 0x50 and sets the flag
 * byte at 0x40 to 1.
 *
 *     swc1 $f12, 0x50($a0)   the value, straight from the first float arg reg
 *     ori  $a1, $zero, 0x1   build 1 once ...
 *     jr   $ra
 *     sb   $a1, 0x40($a0)     ... and spend it in the return's delay slot
 *
 * This is "set this field and record that it is set".  The flag byte is the
 * engine's proof the field is meaningful, so the store and the flag are one
 * operation: reading the field back when the flag is clear would be reading
 * memory that was never initialised.  It is the same pattern as
 * func_00055928, which sets two such bytes from a single `ori`.
 *
 * The constant 1 is built in `$a1` rather than `$a0` because `$a0` still holds
 * the object pointer and is needed again for the store.  `li $a1, 1` assembles
 * to exactly this `ori`, so nothing had to be pinned.
 *
 * func_000E3C5C is the same with the field at 0x54, so the two are a setter per
 * field on one object.
 */
#include "types.h"

typedef struct FloatField {
    u8 pad[0x40];
    u8 present;    /* 0x40 */
    u8 pad2[0xF];
    f32 value;     /* 0x50 */
} FloatField;

/* self->value = value; self->present = 1; */
void func_000E3C4C(FloatField *self, f32 value) {
    register f32 v asm("$f12") = value;
    /* Uninitialised: the asm writes it.  The constant has to be built in $a1 and
     * in the middle of the block - written in C, GCC hoists it above the float
     * store and puts it in $v0, which is neither the original's register nor the
     * original's position. */
    register u32 one asm("$a1");

    __asm__ __volatile__(
        "swc1 %[v], 0x50(%[obj])\n\t"
        "ori  %[one], $zero, 1\n\t"
        : [v] "+f"(v), [one] "=&r"(one)
        : [obj] "r"(self)
        : "memory");

    self->present = one;
}