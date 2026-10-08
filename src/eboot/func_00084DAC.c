/**
 * The Sims 2 PSP - func_00084DAC (0x00084DAC, 0x14 bytes)
 *
 * Loads a pointer from offset 0x18 of the argument, adds 0x1C to the
 * argument pointer, loads a pointer from the first loaded pointer, stores
 * it at the new argument address, and returns the loaded pointer.
 *
 *     lw   $a1, 0x18($a0)
 *     addiu $a0, $a0, 0x1C
 *     lw   $a1, 0x0($a1)
 *     jr   $ra
 *     sw   $a1, 0x0($a0)
 *
 * **Chain of pointers**: `tmp = a0->ptr_at_0x18; a0 += 0x1C; tmp = tmp->ptr_at_0; a0->ptr_at_0 = tmp`. Returns the final pointer.
 *
 * The delay slot does the store. Returns the loaded pointer.
 */
#include "types.h"

__attribute__((noreturn)) u32 func_00084DAC(void *self) {
    register void *p asm("$a0") = self;
    register u32 tmp asm("$a1");
    __asm__ __volatile__(
        "lw   %[t], 0x18(%[p])\n\t"
        "addiu %[p], %[p], 0x1C\n\t"
        "lw   %[t], 0x0(%[t])\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "sw   %[t], 0x0(%[p])\n\t"
        ".set reorder\n\t"
        : [p] "+r"(p), [t] "=r"(tmp)
        :
        : "memory");
}