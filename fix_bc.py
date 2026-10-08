import pathlib

content = r'''/**
 * The Sims 2 PSP - func_000908BC (0x000908BC, 0x0C bytes)
 *
 * Clears a word through second argument, returns 0.
 *
 *     sw   $zero, 0x0($a1)
 *     jr   $ra
 *     or   $v0, $zero, $zero
 *
 * **Clears a word through second argument, returns 0.**
 * Same pattern as func_000908D8, different address in the disassembly
 * (but same logical operation).
 */
#include "types.h"

__attribute__((noreturn)) u32 func_000908BC(void *a0, void *a1) {
    (void)a0; (void)a1;
    __asm__ __volatile__(
        "sw   $zero, 0x0($a1)\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "or   $v0, $zero, $zero\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$v0");
}
'''

pathlib.Path('src/eboot/func_000908BC.c').write_text(content, encoding='utf-8')
print('Done')