/**
 * The Sims 2 PSP - func_0001FCA8 (0x0001FCA8, 0x0C bytes)
 *
 * Stores 0.0f at two consecutive offsets (0x18C and 0x200), returns
 * the argument pointer.
 *
 *     swc1 $f12, 0x18C($a0)
 *     jr   $ra
 *     swc1 $f12, 0x200($a0)
 *
 * **Two float stores, 0x74 bytes apart.**  The delay slot holds the
 * second store.  $f12 must already hold the value (likely 0.0f set
 * by caller).
 *
 * Returns the original pointer.
 */
#include "types.h"

typedef struct Target {
    u8 pad[0x18C];
    float val0;   /* 0x18C - set from $f12 */
    u8 pad2[0x70];
    float val1;   /* 0x200 - set from $f12 */
} Target;

__attribute__((noreturn)) Target *func_0001FCA8(Target *self) {
    register Target *t asm("$a0") = self;
    __asm__ __volatile__(
        "swc1 $f12, 0x18C(%[t])\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "swc1 $f12, 0x200(%[t])\n\t"
        ".set reorder\n\t"
        : : [t] "r"(self)
        : "memory", "$f12");
}