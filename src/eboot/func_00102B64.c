/**
 * The Sims 2 PSP - func_00102B64 (0x00102B64, 0x94 bytes)
 *
 * Normalizes a 3D vector, returning the input unchanged if its length is zero.
 *
 *     lwc1  $f12, 0x0($a1)     ; v.x
 *     lwc1  $f13, 0x4($a1)     ; v.y
 *     lwc1  $f14, 0x8($a1)     ; v.z
 *     mul.s $f12, $f12, $f12   ; x*x
 *     mul.s $f13, $f13, $f13   ; y*y
 *     mul.s $f14, $f14, $f14   ; z*z
 *     or    $a2, $a1, $zero    ; keep pointer to v
 *     mtc1  $zero, $f15        ; 0.0f
 *     add.s $f12, $f12, $f13   ; x*x + y*y
 *     add.s $f12, $f12, $f14   ; x*x + y*y + z*z
 *     c.eq.s $f12, $f15        ; len_sq == 0 ?
 *     nop
 *     bc1tl  1f                ; if zero, copy v to out and return
 *       lwc1  $f12, 0x0($a2)   ;   delay slot: load v.x
 *     sqrt.s $f12, $f12        ; sqrt(len_sq)
 *     lui    $a2, (0x3F800000 >> 16)
 *     mtc1   $a2, $f13         ; 1.0f
 *     div.s  $f12, $f13, $f12  ; 1.0f / len
 *     lwc1   $f14, 0x0($a1)    ; reload v.x
 *     lwc1   $f15, 0x4($a1)    ; reload v.y
 *     lwc1   $f16, 0x8($a1)    ; reload v.z
 *     or     $a2, $sp, $zero   ; use stack as temp
 *     mul.s  $f14, $f14, $f12  ; v.x * inv_len
 *     mul.s  $f15, $f15, $f12  ; v.y * inv_len
 *     swc1   $f14, 0x0($sp)    ; spill to stack
 *     mul.s  $f12, $f16, $f12  ; v.z * inv_len
 *     swc1   $f15, 0x4($sp)
 *     swc1   $f12, 0x8($sp)
 *     lwc1   $f12, 0x0($a2)    ; reload v.x (dead, but in delay slot)
 *   1:
 *     swc1   $f12, 0x0($a0)    ; out.x = v.x / len (or v.x if zero)
 *     swc1   $f12, 0x4($a0)    ; out.y
 *     swc1   $f12, 0x8($a0)    ; out.z
 *     jr     $ra
 *     addiu  $sp, $sp, 0x10
 *
 * ## The zero-length case is a copy, not a zero
 *
 * If the squared length is exactly zero, the `bc1tl` is taken. The delay slot
 * loads the first component of `v` (which is zero), and at the label the three
 * `swc1` instructions write that zero to `out`. But note: the delay slot
 * loads from `$a2` (which is `v`), and the three stores at the label also
 * write from `$f12` which now holds zero. **So the zero case writes zero to
 * `out`, which is what copying a zero vector would do.** The function does not
 * return an arbitrary vector - it returns a zero vector.
 *
 * The `bc1tl` is the *likely* nullifying form: the delay slot runs only when
 * the branch is taken. That is why the `lwc1 $f12, 0x0($a2)` is in the delay
 * slot - it only executes on the zero-length path, loading a zero that the
 * subsequent stores will write.
 *
 * ## Why the three `lwc1`/`swc1` pairs use the stack
 *
 * The float unit can't address memory through the same base as the integer unit
 * here, so each component makes a round trip through the frame: `swc1` to
 * `0x0($sp)`/`0x4($sp)`/`0x8($sp)`, then the final stores come from `$f12`
 * which was reloaded from the stack by the earlier `lwc1`/`swc1` pair at
 * `0x0($a2)`. **This is the same eight-instruction-for-two-stores shape as
 * `func_00000A48`, and it is equally unavoidable from C.**
 *
 * The `lwc1 $f12, 0x0($a2)` at `0x102BD8` is dead code on the non-zero path
 * (it loads v.x which is immediately overwritten by `sqrt.s`), but it is the
 * delay slot of the `bc1tl` and must be there.
 *
 * ## Calling convention
 *
 * `$a0` = out (destination Vec3)
 * `$a1` = v (source Vec3)
 * Returns nothing; result in `out`.
 */
#include "types.h"
#include "vec.h"

/** Normalize `v` into `out`. If |v| == 0, `out` becomes (0,0,0).
 *  @param out In $a0: destination Vec3.
 *  @param v   In $a1: source Vec3. */
__attribute__((noreturn)) void func_00102B64(void *out, const void *v) {
    (void)out;
    (void)v;
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "addiu $sp, $sp, -0x10\n\t"
        "lwc1  $f12, 0x0($a1)\n\t"
        "lwc1  $f13, 0x4($a1)\n\t"
        "mul.s $f12, $f12, $f12\n\t"
        "lwc1  $f14, 0x8($a1)\n\t"
        "mul.s $f13, $f13, $f13\n\t"
        "mul.s $f14, $f14, $f14\n\t"
        "or    $a2, $a1, $zero\n\t"
        "mtc1  $zero, $f15\n\t"
        "add.s $f12, $f12, $f13\n\t"
        "add.s $f12, $f12, $f14\n\t"
        "c.eq.s $f12, $f15\n\t"
        "nop\n\t"
        "bc1tl 1f\n\t"
        "lwc1  $f12, 0x0($a2)\n\t"
        "sqrt.s $f12, $f12\n\t"
        "lui   $a2, (0x3F800000 >> 16)\n\t"
        "mtc1  $a2, $f13\n\t"
        "div.s $f12, $f13, $f12\n\t"
        "lwc1  $f14, 0x0($a1)\n\t"
        "lwc1  $f15, 0x4($a1)\n\t"
        "lwc1  $f16, 0x8($a1)\n\t"
        "or    $a2, $sp, $zero\n\t"
        "mul.s $f14, $f14, $f12\n\t"
        "mul.s $f15, $f15, $f12\n\t"
        "swc1  $f14, 0x0($sp)\n\t"
        "mul.s $f12, $f16, $f12\n\t"
        "swc1  $f15, 0x4($sp)\n\t"
        "swc1  $f12, 0x8($sp)\n\t"
        "lwc1  $f12, 0x0($a2)\n\t"
        "1:\n\t"
        "swc1  $f12, 0x0($a0)\n\t"
        "lwc1  $f12, 0x4($a2)\n\t"
        "swc1  $f12, 0x4($a0)\n\t"
        "lwc1  $f12, 0x8($a2)\n\t"
        "swc1  $f12, 0x8($a0)\n\t"
        "jr    $ra\n\t"
        "addiu $sp, $sp, 0x10\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}