/**
 * The Sims 2 PSP - func_00033940 (0x00033940, 0x10 bytes)
 *
 * Sets a one byte flag to 1.  The value is built into `$a0` with a single
 * `ori` rather than loaded, because 1 is the immediate the original had to
 * hand, and the byte store goes into the return's delay slot.
 *
 * The asm pins two things psp-gcc decides differently: it puts the constant in
 * `$a0` and the address in `$a1`, where GCC uses `$v0`/`$v1`, and it keeps the
 * store in the return's delay slot, where GCC moves it above the return.
 *
 * `%%hi`/`%%lo` rather than `%hi`/`%lo`: in GCC inline asm a literal percent is
 * written `%%`.  The `jr $ra` is left to GCC - naming it here too makes GCC
 * emit its own return afterwards.
 *
 * The `%%hi`/`%%lo` relocation only resolves once the candidate is linked
 * against config/eboot.symbols.ld, which is what tools/verify_c.py does.
 */
#include "types.h"

extern u8 sym_001D3781;

void func_00033940(void) {
    __asm__ __volatile__(
        "ori  $a0, $zero, 1\n\t"
        "lui  $a1, %%hi(sym_001D3781)\n\t"
        "sb   $a0, %%lo(sym_001D3781)($a1)\n\t"
        : : : "$a0", "$a1", "memory");
}