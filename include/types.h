/**
 * Fixed width integer types used across the decompilation.
 *
 * The PSP is a 32 bit MIPS (Allegrex) machine built with the o32/eabi32 ABI,
 * so `long` is 32 bits wide - matching what the original engine assumed.
 */
#ifndef TYPES_H
#define TYPES_H

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;

typedef float f32;
typedef double f64;

typedef int UNK_TYPE;
typedef unsigned int size_t;

#define NULL ((void *)0)

/* The compiler folds these to the single `sqrt.s` / `div.s` the original
 * emits; psp-gcc's headers are not on the include path (the decompilation
 * supplies no libc), so they are declared here. */
f32 sqrtf(f32 x);
f32 fabsf(f32 x);
f32 floorf(f32 x);
s32 abs(s32 x);

#endif /* TYPES_H */
