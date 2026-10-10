/* Return the static address materialized by the retail code. */
void *return_pointer_0x1DAC88_at_0010031C(void)
{
    register volatile unsigned int high_address __asm__("$2") = 0x001E0000;
    return (void *)(high_address - 0x5378);
}
