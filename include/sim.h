/**
 * The Sims 2 PSP - core vector types and the object layout the first engine
 * functions work on.
 *
 * The engine keeps entity state in one big POD block whose members are
 * addressed by fixed offsets; the vector helpers below are the first code in
 * the module to touch it, and they pin down the layout of the leading fields.
 */
#ifndef SIM_H
#define SIM_H

#include "types.h"
#include "vec.h"

/* Offset 0x54: length of the position vector, cached.
 * Offset 0x58: position.
 * Offset 0x60: unit direction.
 *
 * The compiler reloads 0x54 rather than keeping it live, which is how the
 * "clamp short vectors" path below came to be written the way it is.
 */
typedef struct SimEntity {
    f32 cached_len;  /* 0x54 */
    f32 pad_0x58_[0];
    Vec2f pos;       /* 0x58 */
    Vec2f dir;       /* 0x60 */
} SimEntity;

/* Epsilon the length comparisons use: 2^-10. */
#define SIM_EPSILON 0.0009765625f

#endif /* SIM_H */
