/**
 * The Sims 2 PSP - renderMeshInstances_1060 (0x001BEA60, 0xDC bytes)
 *
 * A matrix product on the vector coprocessor, then a 4x4 turned into a 4x3 on the
 * way into the command buffer.
 *
 *     addiu $sp, $sp, -0x40
 *     lv.q  R100, 0x0($a2)
 *     lv.q  R101, 0x10($a2)
 *     lv.q  R102, 0x20($a2)
 *     lv.q  R103, 0x30($a2)
 *     lv.q  R000, 0x0($a1)
 *     lv.q  R001, 0x10($a1)
 *     lv.q  R002, 0x20($a1)
 *     lv.q  R003, 0x30($a1)
 *     viim.s S400, 32
 *     vtfm4.q R200, M100, R000
 *     vtfm4.q R201, M100, R001
 *     vtfm4.q R202, M100, R002
 *     vtfm4.q R203, M100, R003
 *     vscl.t R200, R200, S400
 *     vscl.t R201, R201, S400
 *     vscl.t R202, R202, S400
 *     sv.q  R200, 0x0($sp)
 *     sv.q  R201, 0x10($sp)
 *     sv.q  R202, 0x20($sp)
 *     sv.q  R203, 0x30($sp)
 *     lui  $a0, 0x1E
 *     lw   $a1, -0x48F0($a0)
 *     cache 0x18, 0x3C($a1)
 *     lui  $a2, 0x2B00
 *     lui  $a3, 0x2B00
 *     lui  $t0, 0x2B00
 *     lwr  $a2, 0x1($sp)
 *     lwr  $a3, 0x5($sp)
 *     lwr  $t0, 0x9($sp)
 *     sw   $a2, 0x0($a1)
 *     sw   $a3, 0x4($a1)
 *     sw   $t0, 0x8($a1)
 *     ... the same three loads and three stores for each of the four rows ...
 *     addiu $a1, $a1, 0x30
 *     sw   $a1, -0x48F0($a0)
 *     jr   $ra
 *     addiu $sp, $sp, 0x40
 *
 * **A 4x4 by 4x4 product, scaled, then written to the GPU command buffer as four
 * rows of three floats.**
 *
 * **The machine code is the source**, as in `syncSkeleton_27D0.c` and
 * `syncSkeleton_2808.c`, and for the same reason: psp-gcc has no `float4` and no
 * operator that lowers to `lv.q`, `vtfm4.q` or `vscl.t`, so there is no C spelling
 * at all.  That is a different reason from the lerps' - there the compiler had a
 * spelling and declined it, here there is none - and the distinction is kept.
 *
 * Three things in the twenty-eight lines after the store are worth reading:
 *
 * **The three `lui 0x2B00`s are dead.**  Each is overwritten by the `lwr` two
 * instructions later, before anything reads it, and the `sw` stores the loaded word
 * rather than 0x2B000000.  This is the same shape as the dead `lui 0x0` in
 * `func_0009674C`: a constant that went through the `%hi`/`%lo` path in the source
 * and was then unconditionally overwritten, so the high half survives with nothing
 * to do.  Three of them in a row is the most concentrated instance in the module.
 *
 * **The `lwr` offsets are 1, 5 and 9**, which is how a 4x4 is turned into a 4x3:
 * they read bytes 1 to 4, 5 to 8 and 9 to 12 of each sixteen-byte row, so components
 * 1, 2 and 3 are copied out and component 0 - the homogeneous w - is dropped.  The
 * `lwr` is the *unaligned* load, which is the only way to reach those bytes on this
 * ISA, and it is why a four-wide vector becomes three words rather than three
 * floats.
 *
 * **`cache 0x18, 0x3C($a1)` before the stores** is a cache hint on the command
 * buffer: sub-opcode 0x18 in the CACHE instruction's five-bit field, on the word
 * four words past the cursor.  `renderMeshInstances_1034`, twenty-eight bytes
 * earlier, does the same thing with the same offset, which makes it a fixed part of
 * writing a command rather than anything about this transform.
 *
 * **The cursor advances by 0x30** - twelve words, the four rows of three - and goes
 * back to the same global, 0x0BF1C1C's neighbour at 0x01FB710.  The twelve words
 * written are a 4x3 block of floats, which is the format the command buffer wants.
 *
 * **`.set noreorder` around the last two words only.**  Under `.set reorder` the
 * assembler would hoist the `addiu $a1, $a1, 0x30` or the `sw` into `jr $ra`'s delay
 * slot; `noreorder` also stops it *inserting* a `nop`, and neither is wanted here
 * because the slot already has the frame teardown in it.
 */
#include "types.h"

/** Write a 4x3 block of transformed vectors to the command buffer.
 *  @param a1 In $a1: a 4x4 matrix, sixteen bytes per row.
 *  @param a2 In $a2: another 4x4 matrix.
 *  Advances the command-buffer cursor by 0x30 and leaves the vector coprocessor
 *  clobbered. */
__attribute__((noreturn)) void renderMeshInstances_1060(void *a1, void *a2) {
    (void)a1;
    (void)a2;
    __asm__ __volatile__(
        "addiu $sp, $sp, -0x40\n\t"
        "lv.q  R100, 0x0($a2)\n\t"
        "lv.q  R101, 0x10($a2)\n\t"
        "lv.q  R102, 0x20($a2)\n\t"
        "lv.q  R103, 0x30($a2)\n\t"
        "lv.q  R000, 0x0($a1)\n\t"
        "lv.q  R001, 0x10($a1)\n\t"
        "lv.q  R002, 0x20($a1)\n\t"
        "lv.q  R003, 0x30($a1)\n\t"
        "viim.s S400, 32\n\t"
        "vtfm4.q R200, M100, R000\n\t"
        "vtfm4.q R201, M100, R001\n\t"
        "vtfm4.q R202, M100, R002\n\t"
        "vtfm4.q R203, M100, R003\n\t"
        "vscl.t R200, R200, S400\n\t"
        "vscl.t R201, R201, S400\n\t"
        "vscl.t R202, R202, S400\n\t"
        "sv.q  R200, 0x0($sp)\n\t"
        "sv.q  R201, 0x10($sp)\n\t"
        "sv.q  R202, 0x20($sp)\n\t"
        "sv.q  R203, 0x30($sp)\n\t"
        "lui  $a0, 0x1E\n\t"
        "lw   $a1, -0x48F0($a0)\n\t"
        "cache 0x18, 0x3C($a1)\n\t"
        "lui  $a2, 0x2B00\n\t"
        "lui  $a3, 0x2B00\n\t"
        "lui  $t0, 0x2B00\n\t"
        "lwr  $a2, 0x1($sp)\n\t"
        "lwr  $a3, 0x5($sp)\n\t"
        "lwr  $t0, 0x9($sp)\n\t"
        "sw   $a2, 0x0($a1)\n\t"
        "sw   $a3, 0x4($a1)\n\t"
        "sw   $t0, 0x8($a1)\n\t"
        "lwr  $a2, 0x11($sp)\n\t"
        "lwr  $a3, 0x15($sp)\n\t"
        "lwr  $t0, 0x19($sp)\n\t"
        "sw   $a2, 0xC($a1)\n\t"
        "sw   $a3, 0x10($a1)\n\t"
        "sw   $t0, 0x14($a1)\n\t"
        "lwr  $a2, 0x21($sp)\n\t"
        "lwr  $a3, 0x25($sp)\n\t"
        "lwr  $t0, 0x29($sp)\n\t"
        "sw   $a2, 0x18($a1)\n\t"
        "sw   $a3, 0x1C($a1)\n\t"
        "sw   $t0, 0x20($a1)\n\t"
        "lwr  $a2, 0x31($sp)\n\t"
        "lwr  $a3, 0x35($sp)\n\t"
        "lwr  $t0, 0x39($sp)\n\t"
        "sw   $a2, 0x24($a1)\n\t"
        "sw   $a3, 0x28($a1)\n\t"
        "sw   $t0, 0x2C($a1)\n\t"
        "addiu $a1, $a1, 0x30\n\t"
        "sw   $a1, -0x48F0($a0)\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "addiu $sp, $sp, 0x40\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory", "$a0", "$a1", "$a2", "$a3", "$t0");
}