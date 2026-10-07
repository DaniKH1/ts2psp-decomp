/**
 * The Sims 2 PSP - func_000000B0 (0x000000B0, 0xA0 bytes)
 *
 * Weighted blend of two 2D vectors, scaled back down:
 *
 *     out = (a * 90.0f + b * 10.0f) * 0.01f
 *
 * 90 + 10 = 100 and 0.01 = 1/100, so the three constants multiply out to a
 * convex combination - "90% a, 10% b".
 *
 * The original source reached the two intermediates through local structs,
 * which is why the compiled code spills both of them to the stack and reloads
 * them instead of keeping them in the FPU.  `100.0f - 10.0f` is computed at
 * run time rather than folded to `90.0f`, which pins down the source form:
 *
 *     a * (100.0f - 10.0f)  +  b * 10.0f
 */
#include "types.h"
#include "vec.h"

void func_000000B0(Vec2f *out, Vec2f *a, Vec2f *b) {
    Vec2f wa = { a->x * (100.0f - 10.0f), a->y * (100.0f - 10.0f) };
    Vec2f wb = { b->x * 10.0f, b->y * 10.0f };
    Vec2f sum = { wa.x + wb.x, wa.y + wb.y };

    out->x = sum.x * 0.01f;
    out->y = sum.y * 0.01f;
}
