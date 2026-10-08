/**
 * The Sims 2 PSP - func_001A9B78 (0x001A9B78, 0x14 bytes)
 *
 * Loads a pointer from offset 0x18, stores a float from $f12 at offset
 * 0x34, sets bit 2 of the loaded pointer, stores it back.
 *
 *     lw   $a1, 0x18($a0)
 *     swc1 $f12, 0x34($a0)
 *     ori  $a1, $a1, 0x4
 *     jr   $ra
 *     sw   $a1, 0x18($a0)
 *
 * **Sets a flag bit (bit 2) on a pointer while also storing a float**.
 * The float comes from $f12 (first float argument register) and is
 * stored at offset 0x34 of the first argument.
 *
 * The pointer at 0x18 has bit 2 set as a flag - this is likely a
 * state bit in a structure that's pointed to by the first argument.
 *
 * Returns void.
 */
#include "types.h"

__attribute__((noreturn)) void func_001A9B78(void *self, float f) {
    (void)self; (void)f;
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "lw   $a1, 0x18($a0)\n\t"
        "swc1 $f12, 0x34($a0)\n\t"
        "ori  $a1, $a1, 0x4\n\t"
        "jr   $ra\n\t"
        "sw   $a1, 0x18($a0)\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$a1", "$f12");
}