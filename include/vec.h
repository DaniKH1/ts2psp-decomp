/**
 * The Sims 2 PSP - floating point type definitions.
 *
 * The engine stores vectors and matrices as plain float arrays in `.data`
 * and passes pointers to them around, so `Vec2f`/`Vec3f` are only used where
 * the structure is visible in the disassembly.
 */
#ifndef VEC_H
#define VEC_H

#include "types.h"

typedef struct {
    f32 x, y;
} Vec2f;

typedef struct {
    f32 x, y, z;
} Vec3f;

typedef struct {
    f32 x, y, z, w;
} Vec4f;

#endif /* VEC_H */
