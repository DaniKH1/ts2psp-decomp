/**
 * The Sims 2 PSP - func_000BA16C (0x000BA16C, 0x100 bytes)
 *
 * The shared virtual method at slot +0x40 of `sym_001EA3E8` - the last
 * of the four real bodies in the table.  It looks a symbol up and, if
 * the entry carries a name, **copies 0x28 bytes of it into the caller's
 * output buffer**; otherwise it writes a single zero byte.
 *
 * **The 0x28 is the copy size and it appears twice**, as
 * `ori $a2, $zero, 0x28` in the delay slot of two separate
 * `func_0012CA5C` calls.  0x28 = 40 bytes, which is a fixed-size name
 * field, not a string length - a `strncpy`-shaped copy into a struct
 * whose first member is a `char[40]`.
 *
 * **`sb $zero, 0x0($s2)` on both failure paths is the "empty" answer.**
 * Only one byte is cleared, not 0x28: the buffer is assumed to already
 * hold whatever it held, and clearing just the length/first byte marks
 * it empty.  Two separate sites do this, so there are two distinct ways
 * to come back empty-handed:
 *
 *   - the slot's own pointer is null, and
 *   - `func_00105C60` returned something whose +4 is null.
 *
 * **The sentinel-1 test appears a second time inside this function**,
 * and the constant lives in `$s3`, loaded at the very top and preserved
 * across `func_000BA2D8` - the same idiom as `func_000B9F88`, and the
 * third instance of the "not interned yet" test in the table's real
 * methods.
 *
 * `sll $a0, $a0, 2` before the second sentinel test is the index-to-
 * byte scaling, and `bltz $a0` is the range check on the difference
 * `name_ptr - table_base`.
 */
#include "types.h"

__attribute__((noreturn)) void func_000BA16C(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x30\n\t"
        "sw    $s0, 0x10($sp)\n\t"
        "sw    $s1, 0x14($sp)\n\t"
        "sw    $s2, 0x18($sp)\n\t"
        "sw    $s3, 0x1C($sp)\n\t"
        "ori   $s3, $zero, 0x1\n\t"
        "or    $s0, $a0, $zero\n\t"
        "or    $s1, $a1, $zero\n\t"
        "or    $s2, $a2, $zero\n\t"
        "sw    $ra, 0x20($sp)\n\t"
        "jal   func_000BA2D8\n\t"
        "or    $a0, $s0, $zero\n\t"
        "lw    $a1, 0x14($s0)\n\t"
        "or    $a0, $v0, $zero\n\t"
        "addiu $a1, $a1, 0xC\n\t"
        "lw    $a3, 0x0($a1)\n\t"
        "ori   $a2, $zero, 0x0\n\t"
        "bnel  $a3, $s3, .Leboot_000BA1B8\n\t"
        "addu  $a2, $a1, $a3\n\t"
        ".Leboot_000BA1B8:\n\t"
        "or    $a1, $a2, $zero\n\t"
        "beqz  $a1, .Leboot_000BA220\n\t"
        "nop\n\t"
        "lhu   $a3, 0x8($a1)\n\t"
        "addu  $a3, $s1, $a3\n\t"
        "subu  $a0, $a3, $a0\n\t"
        "bltz  $a0, .Leboot_000BA220\n\t"
        "nop\n\t"
        "lw    $a3, 0x0($a1)\n\t"
        "sll   $a0, $a0, 2\n\t"
        "ori   $a1, $zero, 0x0\n\t"
        "bnel  $a3, $s3, .Leboot_000BA1EC\n\t"
        "addu  $a1, $a2, $a3\n\t"
        ".Leboot_000BA1EC:\n\t"
        "addu  $a0, $a0, $a1\n\t"
        "lw    $a1, 0x0($a0)\n\t"
        "addu  $a0, $a0, $a1\n\t"
        "beqz  $a0, .Leboot_000BA214\n\t"
        "or    $a1, $a0, $zero\n\t"
        "or    $a0, $s2, $zero\n\t"
        "jal   func_0012CA5C\n\t"
        "ori   $a2, $zero, 0x28\n\t"
        "b     .Leboot_000BA218\n\t"
        "nop\n\t"
        ".Leboot_000BA214:\n\t"
        "sb    $zero, 0x0($s2)\n\t"
        ".Leboot_000BA218:\n\t"
        "b     .Leboot_000BA250\n\t"
        "nop\n\t"
        ".Leboot_000BA220:\n\t"
        "lw    $a0, 0x10($s0)\n\t"
        "jal   func_00105C60\n\t"
        "or    $a1, $s1, $zero\n\t"
        "lw    $s0, 0x4($v0)\n\t"
        "beqz  $s0, .Leboot_000BA24C\n\t"
        "or    $a0, $s2, $zero\n\t"
        "or    $a1, $s0, $zero\n\t"
        "jal   func_0012CA5C\n\t"
        "ori   $a2, $zero, 0x28\n\t"
        "b     .Leboot_000BA250\n\t"
        "nop\n\t"
        ".Leboot_000BA24C:\n\t"
        "sb    $zero, 0x0($s2)\n\t"
        ".Leboot_000BA250:\n\t"
        "lw    $s0, 0x10($sp)\n\t"
        "lw    $s1, 0x14($sp)\n\t"
        "lw    $s2, 0x18($sp)\n\t"
        "lw    $s3, 0x1C($sp)\n\t"
        "lw    $ra, 0x20($sp)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x30\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}