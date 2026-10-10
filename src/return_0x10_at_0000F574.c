/* The retail leaf returns 0x10 with `ori` in the `jr ra` delay slot.
 * GCC canonicalizes a C return of 0x10 to `addiu`, so define this tiny
 * Allegrex leaf explicitly while keeping its typed C declaration here.
 */
int return_0x10_at_0000F574(void);

__asm__(
    ".section .text.return_0x10_at_0000F574,\"ax\",@progbits\n"
    ".align 2\n"
    ".globl return_0x10_at_0000F574\n"
    ".ent return_0x10_at_0000F574\n"
    "return_0x10_at_0000F574:\n"
    ".set noreorder\n"
    "jr $31\n"
    "ori $2, $0, 0x10\n"
    ".set reorder\n"
    ".end return_0x10_at_0000F574\n"
    ".size return_0x10_at_0000F574, .-return_0x10_at_0000F574\n"
);
