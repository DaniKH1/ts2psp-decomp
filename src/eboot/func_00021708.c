/**
 * The Sims 2 PSP - func_00021708 (0x00021708, 0x14 bytes)
 *
 * Loads a pointer from offset 0x74 of the first argument, loads a pointer
 * from offset 0 of the second argument, and stores the first through the
 * second. Returns 1.
 *
 *     lw   $a0, 0x74($a0)
 *     lw   $a1, 0x0($a1)
 *     ori  $v0, $zero, 0x1
 *     jr   $ra
 *     sw   $a0, 0x0($a1)
 *
 * **Chain of pointers**: `a0 = a0->ptr_at_0x74; a1 = a1->ptr_at_0; *a1 = a0`.
 * Returns 1 (success/status).
 *
 * The delay slot does the store. Returns 1 explicitly.
 */
#include "types.h"

__attribute__((noreturn)) u32 func_00021708(void *a0, void *a1) {
    register void *p0 asm("$a0") = a0;
    register void *p1 asm("$a1") = a1;
    __asm__ __volatile__(
        "lw   %[p0], 0x74(%[p0])\n\t"
        "lw   %[p1], 0x0(%[p1])\n\t"
        "ori  $v0, $zero, 0x1\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "sw   %[p0], 0x0(%[p1])\n\t"
        ".set reorder\n\t"
        : [p0] "+r"(p0), [p1] "+r"(p1)
        :
        : "memory", "$v0");
}