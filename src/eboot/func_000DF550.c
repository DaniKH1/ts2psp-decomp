/**
 * The Sims 2 PSP - func_000DF550 (0x000DF550, 0x1C bytes)
 *
 * Copies a three-float vector from the third argument to the second.
 *
 * Byte for byte the same as func_000DF534; see that function for the whole
 * reading, and in particular for the unused first argument that leaves `$a0`
 * untouched.  The one detail worth repeating here is the register declaration:
 *
 *     register f32 component asm("$f12");
 *
 * uninitialised, because the asm writes it before reading it, and made an
 * earlyclobber output.  Declaring it is what stops GCC reloading the last
 * component into `$f0` for the final store - the value is already in `$f12` and
 * GCC is told so.  With no such declaration the block ends after the fifth
 * instruction, `dst->z = component` becomes `swc1 $f0, ...` fed by a fresh
 * `lwc1 $f0`, and the function comes out four bytes too long.
 */
#include "types.h"
#include "vec.h"

void func_000DF550(void *unused, Vec3f *dst, Vec3f *src) {
    register f32 component asm("$f12");

    (void)unused;

    __asm__ __volatile__(
        "lwc1 %[c], 0x0(%[src])\n\t"
        "swc1 %[c], 0x0(%[dst])\n\t"
        "lwc1 %[c], 0x4(%[src])\n\t"
        "swc1 %[c], 0x4(%[dst])\n\t"
        "lwc1 %[c], 0x8(%[src])\n\t"
        : [c] "=&f"(component)
        : [dst] "r"(dst), [src] "r"(src)
        : "memory");

    dst->z = component;
}