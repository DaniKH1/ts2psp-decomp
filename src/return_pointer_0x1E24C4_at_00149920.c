/* Return the static address materialized by the retail code. */
void *return_pointer_0x1E24C4_at_00149920(void)
{
    register volatile unsigned int high_address __asm__("$2") = 0x001E0000;
    return (void *)(high_address + 0x24C4);
}
