/**
 * The Sims 2 PSP - func_000BF46C (0x000BF46C, 0x30 bytes)
 *
 * Returns a pointer to the first non-NUL byte of the length-prefixed string at
 * field 0x14, walking the end field at 0x20.
 *
 * The same string-length scan as func_000BF43C with the second field at a
 * different offset; see it for the pins, in particular the `sltiu`/`andi` pair
 * that cancels the sign extension from `lb` and the `beql` whose displacement
 * is written out so it lands on the `jr $ra`.
 */
#include "types.h"

/* Length-prefixed string, as the engine stores them. */
typedef struct PString {
    u32 length;   /* 0x00 */
    char data[];  /* 0x04 */
} PString;

typedef struct HoldsString {
    u8 pad[0x14];
    PString *text;   /* 0x14 */
} HoldsString;

char *func_000BF46C(HoldsString *self) {
    register HoldsString *obj asm("$a0") = self;
    register u32 offset asm("$a1");
    register char *found asm("$v0");

    __asm__ __volatile__(
        "lw     %[a0], 0x14(%[a0])\n\t"
        "ori    %[v0], $zero, 0\n\t"
        "lw     %[a1], 0x20(%[a0])\n\t"
        "addiu  %[a0], %[a0], 0x20\n\t"
        "addu   %[a0], %[a0], %[a1]\n\t"
        "lb     %[a1], 0x0(%[a0])\n\t"
        "sltiu  %[a1], %[a1], 1\n\t"
        "andi   %[a1], %[a1], 0xFF\n\t"
        ".set noreorder\n\t"
        "beql   %[a1], $zero, . + 8\n\t"
        "or     %[v0], %[a0], $zero\n\t"
        ".set reorder\n\t"
        : [a0] "+r"(obj), [a1] "+r"(offset), [v0] "+r"(found)
        :
        : "memory");
    return found;
}