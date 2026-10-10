/* func_00049AA0 - the write half of the func_00049A90 accessor pair:
 *
 *   lui  a1, 0x7
 *   addiu a1, a1, 0x43A0     ; base 0x000743A0
 *   jr   ra
 *   sw   a0, 4(a1)           ; store in the delay slot
 */
struct unk_743A0 {
    unsigned f0;
    unsigned f4;
};

extern char code_000743A0;

void func_00049AA0(unsigned v)
{
    register struct unk_743A0 *base asm("$5")
        = (struct unk_743A0 *)&code_000743A0;
    base->f4 = v;
}
