/* Return the static address materialized by the retail code. */
void *return_pointer_0xE2340_at_00082180(void)
{
    register volatile unsigned int high_address __asm__("$2") = 0x000E0000;
    return (void *)(high_address + 0x2340);
}
