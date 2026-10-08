/**
 * The Sims 2 PSP - func_001433F8 (0x001433F8, 0x10 bytes)
 *
 * Loads a pointer from a fixed global, stores the argument at offset 0x58
 * of that pointer, and returns the pointer.
 *
 *     lui  $a1, 0x1E
 *     lw   $a1, 0x1F8C($a1)   0x1E0000 + 0x1F8C = 0x1E1F8C
 *     jr   $ra
 *     sw   $a0, 0x58($a1)      *pointer = arg, in delay slot
 *
 * **The return value is the pointer** - `$a1` holds the loaded pointer, and
 * the delay slot stores through it.  The function does not explicitly move
 * `$a1` to `$v0`, so the return is effectively the pointer in `$a1`.  The C
 * transcription returns it explicitly.
 *
 * The global at 0x1E1F8C holds a pointer - this is a level of indirection
 * where the global is a pointer to a structure, and the function writes to
 * offset 0x58 of that structure.
 */
#include "types.h"

/* 0x1E0000 + 0x1F8C.  Low global page holding a pointer. */
#define GLOBAL_PTR  0x0001E1F8Cu

typedef struct Target {
    u8  pad[0x58];
    u32 value;   /* 0x58 - set to argument */
} Target;

__attribute__((noreturn)) Target *func_001433F8(u32 value) {
    register u32 v asm("$a0") = value;
    __asm__ __volatile__(
        "lui  $a1, 0x1E\n\t"
        "lw   $a1, 0x1F8C($a1)\n\t"
        ".set noreorder\n\t"
        "jr   $ra\n\t"
        "sw   %[v], 0x58($a1)\n\t"
        ".set reorder\n\t"
        : : [v] "r"(v)
        : "memory", "$a1");
}