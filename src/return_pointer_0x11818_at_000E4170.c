/* Return the static address materialized by the retail code. */
void *return_pointer_0x11818_at_000E4170(void)
{
    register volatile unsigned int high_address __asm__("$2") = 0x00010000;
    return (void *)(high_address + 0x1818);
}
