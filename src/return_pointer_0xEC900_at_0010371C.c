/* Return the static address materialized by the retail code. */
void *return_pointer_0xEC900_at_0010371C(void)
{
    register volatile unsigned int high_address __asm__("$2") = 0x000F0000;
    return (void *)(high_address - 0x3700);
}
