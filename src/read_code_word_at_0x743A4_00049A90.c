/* read_code_word_at_0x743A4_00049A90 - the read half of an accessor pair (see func_00049AA0):
 *
 *   lui  a0, 0x7
 *   addiu a0, a0, 0x43A0     ; base 0x000743A0
 *   jr   ra
 *   lw   v0, 4(a0)           ; load in the delay slot
 *
 * The +4 rides on the load, not on the address materialization: the
 * retail compiler kept a distinct base at 0x743A0 (a symbol reference
 * at link time) and added the field offset separately.  The word at
 * 0x743A4 is a `jr ra` in the image, so this patches a function's
 * return jump at runtime (self-modifying hook), which explains why the
 * base is code, not data.
 */
struct unk_743A0 {
    unsigned f0;
    unsigned f4;
};

extern char code_000743A0;

unsigned read_code_word_at_0x743A4_00049A90(void)
{
    register struct unk_743A0 *base asm("$4")
        = (struct unk_743A0 *)&code_000743A0;
    return base->f4;
}
