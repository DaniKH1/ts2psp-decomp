/* Return the static address materialized by the retail code. */
void *return_pointer_0x1D4D34_at_00093F98(void)
{
    register volatile unsigned int high_address __asm__("$2") = 0x001D0000;
    return (void *)(high_address + 0x4D34);
}
