/* Return the static address materialized by the retail code. */
void *return_pointer_0x63FF8_at_00101ADC(void)
{
    register volatile unsigned int high_address __asm__("$2") = 0x00060000;
    return (void *)(high_address + 0x3FF8);
}
