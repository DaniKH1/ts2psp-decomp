/**
 * The Sims 2 PSP - func_00000A48 (0x00000A48, 0xE0 bytes)
 *
 * Normalises an entity's position vector, with a floor on how short a vector
 * is allowed to get.
 *
 * The code first moves `*delta` into the entity's position, then measures the
 * result, stores that length at offset 0x54, and only computes the unit vector
 * when the length is non-zero.  Afterwards the length is *reloaded from the
 * entity* and compared against the epsilon - which is why the second test is
 * written against `self->cached_len` rather than the local `len`: the reload
 * is what makes the compiler unable to fold `len <= 0.0f` into
 * `len == 0.0f` after the `sqrt`.
 *
 *     pos += delta
 *     len  = |pos|                       -> cached at +0x54
 *     if (len > 0) dir = pos / len       -> written at +0x60
 *     if (len <= EPSILON) {              -> +0x58/+0x64 scaled by EPSILON
 *         pos   = dir * EPSILON
 *         len   = EPSILON
 *     }
 *
 * `EPSILON` is 0x3F7D70A4 = 2^-10.
 */
#include "types.h"
#include "vec.h"
#include "sim.h"

void func_00000A48(SimEntity *self, Vec2f *delta) {
    self->pos.x += delta->x;
    self->pos.y += delta->y;

    f32 len = self->pos.x * self->pos.x + self->pos.y * self->pos.y;
    len = sqrtf(len);
    self->cached_len = len;

    f32 eps = SIM_EPSILON;

    if (len > 0.0f) {
        f32 inv = 1.0f / len;
        self->dir.x = self->pos.x * inv;
        self->dir.y = self->pos.y * inv;
    }

    if (self->cached_len <= eps) {
        self->pos.x = self->dir.x * eps;
        self->pos.y = self->dir.y * eps;
        self->cached_len = eps;
    }
}
