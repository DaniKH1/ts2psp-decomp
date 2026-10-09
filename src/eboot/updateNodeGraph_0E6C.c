/**
 * The Sims 2 PSP - updateNodeGraph_0E6C (0x001B66EC, 0x8 bytes)
 *
 *     jr    $ra
 *       or    $v0, $a0, $zero
 *
 * **Returns its own first argument unchanged.**
 *
 * **`or $v0, $a0, $zero` is the classic no-op move** - a register-to-
 * register `or` with `$zero` as one source copies the other source
 * through the ALU and sets no flags, so `$v0` ends up holding exactly
 * what `$a0` held.  The encoding is the word that proves it: `25108000`,
 * where the destination field is `$v0` and the source field is `$a0`.
 *
 * A function whose whole body is `return this;` is the identity, and
 * that is the only reading consistent with the bytes: no memory is
 * touched, no branch is taken, no callee is invoked.  **Where it sits is
 * the interesting part.**  It lives in `.text.updateNodeGraph` - the
 * scene-graph update code - and an identity function there is either a
 * virtual method whose contract is "do nothing, succeed" or a stub for a
 * node type that needs no per-frame work.
 *
 * **Which of the two cannot be determined from these eight bytes.**  The
 * register `$a0` is passed through without being read, so nothing here
 * says whether the pointer was null-checked, whether the caller even
 * passed a pointer, or what the return value is supposed to mean.
 */
#include "types.h"

__attribute__((noreturn)) void updateNodeGraph_0E6C(void) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "jr    $ra\n\t"
        "or    $v0, $a0, $zero\n\t"
        ".Leboot_001B66EC:\n\t"
        ".set reorder\n\t"
        :
        :
        : "memory");
}