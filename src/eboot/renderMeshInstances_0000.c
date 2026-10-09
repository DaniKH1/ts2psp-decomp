/**
 * The Sims 2 PSP - renderMeshInstances_0000 (0x001BDA00, 0x54 bytes)
 *
 *     addiu      $sp, $sp, -0x20
 *     or         $a2, $a1, $zero
 *     lui        $a1, %hi(sym_000E24A8)
 *     sw         $ra, 0x10($sp)
 *     bnez       $a2, .Leboot_001BDA34
 *       addiu    $a1, $a1, %lo(sym_000E24A8)
 *     or         $a2, $a0, $zero
 *     or         $a0, $a1, $zero
 *     or         $a1, $a2, $zero
 *     jal        renderMeshInstances_0B1C
 *       or        $a2, $zero, $zero
 *     b          .Leboot_001BDA48
 *       nop
 *   .Leboot_001BDA34:
 *     or         $a2, $a0, $zero
 *     or         $a0, $a1, $zero
 *     or         $a1, $a2, $zero
 *     jal        renderMeshInstances_0C1C
 *       or        $a2, $zero, $zero
 *   .Leboot_001BDA48:
 *     lw         $ra, 0x10($sp)
 *     jr         $ra
 *       addiu    $sp, $sp, 0x20
 *
 * A two-way dispatch on the **second argument**: if it is null it calls
 * `renderMeshInstances_0B1C`, otherwise `renderMeshInstances_0C1C`.  Both
 * calls receive the **identical argument triple**:
 *
 *     a0 = sym_000E24A8   (a global, loaded through the usual lui/addiu pair)
 *     a1 = arg1 (the original $a0)
 *     a2 = 0
 *
 * The argument shuffling is pure register renaming - the object pointer is
 * parked in `$a2` before it is moved into `$a1`, then the zero third
 * argument is materialised in the call's delay slot, which is how the
 * compiler keeps the last `or` out of a separate slot.  Because the
 * `lui`/`addiu` pair is loaded before the branch, `$a0` is already the
 * global on both paths; only the callee differs.  This is a **static /
 * dynamic split of the same routine**, the shape CodeWarrior emits when
 * the two behaviours are large enough not to be worth a branch inside one
 * body - the call targets here are separate functions, so the shared part
 * is only the argument list.
 *
 * Nothing here shows what `sym_000E24A8` holds; it is only passed as the
 * first argument to both callees, so it is some global state object or
 * context rather than a computed value.  Whether the null test is on a
 * count, a pointer or a flag is not determinable from the bytes.
 */
#include "types.h"

__attribute__((noreturn)) void renderMeshInstances_0000(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu      $sp, $sp, -0x20\n\t"
        "or         $a2, $a1, $zero\n\t"
        "lui        $a1, %%hi(sym_000E24A8)\n\t"
        "sw         $ra, 0x10($sp)\n\t"
        "bnez       $a2, .Leboot_001BDA34\n\t"
        "  addiu    $a1, $a1, %%lo(sym_000E24A8)\n\t"
        "or         $a2, $a0, $zero\n\t"
        "or         $a0, $a1, $zero\n\t"
        "or         $a1, $a2, $zero\n\t"
        "jal        renderMeshInstances_0B1C\n\t"
        "  or        $a2, $zero, $zero\n\t"
        "b          .Leboot_001BDA48\n\t"
        "  nop\n\t"
        ".Leboot_001BDA34:\n\t"
        "or         $a2, $a0, $zero\n\t"
        "or         $a0, $a1, $zero\n\t"
        "or         $a1, $a2, $zero\n\t"
        "jal        renderMeshInstances_0C1C\n\t"
        "  or        $a2, $zero, $zero\n\t"
        ".Leboot_001BDA48:\n\t"
        "lw         $ra, 0x10($sp)\n\t"
        "jr         $ra\n\t"
        "  addiu    $sp, $sp, 0x20\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}