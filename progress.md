# The Sims 2 PSP - decompilation

Target: `pgs-si2.iso` -> `/PSP_GAME/SYSDIR/EBOOT.BIN`, decrypted to
`disks/pgs-si2/EBOOT.dec`.

## Status

| metric | value |
| --- | --- |
| loadable bytes reproduced | 2,974,308 / 2,994,664 (99.32 %) |
| **code bytes identical** | **100 %** (507,673 / 507,673 instructions) |
| functions recovered | 7,497 |
| functions byte-identical | 7,497 (100 %) |
| relocations recovered | 66,503 / 66,503 |
| functions written in C | 96 (see below) |
| **C functions that byte-match** | **90** (linked from `src/`) |
| named symbols recovered | 3 functions + 3,754 strings |
| static constructors mapped | 320 (160 register file format tags) |

The assembly baseline is complete: the per-function `.s` that splat produced,
assembled with `psp-as` and linked with `psp-ld`, reproduces **every byte of the
module image** - a direct comparison of `0x74`..`0x1EFD84` shows 0 differing
bytes, and both `PT_LOAD` headers match the original including their alignment.
The only remaining ELF-level differences are `e_type` and the `.rel.*` /
`.symtab` / `.strtab` / `.shstrtab` link-time metadata, none of which are part
of the image.

This tree is the public one, and it is the second home of the project: it was
applated from a working copy that had the disc image in its history, so the
history here starts from a single clean commit and the package is 9.6 MB instead
of 574 MB.  See *What the repository does not contain* below for what the two
trees trade between them.

## What the module is

* 32-bit little endian MIPS (Allegrex), `o32`/eabi32, NOREORDER.  The ELF
  header reads `0x10a23001, noreorder, allegrex, eabi32, mips2`.
* Linked at base address 0 and relocated to `0x08804000` by the PSP loader, so
  every address in this project is module relative.
* No small data: no `.sdata`/`.sbss` and not one `R_MIPS_GPREL16`, so `-G0`.
  Every global is reached through an absolute `%hi`/`%lo` pair.
* The engine is Maxis' "Elem" (`c:/ad_clean/sims_psp/src/elem/...`).
* `BOOT.BIN` on the disc is the decrypted EBOOT, byte for byte.

### Layout

| section | vaddr | size | contents |
| --- | --- | --- | --- |
| `.text` | 0x00000000 | 0x1B0128 | C runtime, engine and game code |
| `.sceStub.text.*` | 0x001B0128 | 0x6F4 | PSPLINK import stubs (`jr $ra`) |
| `.text.collision` | 0x001B0820 | 0x33FC | collision module |
| `.text.sortAndCullScene` | 0x001B3C1C | 0x1C64 | scene culling |
| `.text.updateNodeGraph` | 0x001B5880 | 0x319C | |
| `.text.syncSkeleton` | 0x001B8A1C | 0x2840 | |
| `.text.drawing` | 0x001BB280 | 0xF70 | |
| `.text.renderCommon` | 0x001BC200 | 0x17D4 | |
| `.text.renderMeshInstances` | 0x001BDA00 | 0x169C | |
| `.lib.ent*`, `.lib.stub*` | 0x001BF09C | 0x228 | library stub tables |
| `.rodata.sce*` | 0x001BF2C4 | 0x4B4 | module info, resident list, NIDs |
| `.rodata` | 0x001BF880 | 0x1223C | strings, float tables, vtables |
| `.data` | 0x001D1B00 | 0x1C488 | initialised data |
| `.ctors`, `.dtors` | 0x001EDF88 | 0x10 | |
| `.cplinit` | 0x001EDF98 | 0xA10 | static constructor list |
| `.psp_lib_mark*` | 0x001EE9A8 | 0x64 | |
| `.linkonce.d` | 0x001EEA0C | 0x1300 | tentative definitions |
| `.bss` | 0x001EFD80 | 0xEB584 | |

## Toolchain: the original was CodeWarrior, not GCC

The decompilation is byte exact as assembly, and the C sources are the readable
form.  Getting C to match *instruction for instruction* needs the original
compiler, and the evidence says it was **CodeWarrior for PSP** (`mwccpsp` +
`mwldpsp`), the same toolchain the reference project
[tclamb/mhp2g-decomp](https://github.com/tclamb/mhp2g-decomp) uses for its EA
title.  Four independent signals:

1. **Float constants.**  Every float literal is built with
   `lui $reg, %hi(bits)` + `mtc1 $reg, $fN`.  No combination of `-O0..-O3`,
   `-ffast-math`, `-fno-unsafe-math-optimizations`, `-mgpopt`, `-mno-gpopt`,
   `-mfp32` or `-mhard-float` makes psp-gcc do this (it always puts float
   constants in `.rodata` and loads them with `lwc1`) - see
   `tools/probe_fconst.py`.
2. **Temporaries.**  Only `$f12` and up are used as FP temporaries, which is
   CodeWarrior's convention; GCC spreads them over `$f0`-`$f11`.
3. **Constant folding.**  `100.0f - 10.0f` is computed at run time with
   `sub.s` instead of being folded, and `sqrt(...) <= 0.0f` keeps a `c.le.s`
   against an explicit zero.
4. **Section names.**  `.text.collision`, `.text.drawing`, ... are per-module
   code sections, not `-ffunction-sections` output (which would be named after
   each mangled function).

`mwccpsp.exe` is proprietary and cannot be shipped here, so the build links
from `asm/` by default.  A C function is promoted into the image only once it
has been *proven* to compile to the original bytes: `tools/verify_c.py`
compiles each `src/eboot/*.c` on its own and writes the winners to
`config/matched_c.txt`, which `tools/gen_linker_script.py` reads.  The image
therefore stays byte exact no matter how far the C gets.

### Matching C is possible, it just needs the registers pinned

The four signals above are about *code generation*, not about a hard
impossibility.  Where a function leaves no freedom - a load, an add, a store,
a return, with no branches - psp-gcc emits the same instruction sequence as
CodeWarrior and only the *register choice* differs.  Ninety functions are
now byte exact this way:

```
func_00000000  void            the reciprocal bootstrap (floats)
func_000103AC  void            character mesh loader constructor (floats)
func_00012F3C  Flagged*        is bit 31 of flags set?
func_00029CFC  Counter*, u32   self->value = self->value - delta
func_00033940  void            flag = 1
func_00049A90  u32             return global[1]
func_00049AA0  u32             global[1] = value
func_00052604  Pair*, s32, s32 two-field setter returning self
func_0006B86C  RefCounted*     ++self->ref_count
func_00097784  u32             is the global at 0x1D4D9C set?
func_000A9128  Clearable*      clear 12 bytes, return self
func_000804B8  Flagged*        is bit 3 of flags set?
func_000BA9DC  Span*           (end - begin) / 4, rounding towards zero
func_000BA9FC  T*, u32         array[index], scaled index separate from the add
func_000BAA10  T*, u32         ... the same lookup, second owning type
func_000BF43C  HoldsString*    first non-NUL byte (branches)
func_000BF46C  HoldsString*    ... the same, another field
func_0007E6FC  void            set the flag byte at 0x1D48A8
func_000C2FE4  T*, Triple*     copy a 12-byte record into offset 0x80
func_00055928  T*, void*, f32* install a transform, set two flag bytes
func_00055974  T*, void*, f32* ... the same, block at a different offset
func_000C42C8  Growable*, s32  append a word, count at 0x10C
func_000D0C5C  Keyed*, Keyed*  a->key < b->key
func_000D3D38  T*             copy the pointer at 0x1D9E6C into offset 0x148
func_000D6B10  T*             ... a different global, into offset 0
func_000E3FE0  u32             is the global at 0x1BA290 set?
func_000E4B94  Timed*          now - start
func_000E12FC  void            set the flag byte at 0x1DA119
func_000F20F8  Clearable*      clear 12 bytes, return self
func_000F2FC0  Growable*, s32  ... the same append, different field order
func_000F210C  Clearable*      clear 12 bytes, return self
func_000F2200  T*, Triple*     ... a 12-byte record, another type
func_000F8908  Range*          end - begin
func_00101AE8  char*           vtable accessor, 0x1DB050
func_00101AF8  char*           vtable accessor, 0x1DB0AC
func_00101D24  char*           vtable accessor, 0x1DB014
func_00101D34  f32             read a float from the 0x1DB014 descriptor
func_00101D44  f32             ... the float one word further along
func_00102BF8  Transform*      publish a 16-word transform to 0xE69A8
func_00102CA8  Transform*      ... and the same to 0xE22E8
func_00106D74  Ordered*        link < link
func_0010F7CC  Pair*           (later - earlier) / 8, rounding towards zero
func_00128278  Clearable*      clear 12 bytes, return self
func_00123538  Node*, Node*    exchange two doubly linked list nodes
func_00123560  Node*, Node*    ... the same exchange, second entry point
func_001A9C40  T*, Vec3f*      install a vector, set flag bit 7
func_001A9C6C  T*, Vec3f*      ... the same, field at a different offset
func_0009C384  T*, f32         self->field += delta, accumulator in $f12
func_0009C394  T*, f32         ... the same, next field
func_000CE4DC  T*              return field 0xDC - field 0xD4
func_000CE4EC  T*              ... the same, right operand at 0xD8
func_000DF534  void*, V3f*, V3f*  copy a vector, $a0 unused
func_000DF550  void*, V3f*, V3f*  ... the same, emitted twice by CodeWarrior
func_000E3C4C  T*, f32         store a float, set the "present" flag byte
func_000E3C5C  T*, f32         ... the same, field at 0x54
func_000A9E90  T*              clear two words, return self
func_0012F854  T*              ... the same, adjacent words
func_00026BB0  T*, Pair*       gather two scattered floats, subtract, store a pair
func_000CDE98  T*, u32         items[count++] = value, count stored before the item
func_0012828C  T*, u32, u32    round down to 32, keep the relation to the delta
func_0012C954  u32, u32        ... the same, writing the global at 0x647D0
func_00058FD4  T*              (link + 1) & ~1 - round up to even
func_001AF15C  u32, u32        value & ~(limit - 1) - the alignment primitive
func_00116CD4  T*, u32, u32    constructor: two arguments, two -1 handles
func_000E7ECC  void            set the one-shot flag at 0x1DAA88
func_000471C8  T*              constructor: status -2, points at 0x1E52A0
func_00127868  T*, u32, u32    constructor: three self-referential pointer pairs
func_000E9D3C  u32             interpolate a 16-bit table at 0x1DA398
func_0014CE0C  u32 x4          64-bit weighted sum, no carry propagation
func_00128F50  Elapsed*        start - now
func_00140A58  u32             flags[index] & 0x07, packed flag bytes
func_00140A74  u32             ... the same, mask 0x04
func_00133678  u32             is the global at 0x1BF0D4 set?
func_00150988  Body*, Vec3f*   copy a 3-float vector out of an object
func_0016BC70  T*, Pair*       copy an 8-byte pair into offset 0x48
func_0016F698  T*, Pair*       ... the same, through a second type
func_0016F6B0  T*, Pair*       ... into offset 0x50
func_0016F6D8  T*, Pair*       ... into offset 0xB8
func_00171818  T*, Vec4f*      copy a 4-float vector out of an object
func_00193358  T*, Vec3f*      ... a 3-float vector, different offset
func_001933C8  T*, Vec4f*      ... a 4-float vector, different offset
func_00194ADC  T*, Triple*     ... a 12-byte record, different offset
func_00196628  T*, Vec4f*      ... a 4-float vector, different offset
func_001A9ABC  Flagged*        is bit 15 of flags set?
func_001A9ACC  Flagged*        is bit 17 of flags set?
func_001A9D54  Flagged*        set bit 20 of flags
func_001A9D94  Flagged*        set bit 18 of flags
func_001A9B00  T*, Vec4f*      ... a 4-float vector, different offset
func_001A9D80  Flagged*        is bit 20 of flags set?
func_001965E8  T*, Vec3f*      ... a 3-float vector, different offset
func_00196608  T*, Vec3f*      ... a 3-float vector, through a second type
func_001A69A0  u32, u32        is (a + b) a power of two?
func_001A69B8  u32, u32        ... the same, operands summed the other way
func_001AF144  u32, u32        ... and again
```

Four constraints came up repeatedly, and each is a property of the original
rather than a workaround:

* **Register allocation.**  CodeWarrior reuses the argument register for the
  result (`subu $a1, $a2, $a1`), GCC allocates `$v0`.  Pinning the computation
  with inline asm fixes it, and declaring the operands read-write (`"+r"`) is
  what stops GCC from keeping a copy it no longer needs.
* **Instruction order across the return.**  CodeWarrior puts the last
  instruction in the delay slot of `jr $ra`; GCC moves it above the return.
  Letting GCC emit its own `jr` and leaving a slot in the asm is what keeps
  that shape.
* **Identical in and out.**  When a pointer is both argument and return, making
  it the asm's output as well as its input (`"+r"`) means the copy is free.
  A `register ... asm("$a0")` variable is what puts it in the right register.
* **Floats.**  CodeWarrior starts FP temporaries at `$f12` and builds float
  literals with `lui` + `mtc1`; GCC uses `$f0`-`$f2` and loads them with
  `lwc1` from `.rodata`.  Both are pinned with `asm("$f12")` variables.

The C still says what the function means; the asm pins the instructions both
compilers already agree on.

### A register cannot be both an operand and a named hard register

Three errors that all look like the same mistake, and they are worth listing
together because each one's message points somewhere unhelpful:

* **An input operand needs a C expression.**  `[f] "f12"` in the *input* section
  fails with "expected `(` before `:`" - every input needs `(expr)`, and a bare
  register name is not one.  For a scratch with no C variable, name `$f12` in the
  template text and list `"$f12"` as a clobber.
* **An uninitialised hard register cannot be `"+r"`.**  "input operand constraint
  contains `+`".  If the asm writes the register before reading it, the constraint
  is `=&r` (output) or the register is listed as a clobber.
* **A compound literal has no register.**  `(u32){0}` cannot be an output operand
  bound to a hard register; declare a named variable instead.

The rule underneath all three: an uninitialised `register ... asm("$r")` variable
means *the asm produces this value*, so it belongs in the output section.

### GCC's delay-slot filler duplicates instructions, and that has a limit

`func_001AF15C` ends with an `and` and psp-gcc fills the delay slot by **emitting
the `and` a second time**.  That is safe because `and` is idempotent.

The limit is now known, from `func_000E9D3C`: its last instruction is `srav`, and a
duplicated shift would corrupt the result.  The fix is the inverse of the usual
rule - the final instruction goes in **C**, not in the asm, so GCC schedules it
into the delay slot exactly once.  So:

| last instruction | where it goes | why |
| --- | --- | --- |
| idempotent (`and`, `sw` of the same value) | asm | GCC may duplicate it safely |
| not idempotent (`srav`, `addu` into a counter) | C | GCC schedules it once, into the slot |

### A handful of fixed globals, and what they are for

Four functions now touch addresses directly, with no argument involved.  These are
worth listing together because they are the whole of what `.bss` looks like from
the code side:

| function | address | what it does there |
| --- | --- | --- |
| `func_000E7ECC` | `0x1DAA88` | one-shot flag, set to 1 |
| `func_000471C8` | `0x1E52A0` | an object's own global state, pointed at |
| `func_000E9D3C` | `0x1DA398` | a 256-entry table of 16-bit values |
| `func_0012C954` | `0x647D0` | the round-down state, written every call |

All four are built as `lui $reg, <page>` plus a signed displacement with
R_MIPS_HI16/LO16 relocations, never as an immediate operand - which is why they
survive a move in the address space, and why `andi` cannot be used for any mask
derived from them.

`0x647D0` is the odd one out: the other three are above `0x1D0000`, in what is
almost certainly the module's own data region, while `0x647D0` is low.  It may be a
different section, or a variable that predates the rest of the layout.

### GCC rewrites the code rather than just re-registering it

Six of the sixteen functions that followed the float work turned out to need
something the existing rules did not cover: psp-gcc **optimises the expression
away**.  Each of these produces the right value with fewer instructions, and in
each case the original has the longer form.

| function | written as | GCC emits | original |
| --- | --- | --- | --- |
| `func_00058FD4` | `(link + 1) & ~1u` | `ins $v0, $zero, 0, 1` | `addiu` then `and` |
| `func_001AF15C` | `value & ~(limit - 1)` | `negu $a1, $a1` | `addiu -1` then `not` |
| `func_0012828C` | `2*n + delta - aligned` | keeps the subtraction | three instructions |

`ins` is the sharpest of these.  It is an Allegrex-only bitfield instruction and
`addiu`+`and` is the portable two-instruction form of the same thing, so GCC is
using a target-specific instruction to beat a compiler that predates the
instruction set.  **psp-gcc is not a weaker compiler here; it is a differently
opinionated one**, and byte-exactness is as much about stopping its optimisations
as about fixing its register choices.

Two consequences worth knowing:

* **Operand order of `and` is not negotiable through C.**  GCC normalises it:
  `a & b` and `b & a` both compile with the same operand on the left.  When the
  original has `$a0` first, the `and` has to be written in asm - no rearrangement
  of the source will do it.
* **Putting the last instruction in asm means GCC must fill the delay slot, and
  it duplicates the instruction.**  That is only correct because `and` is
  idempotent - `x & y & y == x & y`.  A function whose final instruction had a
  side effect could not be written this way at all, so this trick is narrower than
  it looks.

### A shared idiom, found three times

`func_001AF15C` (limit as an argument), `func_0012828C` (the `0x1F`/`-0x20` pair
with the limit as an immediate) and `func_00058FD4` (round up to even) are one
operation - "align a value to a granularity" - with the granularity supplied
three different ways.  That makes it the engine's alignment primitive, and it
explains the `not`/`-0x20` mask idiom: `andi` takes a **zero-extended** 16-bit
immediate, so `andi $reg, 0xFFFE` would clear the upper sixteen bits as well and
give the wrong answer for any address above 64 KB.  `addiu` sign-extends, so it is
the only way to build these masks.

`func_0012C954` is the same round-down writing to a **global at `0x647D0`**, and
it is why that idiom is worth naming: `lui $a3, 0x6` with a `0x47D0` displacement
is the engine reaching for a fixed location directly, with no register holding
anything derived from an argument.

### Floats: the ten that needed no `.set noreorder` at all

The float functions were the open item for a long time, on the grounds that
CodeWarrior's FP habits were the hardest to reproduce.  Ten of them are now
byte-exact, and the useful finding is that **most needed nothing exotic** - the
disagreement is confined to which register is the destination, and four rules
cover it:

* **Name the float register as `$f12`, not as `12`.**  `register f32 x asm("12")`
  is accepted and silently ignored; `asm("$f12")` is honoured.  GCC's own choice
  of `$f0` as the destination of the operation is not something a binding will
  change either way, so the destination usually has to be named in the asm text.
* **A binding is only binding where GCC has no choice.**  `asm("$f0")` on a
  returned float works because `$f0` is the ABI's return register; the same
  binding on a second operand does not, because GCC treats `$f12` as free and
  picks `$f1` as its scratch instead.
* **Declare the asm's scratch as an earlyclobber output to reuse its register.**
  `register f32 c asm("$f12"); ... : [c] "=&f"(c)` tells GCC the value is already
  there, so the C store that follows uses `$f12` instead of reloading into `$f0`.
  Without it `func_000DF534` comes out four bytes too long.
* **Where the arithmetic is genuinely different, say so.**  `func_0009C384` is
  `self->field += delta`; GCC wants `add.s $f0, $f0, $f12` with the destination in
  the load's register, and CodeWarrior wants `add.s $f12, $f13, $f12` with the
  accumulator staying in the argument register.  No binding moves GCC off that,
  so the two instructions go in asm and the store stays in C.

The float functions where the *constant* is the problem - the `lui` + `mtc1` pair -
are a separate problem and still need `.set noreorder`; see below.

### Floats: `.set noreorder` is the missing piece

`func_00000000` was the first float function and it took one non-obvious fix.
It builds the 2^28 scale with `lui $a0, 0x4334` + `mtc1 $a0, $f13`, then
divides by `$f13` **one instruction later**.  The GNU assembler sees an FPU
write followed by an FPU read and inserts a `nop` hazard barrier; CodeWarrior
does not, and the original has no gap, because the Allegrex `mtc1` result is
available immediately.  `.set noreorder` around that pair removes the invented
`nop`.

The scope matters, and it turns out to be the same on both sides of the
function:

* too narrow and the `div.s` gets a `nop`; too wide and GCC loses the delay
  slot of the return, which is where the final `swc1` has to land.  In
  `func_00000000` `.set reorder` goes back right after the first `div.s`.
* `func_000103AC` needs the opposite: `.set noreorder` has to stay on for the
  whole body, because the `jal` instructions and their delay slots are the
  whole point of the function, and it ends just before the epilogue so GCC can
  schedule the frame teardown.

### `func_000103AC`, and two things the tooling got wrong

The character mesh loader constructor was the last of the three float
candidates, and it is a whole-body asm block rather than C with pins, for a
reason that is about the *compiler's* prologue rather than codegen: the frame
is created after the float work and `sw $ra` goes in after the first three
arguments are set up, so there is no prologue for GCC to emit.

Two tooling bugs surfaced while writing it:

* **`psp-ld` resolves an unknown name to the first symbol in the image rather
  than failing.**  `gen_linker_symbols.py` now emits an absolute assignment for
  every function name, so a candidate linked on its own calls the real callee.
  Without it `elem_register_chunk_tag` silently resolved to `func_00000000`
  and the comparison showed differences that were not there.
* **The last instruction of an asm block cannot reach the return's delay
  slot.**  The frame teardown has to be the *last* thing in the block, and the
  `jr` left to GCC - writing `jr` in the asm makes GCC append a second one.

### Finding the functions worth writing

`tools/c_candidates.py` ranks the remaining 7,485 functions by how likely they
are to be reproducible, weighting floats and branches against.

`tools/c_shapes.py` goes further and groups them by instruction sequence,
ignoring registers.  400 of the functions are float-free, branch-free and
call-free, which turns out to be too permissive: 146 of them are compiler
scaffolding rather than engine code - 64 are function-local static guards
(`addiu $sp, -0x10` / `sb $zero` / `lw` / restore) and a further 82 are C++
virtual dispatch thunks, both recognisable by stack stores or a bare `jalr`
through a vtable.  Excluding those leaves **254 functions in real shapes**.

`--done` hides the shapes whose members already have byte-exact C, which is
what makes the ranking useful day to day: it points at the next shape rather
than re-listing finished work.

The shapes that have paid off so far:

| shape | count | what it is |
| --- | --- | --- |
| `lw, lw, jr, subu` | 5 | elapsed-time and ordering comparators |
| `addiu, lwc1, swc1 x3` | 4 | copy a `Vec3f` out of an object |
| `addiu, lwc1, swc1 x4` | 4 | copy a `Vec4f` out of an object |
| `addiu, lw, lw, sw, sw` | 4 | copy an 8-byte pair into an object |
| `sw $zero x3, jr, move` | 4 | clear 12 bytes, return self |
| `lui, lw, jr, sltu` | 3 | is a global set? |
| `lui, addiu, jr, addiu` | 3 | C++ vtable accessor |
| `lw, lui, and, jr, sltu` | 3 | is a bit set in a flags word? |
| `lw x3, addiu, sw x3` | 3 | copy a 12-byte record into an object |
| `addu, addiu, addiu, not, and` | 3 | is (a + b) a power of two? |
| `lb, sltiu, andi, beql` | 2 | first non-NUL byte of a string |
| `lw, sll, addu, jr, lw` | 2 | array index, scale kept separate |
| `lw, andi, jr, sltu` | 2 | is a bit set? (`andi` not `and`) |
| `lw, lui, or, jr, sw` | 2 | set a bit in a flags word |
| `lw, sra, srl, addu, jr, sra` | 2 | divide by 4 or 8, rounding to zero |
| `ori, lui, jr, sb` | 1 | set a flag byte |

Three of the "power of two" functions differ only in the operand order of the
`addu` - `addu $a0, $a0, $a1` against `addu $a0, $a1, $a0`.  That is the same
value and the same instruction count, and it is why they are three separate
functions rather than one with two callers: CodeWarrior took the operand order
from the order the arguments appear in the source, so `a + b` and `b + a` got
different functions.

### Branches, and the comparison they hang on

`func_000BF43C` is the first function with a conditional that reproduces, and
two things were needed.

**The comparison.**  The original tests the byte with `sltiu $a1, $a1, 1`
followed by `andi $a1, $a1, 0xFF` rather than a single comparison against zero.
That is how the sign extension from `lb` is cancelled: `sltiu` against 1 gives 1
for every byte except exactly 0 and exactly 1, and the `andi` discards the bits
of the latter.  Written as `if (*p != 0)` psp-gcc emits one `bne` - a
completely different instruction sequence.  So the flag queries throughout the
module are written with the same two instructions even where the arithmetic
looks redundant, and it is the same reason the bit-flag tests end in `sltu $zero,
$v0` instead of returning the `and` result: the callers want a clean 0 or 1.

**The displacement.**  `beql` has to land *on* the instruction after its delay
slot, and the delay slot is the only instruction being skipped, so the target is
the `jr $ra` two words later.  A label after the delay slot puts the target one
word early and the `or` gets skipped too; GCC makes the same mistake in the
same direction, so the displacement is written out as `beql $a1, $zero, .+8`
with `.set noreorder` around it.

That is the easy kind of branch: both paths rejoin immediately, so nothing
depends on block layout.  Loops and multi-block conditionals are still untested
and are where layout will matter.

### Load in asm, store in C

`func_000D3D38` took five attempts and the thing that finally worked is worth
stating as a rule, because it generalises to every function that loads one
register and stores it:

* the **load** goes in the asm, because the `lui` + `lw` pair and the register
  choice have to be pinned;
* the **store** is left as C, because GCC then schedules it into the delay slot
  of the return it emits itself.

Writing `jr $ra` in the asm instead does not work: GCC appends its own return
afterwards and the function ends up with two.  Putting the store in the asm
alongside it does not work either - GCC leaves the delay slot empty and the
store ends up above the branch.  Only the split does it.

### Registers the original overwrites before reading

Two shapes here build an address in a register that already holds a live value
and overwrite it without reading it - `$a1` in `func_00140A58` and
`func_000D3D38`.  Declaring those `register ... asm("$a1")` variables
*uninitialised* is what reproduces it: give them an initialiser and GCC emits a
`move $a1, $a0` to reconcile the incoming value with the one the asm computes.

And `func_000D3D38` leaves its copied pointer in `$a1` on the way out, but no
caller reads it.  Declaring the C function `void` is what stops GCC adding a
`move $v0, $a1` and a second `jr $ra` that the original does not have.

### Long interleaved copies are generated, not typed

`func_00102BF8` is 140 bytes: sixteen words copied from the argument into the
global at `0x000E69A8`, with the loads and stores interleaved in a rotating
three-register pattern.  Sixteen interleaved load/store pairs is exactly the
kind of thing that reads correctly by eye and is off by one register, so
`tools/gen_copy_asm.py` emits the block from the word count and the destination:

```
python tools/gen_copy_asm.py --words 16 --dest-hi 0xE --dest-off 0x69A8
```

Three registers cover an unbounded run because each store trails the load that
filled its register by two, so the oldest of `$a1`..`$a3` is always free in
time.  Two details are worth recording because they are not obvious:

* the destination base is materialised *after* the first three loads, not
  before, because `$t0`/`$t1` are only wanted once those words are safely held;
* the last word loads into `$a0` - the object pointer has been read for the final
  time and is free - and its store is the one that goes in the return's delay
  slot.

The sibling `func_00102CA8` is the same function against `0x000E22E8`, and both
came out of the generator byte exact.

### Packed flag bytes

`func_00140A58` and `func_00140A74` read one bit out of the byte array at
`0x001E1B98`, with masks `0x07` and `0x04`.  The bytes there are `0xF0` and
`0x7F` side by side, which only makes sense if several booleans share one byte -
so the engine has a table of packed flag bytes rather than a byte per flag, and
these two functions are one-bit accessors over it.  They sit four words apart,
which is how the engine lays out a family of accessors over the same array.

The vector and pair copies are worth noting because they are *not* float-free -
they are `lwc1`/`swc1` throughout - and they matched first try.  The `$f12` pin
and the pattern for leaving the delay slot alone carry straight over, so the
float fix has far more reach than the one function that needed `.set
noreorder`.

The vtable accessors are the first sign of the class hierarchy coming back:
`0x001DB014`, `0x001DB050` and `0x001DB0AC` are three accessors in one group,
which is where the controllers that register `Start` and `ActiveController`
keep their type information.

The rule of thumb from the ninety that work: if the function has no
branches, or only branches that rejoin immediately, the arithmetic is what both
compilers already agree on, and only the registers are in question.

### Register bindings do not alias, and hard registers are not operands

Two rules, both from GCC rejecting the obvious form:

* Two `$a`-registers cannot be bound to the same C variable, and a named operand
  cannot also be an earlyclobber output.  `func_00123538` needs `$a2` and `$a3`
  separately, so it has two `register ... asm("$r")` variables.
* **A hard register cannot be an operand at all** if it is written but has no
  operand number to be listed under.  `func_001A9C40` uses `$t0` as a scratch
  register for the middle word of a vector; declaring `register u32 y
  asm("$t0")` and then listing `[t0] "+r"(y)` fails with "input operand
  constraint contains `+`".  The working form is to name `$t0` directly in the
  asm text and list it as a clobber:

  ```
  register u32 dst asm("$a3");
  __asm__ __volatile__(
      "lw  $t0, 0x4(%[a1])\n\t"
      ...
      : [a0] "+r"(obj), [a3] "+r"(dst)
      : "$t0", "$f12", "memory");
  ```

  So the division is: registers that carry a value across the block get a
  variable, registers the asm only scratches with go in the clobber list.

### An argument the function does not use

`func_000DF534` never reads `$a0`: it copies from `$a2` to `$a1` and leaves the
first argument slot alone.  Declared as the obvious `f(Vec3f *, Vec3f *)` the
destination lands in `$a0`, and pinning it back with `register ... asm("$a1")`
makes GCC emit two `move`s to get there - which the original does not have.

The fix is to declare the argument the original had and does not use:

```c
void func_000DF534(void *unused, Vec3f *dst, Vec3f *src)
```

with both pointers as plain `"r"` inputs, no bindings at all.  This is what a
deleted parameter looks like from the outside: registers are assigned
positionally, so an argument the body no longer needs still consumes its slot.
**A register the original never reads is evidence about the source, not noise** -
it means the source had a parameter here that has since been optimised away, and
`func_000DF534`/`func_000DF550` being byte-identical suggests whatever split
produced them is still visible in the original.

### Returning `this` needs the pointer on a `register` variable

`or $v0, $a0, $zero` in a return's delay slot - 99 functions end that way, and
the recipe was already worked out for `func_00052604` before it was needed again:

* the stores go in asm, because plain C makes GCC hoist the copy above them -
  `$v0` is not live across the stores, so nothing forces it to wait;
* the pointer is a **separate** `register ... asm("$a0")` variable, not the
  parameter itself, which GCC is free to re-allocate to `$a1`;
* it is the asm's output as well as its input (`"+r"`), so the value already in
  `$a0` counts as the result and GCC emits no second copy.

Getting there by trial and error wastes a build cycle per attempt: the answer was
sitting in `src/eboot/func_00052604.c` the whole time.  `tools/delay_slots.py`
lists the group.

### One `ori` feeding two stores

`func_00055928` builds the value 1 once with `ori $a1, $zero, 1` in the middle of
a float copy, and uses it for both flag byte stores at 0x109 and 0x10B.  Written
in C that is two `mov`/`ori` sequences; the original is one.  The position
matters too - the `ori` sits between two `lwc1`/`swc1` pairs rather than before
or after the group.

`$a1` is also overwritten by that `ori` and then reused at the end of the
function.  That looks wrong and is not: the original has already read
everything it needs out of `$a1`, and the tail walks a different object whose
pointer arrives in the same register.

### Fixed-point divide, and why the sign fixup is four instructions

`func_000BA9DC` and `func_0010F7CC` divide by 4 and by 8 rounding towards
zero, and the four-instruction fixup is the standard MIPS idiom: divide by the
shift, then add one back if the shifted-out bits were all ones *and* the value
was negative, so `-1 / 4` is 0 and not -1.  `srl $a1, $a1, 30` brings the sign
bit down to bit 0 and `addu` adds it.

psp-gcc emits this too, but drops the `srl` sign extraction and folds the
sequence into an `sra`/`add` pair.  So the whole four-instruction sequence is
pinned even though the semantics are identical - the clearest example yet of
the general pattern: **the two compilers agree on what the code means and
disagree on how many instructions it takes to say it.**

The shift amounts are the only difference between the two functions, and they
have to be `3`/`29` rather than `2`/`30` for the divide-by-8 case, because the
sign bit has to land on bit 0 after the shift.

## Pipeline

```
tools/iso9660.py             ISO9660 reader for the UMD image
bin/pspdecrypt/              Sony's PRX decryptor
tools/pspelf.py              ELF access layer, relocation recovery
tools/gen_splat_config.py    ELF -> splat config + symbol/reloc maps
tools/gen_linker_symbols.py   absolute linker symbols, exact data blobs
tools/postprocess_asm.py      cross-file labels global, unpaired %hi folded
tools/gen_linker_script.py    module-shaped linker script
tools/build.py                compile, link, compare
tools/progress.py             per-function matching report
tools/verify_c.py             does this C compile to the original bytes?
tools/try_func.py             compile one candidate C file and diff it
tools/disasm_range.py         disassemble a window with relocations
tools/diff_words.py           first differing words, original vs built
tools/check_headers.py        ELF/program header comparison
tools/check_layout.py         per-section address/offset deltas
tools/fix_segments.py         pad segment 1 to the original's 8-byte alignment
tools/check_image.py          byte-for-byte comparison of the module image
tools/cplinit.py              the static constructor table
tools/tags.py                 decode the packed four-character chunk tags
tools/rodata_strings.py       recover the string literals in .rodata
tools/subsystems.py           group constructors into subsystems by tag signature
tools/c_candidates.py         rank functions by how likely psp-gcc can match them
tools/c_shapes.py             group the simple functions by instruction sequence
tools/gen_copy_asm.py        emit the asm for a long interleaved load/store copy
tools/paths.py               where the module image and the toolchain live
tools/setup.py               report which of the two this clone is missing
tools/delay_slots.py         group functions by what sits in the return's slot
```

```
python tools/split.py --build        # regenerate everything and compare
python tools/build.py --stats        # compare without rebuilding
python tools/progress.py --top 40    # what is left, largest first
python tools/verify_c.py --adopt     # check the C, promote the winners
python tools/tags.py --by-func       # which subsystem registers which file tag
python tools/rodata_strings.py       # the recovered string literals
python tools/setup.py --check        # is this clone ready to build?
```

`tools/build.py --diff` relinks before comparing, which would undo the segment
padding, so the pipeline compares with `--stats` after `fix_segments`.

### Grouping by delay slot, and the stack census it turned up

`tools/delay_slots.py` groups all 7,503 functions by the instruction in the return's
delay slot.  The expectation was a work list; what it produced is mostly
information nobody had.

The delay slot is the last instruction of the function, so it is where psp-gcc and
CodeWarrior disagree most - scheduling into a slot is a choice, not a
requirement.  That part is a to-do list: 99 functions end in `or $v0, $a0, $zero`
("return `this`"), 128 in `or $v0, $zero, $zero` ("return 0"), and 662 leave it as
a `nop`.

The rest is a **census of how much stack each function needs**, for 5,788 functions
nobody has disassembled by hand.  They teardown a frame in their delay slot, the
frame size is the key, and there are 88 distinct sizes:

| delay slot | count | frame |
| --- | --- | --- |
| `addiu $sp, $sp, 0x20` | 2,955 | 32 bytes |
| `addiu $sp, $sp, 0x30` | 963 | 48 bytes |
| `addiu $sp, $sp, 0x40` | 521 | 64 bytes |
| `addiu $sp, $sp, 0x50` | 297 | 80 bytes |
| `addiu $sp, $sp, 0x10` | 191 | 16 bytes |
| `addiu $sp, $sp, 0x60` | 142 | 96 bytes |

In the o32 frame layout the bytes are the saved `$ra`, then any saved `$s`
registers, then spill slots, so the size is a floor on how many values the
compiler could not keep in registers.  It is obtained without reading a single
instruction of the body, and it is also a way to find the functions worth reading
first: a 0x60-byte frame means several values spilled, and spilled values are the
ones with real logic around them.

### What the repository does not contain, and how a clone supplies it

Two inputs are deliberately absent: the decrypted module image (Maxis/EA
material) and the psp-gcc toolchain (a 125 MB third-party build).  Neither can be
committed, so `tools/paths.py` resolves both from the environment:

| variable | what | default |
| --- | --- | --- |
| `TS2PSP_ELF` | the decrypted module image | `disks/pgs-si2/EBOOT.dec` |
| `TS2PSP_PSPDEV` | the psp toolchain's bin directory | `bin/pspdev/bin` |
| `TS2PSP_MSYS` | the MSYS2/Cygwin bin directory that hosts the toolchain | `C:/msys64/usr/bin` |

The defaults are the in-tree paths, so a working copy that has the files in place
needs no environment at all; only a fresh clone has to be told.  Every tool reads
these through `paths.py` rather than hard-coding a location, which is what makes
the repository portable rather than tied to one machine's directory layout.

One thing worth recording because it nearly shipped: `config/eboot.splat.yaml` is
a committed artifact, so it must not carry a path from the machine that generated
it.  `gen_splat_config.py` writes the portable in-tree default and `split.py`
substitutes the resolved path for the duration of the splat call and puts the
default back afterwards.  Without that, a single pipeline run left one machine's
directory layout in a tracked file.

### The build is incremental

The module is ~15,000 translation units and only one or two change per
decompilation iteration, but every iteration rebuilt all of them.  `build.py`
now skips a unit whose object is at least as new as its source *and* every
header that source includes, so editing `include/types.h` still rebuilds
everything that depends on it.

| | before | after |
| --- | --- | --- |
| no changes | 133 s | 54 s |
| one C file changed | 133 s | ~55 s |
| `include/types.h` touched | 133 s | ~54 s + 0.8 s for the 48 dependents |

The 54 s that remains is the link, which has to be redone in full every time:
the generated linker script names all 15,000 objects explicitly and the layout
has to be recomputed.  `--force` restores the from-scratch behaviour, which is
what a clean verification run wants.

Getting the filter the right way round mattered - the first version kept the
*up-to-date* units and rebuilt all of them, which looked like it was working
because the object count was right.  `build.py` now prints `compiled N of M`
rather than just `compiled M`, so an inverted filter is visible immediately.

### Notes on the relocation recovery

PSPLINK leaves the references in place (`SHT_MIPS_REL`, 8 byte entries, no
addend), so every target has to be reconstructed.  Three things are easy to get
wrong and all three bit:

* `%hi`/`%lo` are *not* adjacent.  GCC sinks `%hi` into branch delay slots, so
  one `lui` can feed several `%lo`s several instructions later, and CSE leaves
  copies of `lui` whose `%lo` partner lives in another function.  `pair_hilo`
  matches a `%lo` to the closest preceding `%hi` for the register it is added
  to, then recovers the leftover copies by value (`tools/pspelf.py`).
* The linker's `%hi` is the address *rounded up* to the next 64 KiB and `%lo`
  is the signed difference, so the low half has to be sign extended when the
  two are recombined.  Getting this wrong shifts a symbol by 64 KiB and the
  `lui` by one.
* `.rel.cplinit` and friends address their target section *relatively* while
  `.rel.text`/`.rel.data`/`.rel.rodata` use absolute addresses.

## Remaining ELF-level differences

Not part of the module image, so they affect neither matching nor loading:

* `e_type`: the shipped file has `0xFF80` (PSPLINK's marker), we emit
  `ET_EXEC`.
* `.rel.*`, `.symtab`, `.strtab` and `.shstrtab` are link-time metadata.

Both `PT_LOAD` headers now match the original exactly, including the 8-byte
alignment of segment 1 and the four padding bytes it implies.  That padding
cannot come from a linker flag: `-z max-page-size=4` is required so segment 0
starts at file offset 0x74, and `max-page-size` is global, so no single value
gives segment 0 an alignment of 4 and segment 1 an alignment of 8.
`tools/fix_segments.py` therefore inserts the padding after the link and fixes
`p_offset`, `p_align`, `sh_offset` and `e_shoff` to match.

Note that `tools/build.py --diff` compares section *contents*, which is why this
shift went unnoticed: every section was correct while the file bytes after
`0x1ee00c` were four bytes out.  `tools/check_image.py` compares the module
image itself and is what catches it:

```
2030864/2030864 image bytes identical (100.00%)
  module image is byte identical
```

## Recovering names

The module ships with its symbol table stripped, so names have to come out of
analysis.  `config/eboot.names.txt` is where recovered names accumulate;
`tools/gen_splat_config.py` merges it over the `func_XXXXXXXX` placeholders.

Three sources have proved worth reading:

**String literals.**  3,754 NUL-terminated ASCII strings survive in `.rodata`
and every one is a relocation target, so each has a known address.  They are
named `str_<text>` and the generated asm now reads `str_BoidBehavior_avoidWalls`
instead of `%hi(sym_001BF8A0)`.  They are largely the game's tuning and
behaviour parameters, so they also document the subsystems.

**The static constructor table.**  `.cplinit` is 320 function pointers in link
order, i.e. one per translation unit - the closest thing the binary has to a
symbol table.  160 of them register file format tags.

**Packed four-character tags.**  `elem_register_chunk_tag` (0x00106D84) takes a
32-bit value holding four characters, unpacks it byte by byte into a C string
and adds it to the chunk table of the loader.  Decoding those constants names
the Sims 2 asset formats: `surf`, `bmsh`, `body`, `banm`, `gshd`, `indx`,
`sdta`, `node`, `levl`, `font`, `lua!`, plus the sound banks `wave`, `musc`,
`xms `.  `python tools/tags.py --by-func` groups them by constructor, and the
result is saved as `config/eboot.tags.txt`.

Recovering the tags needed forward symbolic execution over each constructor
rather than a backwards scan: GCC keeps several `lui`s live at once, the low
half is materialised in the *branch delay slot* (so it is applied after the
`jal` in address order), and `mtc1 $a0, $f13` reports `$a0` as its destination
GPR without writing it.

Names identified so far: `elem_register_chunk_tag`, `elem_operator_new` (it
raises `"Out of memory -- allocation of %d bytes failed."`, the C++
replacement `new`) and `elem_throw_bad_alloc`.

### Subsystem map

`python tools/subsystems.py` groups the constructors by the tags they register
and writes `config/eboot.subsystems.txt`.  Half of them register no tags at all
(they only touch the shared runtime), so the baseline is taken as what the 160
that *do* register agree on: `gshd`, `sdta`, `surf`, `tern`, `wave`, `xmb `,
`xmh `, `xms ` - the sound banks and the script loader, which every translation
unit links.  Subtracting that baseline leaves the subsystems:

| constructors | extra tags | subsystem |
| --- | --- | --- |
| 30 | `levl`, `ndbg`, `node`, `rdms` | level streaming |
| 27 | `ndbg`, `node`, `rdms` | node graph / scene traversal |
| 17 | `font` | text and font rendering |
| 17 | `tabl` | tuning tables |
| 6 | `banm`, `bmsh`, `body` | character mesh and animation |
| 3 | `indx` | index files |
| 185 | - | shared runtime only |

Individual constructors are named by the literals they carry, which are
behaviour and script names: `LuaButtonMap` with `pushButtonMap`/`popButtonMap`,
`Chase` with `StartDecisionMaker`/`StopDecisionMaker`, `print graph/do level
graph`, `renderCommon`, `syncSkeleton`.  Those literals are also how the
controller framework is visible - 30-odd constructors register `Start` and
`ActiveController`, which is the base class every controller derives from.

Grouping by tags works where grouping by strings does not: the shared table
means most constructors mention `JoystickX` and `Confirm`, so the first
capitalised string is a control name rather than the module.

## Work list

1. Keep working down `tools/c_shapes.py --done`.  254 real-shape functions were
   identified and 90 are done.  Each shape that works yields several functions
   at once, and the established rules ("load in asm, store in C", "leave an
   overwritten register uninitialised") keep the per-function cost down.
2. The work is hand transcription, deliberately.  A generator *can* emit all
   7,497 function bodies as verbatim asm and does - it was built and measured,
   and it verified 7,497/7,497 - but that is a transcription, not a
   decompilation, and it answers a question this project is not asking.  The
   readable layer is the point, and each sequence needs a person to work out
   what it does.  `tools/gen_copy_asm.py` stays as a helper for the sixteen-word
   copies, where the pattern is long enough to be error-prone but still has to be
   understood.
2. Branches are no longer a blocker, but only the easy kind: a branch whose
   delay slot holds the only instruction it skips, so both paths rejoin
   immediately.  Loops and multi-block conditionals are still untested, and
   those are where block layout will actually matter.
3. **Float functions are largely cracked.**  Twelve are byte-exact; see *Floats: the
   ten that needed no `.set noreorder`* for the four rules.  What is left of the
   old float problem is only the `lui` + `mtc1` constant idiom, which still needs
   `.set noreorder`.  The next float work should be arithmetic that returns into
   `$f0` (`func_0010F7CC` does the divide; a `mul.s` or `sqrt.s` sibling would
   test the same rules against a different instruction).
4. Four functions now read floats out of the type descriptor at `0x001DB014`
   (`func_00101D24`, `func_00101D34`, `func_00101D44`), which confirms that
   address is a class descriptor rather than a plain vtable.  Working out what
   its fields mean is the next step towards naming the controller classes.
5. The packed flag bytes at `0x001E1B98` are worth mapping: the one-bit
   accessors over them will name the individual booleans.
6. **Use the stack census to choose what to read next.**  `tools/delay_slots.py`
   says 5,788 functions have a frame; the 88 distinct sizes are a free bound on
   each one's local count.  Starting from the large frames would find the
   interesting functions far faster than going by address order, which is how the
   first 78 were found.
7. Recover vtables in `.rodata` and give them `ClassName_methods[]` names.  The
   three accessors at `0x001DB014`, `0x001DB050` and `0x001DB0AC` are the
   start of this, and the controllers that register `Start` and
   `ActiveController` should key off them.
8. Read `.rodata.sceNid` for the import list and name the PSP API stubs.
9. Name the 185 constructors that only touch the shared runtime, using the
   functions they call.
