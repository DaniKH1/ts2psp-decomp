/* The retail leaf is `jr ra; ori v0, zero, 0x10`.
 *
 * A plain `return 0x10` compiles to `addiu`, so this GNU C hard-register
 * variable makes the expression use Allegrex's always-zero `$zero` register.
 * The register declaration emits no instruction; the OR compiles to `ori`. */
int return_0x10_at_0000F574(void)
{
    register unsigned int allegrex_zero __asm__("$0");
    return allegrex_zero | 0x10u;
}
