/**
 * The Sims 2 PSP - func_0012C2D0 (0x0012C2D0, 0x3C bytes)
 *
 * A constructor that also allocates: it takes the next value of a module-level
 * counter and stores it in the object as its id.
 *
 *     lui   $a3, 0x1F            0x1F0000
 *     addiu $a3, $a3, -0x2128    0x1EDED8 - a global
 *     sw    $a3, 0x14($a0)       0x14 = &that global
 *     sw    $zero, 0x0($a0)      0x0 = 0
 *     sw    $zero, 0x4($a0)      0x4 = 0
 *     sw    $a1, 0x8($a0)        0x8 = the first argument, kept
 *     sw    $zero, 0xC($a0)      0xC = 0
 *     lui   $a1, 0x1E            $a1 is now spent, rebuilt for the counter
 *     lw    $a3, -0x16C0($a1)    counter = *0x1DE940
 *     move  $v0, $a0             return the receiver
 *     addiu $a3, $a3, 0x1        counter + 1
 *     sw    $a3, -0x16C0($a1)    *0x1DE940 = counter + 1
 *     sw    $a3, 0x10($a0)       0x10 = the same value
 *     jr    $ra
 *     sb    $a2, 0x13($a0)       0x13 = the second argument, one byte
 *
 * **The counter is incremented and the new value is both the global and the
 * object's id.**  `*0x1DE940` is a monotonic counter, and this is the only place
 * in this function that touches it, so the id is unique for the life of the
 * process: the first object through here gets 1, the second 2.  Ids that come
 * from a global counter rather than from memory the caller supplied is a strong
 * hint this is an engine-owned object - something the engine will look up again by
 * id - rather than one the caller owns.
 *
 * **The second argument is stored as a single byte, and it lands inside the id
 * word.**  `sb $a2, 0x13($a0)` writes one byte at offset 0x13 while `sw $a3, 0x10`
 * writes the word 0x10 to 0x13.  So the flag is the *top byte of the id* - the two
 * stores write the same four bytes in two different ways, word then byte.
 *
 * That is a real overlap, not two adjacent fields, and it is why this file writes
 * the flag by offset rather than through a struct member: no layout puts a `u32` at
 * 0x10 and a `u8` at 0x13 side by side.  The order the original uses - word first,
 * byte second - means the byte is meant to win, so the flag is an independent field
 * that happens to occupy the id's high byte rather than a field of some sub-word
 * type.  Either way the value was a `u8` in the source: a byte store is not
 * something a wider value gets truncated into.
 *
 * The layout that falls out is: four words of state (0x0..0xC, one of which is the
 * caller's argument), the id at 0x10 with the flag inside it, and a pointer to a
 * global at 0x14.  Three of those five are zero, which is the constructor
 * establishing known state rather than trusting the caller - the same intent as
 * func_00124658 and func_001241AC.
 *
 * `0x1EDED8` and `0x1DE940` are 0x3D98 bytes apart, both in the module's writable
 * data.  The first is a descriptor stored into the object, the second a counter
 * that is *not* - so these are two unrelated globals and only the second one is
 * mutable state.
 */
#include "types.h"

/* The flag at 0x13 **overlaps the id word at 0x10** - it is that word's top byte.
 * So there is no layout that puts them side by side, and the struct stops short of
 * it: the two overlapping fields are written by offset instead.  Reading the id as
 * a `u32` and the flag as the byte inside it means the store order matters, and the
 * original does the word first and the byte second, which is consistent with the
 * flag being an independent field that happens to sit in the top byte.
 */
#define FIELD_ID    0x10
#define FIELD_FLAG  0x13

typedef struct Obj {
    u32 field_00;    /* 0x00 - zero */
    u32 field_04;    /* 0x04 - zero */
    u32 owner;       /* 0x08 - the first argument */
    u32 field_0C;    /* 0x0C - zero */
    u32 id;          /* 0x10 - the counter's new value, 0x10..0x13 */
    /* 0x13 is the flag byte, inside the word above */
    void *descriptor;/* 0x14 - &0x1EDED8 */
} Obj;

Obj *func_0012C2D0(Obj *self, u32 owner, u8 flag) {
    register Obj *node asm("$a0") = self;
    /* `$a1` holds the owner's argument value for the first five stores and is then
     * overwritten with the address of the counter, so it is in-out.  `$a3` is
     * written twice with unrelated values and never read on entry. */
    register u32 slot asm("$a1") = owner;
    register u32 value asm("$a3");
    register u8 marker asm("$a2") = flag;

    __asm__ __volatile__(
        "lui   %[v], 0x1F\n\t"
        "addiu %[v], %[v], -0x2128\n\t"
        "sw    %[v], 0x14(%[n])\n\t"
        "sw    $zero, 0x0(%[n])\n\t"
        "sw    $zero, 0x4(%[n])\n\t"
        "sw    %[s], 0x8(%[n])\n\t"
        "sw    $zero, 0xC(%[n])\n\t"
        "lui   %[s], 0x1E\n\t"
        "lw    %[v], -0x16C0(%[s])\n\t"
        "move  $v0, %[n]\n\t"
        "addiu %[v], %[v], 0x1\n\t"
        "sw    %[v], -0x16C0(%[s])\n\t"
        : [v] "=&r"(value), [s] "+r"(slot), [n] "+r"(node)
        : [m] "r"(marker)
        : "memory", "hi", "lo");

    /* Left to C: two stores then the return, so the last store fills the delay
     * slot.  Both use the block's register variables so nothing is reloaded - the
     * counter value in particular is already in `$a3` and rebuilding it would cost
     * a load. */
    node->id = value;
    ((u8 *)node)[FIELD_FLAG] = marker;
    /* Reading the return value out of `$v0` rather than returning `node` again: the
     * block already put the receiver there, and returning `node` makes GCC emit a
     * second `move $v0, $a0` that the original does not have.  An uninitialised
     * hard register is the way to say "it is already in there". */
    register Obj *ret asm("$v0");
    return ret;
}