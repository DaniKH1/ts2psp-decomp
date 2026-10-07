/**
 * The Sims 2 PSP - func_000D3D38 (0x000D3D38, 0x10 bytes)
 *
 * Copies the pointer held in the global at 0x001D9E6C into offset 0x148 of the
 * object.  The original also leaves that pointer in `$a1` on the way out, but
 * nothing reads it - every caller discards the return - so the C declares it
 * `void`.
 *
 * The address is read through `$a1` with a `lui` + `lw` pair rather than a
 * single displacement, which puts the value in a register the store can use
 * directly.  That is why the store needs no reload and why it can sit in the
 * return's delay slot, which is where the original has it.
 *
 * `$a1` is left uninitialised on purpose: the original overwrites it before
 * reading, and giving it an initialiser makes GCC emit a `move $a1, $a0` to
 * reconcile the two.
 *
 * The sibling func_000D6B10 is the same against a different global, storing at
 * offset 0 instead of 0x148.
 */
#include "types.h"

/* 0x001D9E6C: a pointer-sized global in .bss. */
extern void *global_001D9E6C;

typedef struct HasPtrAt148 {
    u8 pad[0x148];
    void *value;   /* 0x148 */
} HasPtrAt148;

void func_000D3D38(HasPtrAt148 *self) {
    register HasPtrAt148 *dst asm("$a0") = self;
    register void *src asm("$a1");

    /* Only the load is in the asm.  The store is left to C so that GCC
     * schedules it into the `jr $ra` delay slot - writing `jr` in the asm makes
     * GCC append a second return, and putting the store there as well leaves it
     * above the branch instead. */
    __asm__ __volatile__(
        "lui %[a1], 0x1E\n\t"
        "lw  %[a1], -0x6194(%[a1])\n\t"
        : [a1] "+r"(src)
        :
        : "memory");

    dst->value = src;
}