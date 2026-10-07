/**
 * The Sims 2 PSP - func_000D6B10 (0x000D6B10, 0x10 bytes)
 *
 * Copies the pointer held in the global at 0x001D9EA0 into offset 0 of the
 * object - the member at the very front, so the store needs no displacement.
 *
 * The same shape as func_000D3D38: the load is in the asm and the store is
 * left to C so that GCC schedules it into the return's delay slot, which is
 * where the original has it.  See that function for the full explanation,
 * including why `$a1` is uninitialised and why the C declares no return value.
 */
#include "types.h"

/* 0x001D9EA0: a pointer-sized global in .bss. */
extern void *global_001D9EA0;

typedef struct StartsWithPtr {
    void *value;   /* 0x00 */
} StartsWithPtr;

void func_000D6B10(StartsWithPtr *self) {
    register StartsWithPtr *dst asm("$a0") = self;
    register void *src asm("$a1");

    /* Only the load is in the asm; the store is C, so GCC puts it in the
     * delay slot of the return it emits. */
    __asm__ __volatile__(
        "lui %[a1], 0x1E\n\t"
        "lw  %[a1], -0x6160(%[a1])\n\t"
        : [a1] "+r"(src)
        :
        : "memory");

    dst->value = src;
}