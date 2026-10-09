/**
 * The Sims 2 PSP - updateNodeGraph_0420 (0x001B5CA0, 0x20 bytes)
 *
 *     lw    $a1, 0x8($a0)
 *     lw    $a0, 0x1C($a0)
 *     beqz  $a0, .Leboot_001B5CB8
 *       lwc1    $f0, 0x0($a1)
 *     lwc1    $f12, 0x44($a0)
 *     mul.s    $f0, $f0, $f12
 *   .Leboot_001B5CB8:
 *     jr    $ra
 *       nop
 *
 * Returns `arg->f_08[0]`, multiplied by `arg->f_1C->f_44` if
 * `arg->f_1C` is not null.
 *
 * **The delay slot is the first load, and `beqz` does not nullify,**
 * so `lwc1 $f0, 0x0($a1)` runs on both paths: `$f0` is always
 * `*(float *)arg->f_08` and only the *multiply* is conditional.  The
 * `mul.s` therefore cannot fault on a null `arg->f_1C`, because the
 * branch is what guards it and the branch sits after the load.
 *
 * **The `sra`/`addu` rounding dance is absent here, which is the tell
 * that this multiply is a plain scale and not a division.**  Compare
 * `sortAndCullScene_1024`, which spends seven instructions dividing a
 * byte count by 4 with the correct sign correction; this one just
 * multiplies.
 *
 * `0x44` is the interesting field: **the same offset is the one
 * `updateNodeGraph_036C` compares an incoming `$f12` against, stores
 * back, and then invalidates through `updateNodeGraph_03A0(this, 1)`.**
 * So `arg->f_1C` is an object
 * carrying a scale-like float at `0x44` that the graph code keeps
 * synchronised.  Whether that float is a scale, a correction factor or
 * an inverse is **not determinable from these bytes** - a single
 * unconditional `mul.s` cannot distinguish them.
 *
 * Nothing is written: the function is a pure getter, and the only
 * output is `$f0`.
 */
#include "types.h"

__attribute__((noreturn)) void updateNodeGraph_0420(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "lw    $a1, 0x8($a0)\n\t"
        "lw    $a0, 0x1C($a0)\n\t"
        "beqz  $a0, .Leboot_001B5CB8\n\t"
        "lwc1    $f0, 0x0($a1)\n\t"
        "lwc1    $f12, 0x44($a0)\n\t"
        "mul.s    $f0, $f0, $f12\n\t"
        ".Leboot_001B5CB8:\n\t"
        "jr    $ra\n\t"
        "nop\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}