struct unk_00029CFC {
    char pad[0x30];
    unsigned f30;
};

void func_00029CFC(struct unk_00029CFC *p, unsigned v)
{
    register unsigned tmp asm("a2") = p->f30;
    register unsigned v_reg asm("a1") = v;
    v_reg = tmp - v_reg;
    p->f30 = v_reg;
}