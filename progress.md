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
| functions written in C | 639 (see below) |
| **C functions that byte-match** | **639** (linked from `src/`) |
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
CodeWarrior and only the *register choice* differs.  One hundred and thirteen
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
func_0009D658  void*, u32, u32 advance a 92-byte stride, store a word at +0xB0
func_0009D5BC  void*, u32, f32  ... the same, two floats at +0xA0 and +0xA4
func_00097B54  void*, u32       &items[index], 168-byte stride at offset 0x88
func_000E9BB8  const u8*        little-endian 32-bit read, four lbu and no lw
func_000F8F88  T*, u32          store, set a flag word, return 0
func_000E8EA8  u32, u32         tent weight: y + (x-128)*(128-|y-128|)/128
func_001428E4  void             .cplinit: register(1) for ever
func_000CD5B0  void*            for (i=0;i<1;i++) *p++ = 0; return self
func_0009C9B4  T*               clear 12 floats after a flag, countdown loop
func_0014402C  const char*      strlen, with the loop's first test peeled out
func_00137A04  u32, s32         low `count` bits, then bit 0 cleared
func_0012EB30  u32             &entry[i], 164-byte stride, array at 0x1DEE08
func_00049C50  u32             &entry[i], 2304-byte stride, two-part base
func_00049C7C  u32             ... the same, plus 0x808 on the result
func_00049BC4  u32             &entry[i], 140264-byte stride, a real `mult`
func_00049B3C  u32             ... the same, a field 0x2000 further on
func_0018DDAC  void*, Link*    unlink a node: -2 and a pointer to a global
func_0002D630  Handle*        set the descriptor to 0x1E4988, return the object
func_000FE624  void            zero four words at 0x28..0x3C of the global 0x61A18
func_000FFBE8  void            ... and set two floats at 0xEC and 0xF0 of it
func_0007E024  void*           copy one 2-float global into two slots, return self
func_001241AC  ListHeader*     prev = next = self, count = 0, descriptor set
func_00124658  Counter*        two zeros then a pointer to a global, return self
func_0012C2D0  Obj*, u32, u8   constructor that also allocates an id from a global
func_00143408  u32             one step of a linear congruential generator
func_000AFE00  s32             step a global object's cursor back by 4, return 0
func_000D6AFC  void            copy a doubly-indirected global into the argument
func_00100DC0  s32             compare two 28-bit-masked globals, via a detour
func_00102A54  void            fill 26 constants at a cursor, then advance it 0x68
func_00036D40  Node*, Node*    splice a node onto a global chain, return it
func_0014C958  Node*, u8*, u32 push a tagged node; fill a 9-word block, one word set
func_0013B2D0  s32, 4 outs     four out-parameters: two values, two addresses
func_00097200  void            publish one value to a global and to field 0x80
func_000D3490  void*, void*    register two globals and bump a counter
func_001282B4  void            bump-allocate: cursor += amount, rounded up to four
func_000B9CBC  Packed*         next record in a packed array, via a byte-count span
func_00140AC4  s32             `setjmp`: save the callee-saved context
func_00143A18  s32             `strstr`, hand-rolled (see the note below)
func_000A9A00  s32             a field that is set gives 0, clear gives -24
func_000BBA80  s32             a byte field on a second object, as a boolean
func_0001270C  s32             sign-extend bit 30 with xor and subu, no branch
syncSkeleton_27D0  void        vector unit: scale a 3x4 matrix by three scalars
syncSkeleton_2808  void        vector unit: multiply a 3x4 matrix by a vector
func_00130830  void            a `break 768` - a trap used as a callback placeholder
func_00103E88  void            six-word copy out of self+0xC, three registers deep
func_000EE56C  void            the same rotation, sixteen words, out of self+0x110
func_00197414  void            lerp: out = a + t * (b - a), three floats
func_001AF16C  void            the same, four floats - and $f0 gets used
func_000AD440  void            set two adjacent bytes to 1
func_000E1000  void            two globals to 3 and -1, in two different regions
func_00025594  void            the module's only empty body with a live frame
func_00046CE4  Node*           four-word constructor: tag 5, -2, a global, an owner
func_000F9414  void            set field 0 of a 28-byte-stride element to 2
func_00082134  s32             write 0x2000 through argument 2, return 0
func_000E4970  u32             clear three words at 0xC, return 0
func_000F7D38  u32             the same twenty bytes, verbatim
func_00151240  u32             one zero byte out, four bytes back - all 155 callers read one
func_00096F40  void            zero three floats through the second argument
func_00005B00  s32             return 0 - one of 110
func_000068E8  void            empty body - one of 162, the largest group
func_001342D8  void            big-endian append of a 16-bit value to a byte buffer
func_000FCC78  void            flag + two floats from $f12/$f13
func_000D2A34  void            flag + word + three floats in unusual order
func_0012E9D4  void            word=1 then byte=1 (with redundant ori)
func_001029C8  void            three words to 64-byte-stride element, 5th arg in jal delay slot
func_000F92A0  void            28-byte stride: read-modify-write word0, write adjusted args
func_000E9AD0  u16            fixed-point: div/4 round, negate, *64, +7744, mod 65536
func_0004AA60  u32            arg * 264 + 503236 + 8 (strength-reduced multiply)
func_000E130C  u32            global byte == 1 (xori + sltiu)
func_00133ABC  void            read global byte, store through arg, return garbage
func_001433F8  Target*         load global ptr, store arg at 0x58, return ptr
func_000B9C5C  Record*         init 6-word struct with magic, -1, zeros
func_000DD674  u32            init struct with counter ptr, atomically increment counter
func_000DD504  void            two floats to globals, then byte=1
func_000E0BB4  void            word to 0x115C8, byte=1 to 0x1DA23C
func_00099D80  void            two args to consecutive globals at 0xC9F8/0xC9FC
func_00021708  u32            chain two pointers, store first through second, return 1
func_0008073C  u32            nibble < 8 test via XOR/shift/sltu
func_00084DAC  u32            chain two pointers at offsets 0x18/0, store at +0x1C
func_000A99E8  u32            ptr == 0 or ptr == 0x18 via branchless sltiu
func_001A9D68  void            mask word at 0x18 with 0xFFEFFFFF
func_00084E48  u32            branchless equality test: xor + sltiu
func_001235A8  void            swap prev/next of a node, make both point to self
func_00091360  Node*           pop from list: take node at a1, put at a0, advance
func_001A9B78  void            store float at 0x34, set bit 2 on ptr at 0x18
func_001102F0  void            stream advance: sign flag, constant, advance ptr by 8
func_000F8974  u32            array store: base[10] = arg, return 0
func_000F8990  u32            array load: *out = base[18], return 0
func_000E4B0C  u32            array load: *out = base[10], return 0
func_00120C48  void            complex pointer chain: base[ptr0->idx]-1 = arg
func_001AF144  u32            bitmask alignment: (a0+a1-1) & ~(a1-1)
func_001428FC  s32            abs() via branch-likely bltzl
func_001A9C6C  void            copy 3 words to +0x48, set bit 0x100 at 0x18
func_00150730  Vec3*           copy 3 floats, return dest
func_00196B70  Vec4*           copy 4 floats, return dest
func_000FA5C8  void            float: f12-f13+f14 -> 0x3C, f12 -> 0x8(a1)
func_000CDA20  void            clear 3 words at 0x8,0xC,0x10
func_000D55B0  Record*         clear 5 words at 0x0..0x10, return self
func_00194F94  Node*           self->ptr0=self, self->ptr1=self, return self
func_0019BC2C  Node*           self->ptr0=self, self->ptr1=self, self->cleared=0, return self
func_000F8140  u32            store 4 regs to struct, return 0
func_00004FDC  Vec3*           store 0.0f at 0x5864, return self
func_0000537C  u32            set byte 0x28=1, return 1
func_0000BEDC  u32            store 0.0f via $t0, return 1
func_0005E66C  u32            set byte 0x2A0=1, return 1
func_0005E758  u32            set byte 0x144=1, return 1
func_0005E6FC  u32            check word at 0x140 != 0
func_0001FA3C  Vec3*           store 0 byte + 0.0f at 0x57FC/0x57F4, return self
func_0001FCA8  Vec3*           store 0.0f at 0x18C and 0x200, return self
func_0002F438  u8*            store byte to global 0x1D3624, return addr
func_00036D90  u32            return global word at 0x1D3828
func_00049A84  u32            return global word at 0x0743A0
func_00054510  u32            load half-word at 0x4, mask to 16 bits
func_00058FEC  u32            load word at 0xC, increment, return
func_0006B6B8  void*           return global addr 0x0E2240
func_0006BEE4  void*           return global addr 0x0E2260
func_00080D40  u8             return global byte at 0x1D4930
func_00080D4C  u32            store byte to global 0x1D4930, return 0
func_00080E14  u8             return global byte at 0x1D4931
func_00082180  void*           return global addr 0x0E2340
func_00085424  char*           return string addr "default_category"
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

### Branches work: `noreturn` plus a scoped `noreorder`

`func_000E8EA8` is the tent-weight routine
`arg1 + (arg0 - 128) * (128 - abs(arg1 - 128)) / 128`, 24 instructions, and it is
**the first function here with a branch that is not trivial**.  It matched on the
first attempt, which settles the question the work list had open.

Two things make it work:

* **`__attribute__((noreturn))` on the function.**  The body is one asm block that
  ends with `jr $ra`; without `noreturn`, GCC appends a second return after the
  block.  It is a lie about control flow - the function does return - but it only
  affects codegen, and `__builtin_unreachable()` at the end keeps the C
  well-formed.  This is the one piece of machinery in the whole project that is
  ugly on purpose, and it is what buys control of both the branch and the slot.
* **`.set noreorder` scoped to exactly the block**, ending with `.set reorder`.
  Any wider and GCC loses the delay slot; any narrower and the assembler inserts a
  hazard `nop`.

The branch itself is the *easy* kind, and this function is a clean illustration of
what makes it easy: `bgez` with `sra $a2, $a2, 16` in its delay slot - which **both**
paths need - and the three instructions it skips are only the `negu` and its
sign-extension for the negative case.  Both paths rejoin at the `ori` one
instruction later.  There is exactly one place they could meet and they meet there.

What is still untested is the *other* kind: a loop, or a conditional where one side
is more than a few instructions, because then the compiler has to choose block
order and there is nothing left to pin.

### `srl` as a branch-free sign test

`func_000E8EA8` divides by 128 with rounding towards zero and does it in three
instructions with no second branch:

```
sra  $a2, $a0, 7      >> 7, which rounds towards -infinity
srl  $a2, $a2, 25     the sign, which sat in bit 31, lands in bit 6: 0 or 1
addu $a0, $a0, $a2    adding 1 only when negative rounds the other way
sra  $a0, $a0, 7
```

So `a / 128` and `(a + (a < 0)) / 128` are the same value, written without a
comparison.  Worth knowing because it is the shape C compilers reach for when they
are told to keep division by a power of two, and it means "did the source divide,
or did it add a bias first" is not answerable from the assembly.

### Loops work, and the rule that makes the frame possible

Three loops are byte-exact, and together they settle the last category of control
flow.  `tools/find_loops.py` finds them: **1,740 of the 7,497 functions contain a
backward branch**, and it lists them smallest first, because size is the best
predictor of which ones will fall to the same three fixes.

| function | shape | kind |
| --- | --- | --- |
| `func_001428E4` | `for(;;) register(1);` | unconditional |
| `func_000CD5B0` | `for (i = 0; i < 1; i++)` | runs once, not unrolled |
| `func_0009C9B4` | `n = 12; while (--n) *p++ = 0.0f;` | float loop, countdown |

**The rule that unlocks the frame: do not list `$sp` or `$ra` in the clobber
list.**  `func_001428E4` sets up its own 0x20-byte frame in the asm.  With `$sp`
and `$ra` declared as clobbered, GCC emitted a prologue of its own *first* - it
allocated an 0x8 frame, saved `$fp` and `$ra`, and moved `$fp` - and the block's
frame became the second one, with 8 words too many instructions.  Listing neither
means GCC has nothing to preserve, emits no prologue, and the block's frame is the
only one.  This is safe precisely because the function never returns: no code runs
after the block that could observe `$sp`.

**The pointer advance goes in the branch's delay slot in both counted loops**, so
the cursor ends one element *past* the block - `self + 52` after twelve floats -
rather than on the last element written.  Nothing reads it afterwards; it is there
so the same cursor would be positioned for a following pass.  `.set noreorder` is
what holds it there: left on, the assembler considers the slot for its own
scheduling, declines, and inserts a `nop`.

**`slti $a3, $a2, 1` against a literal 1** is why `func_000CD5B0`'s one-iteration
loop was not unrolled.  A countdown from a variable (`bgtz`, in
`func_0009C9B4`) needs no third register, because the condition is the value; a
comparison needs one, and it gets `$a3`.

### The landmine: GCC does not count an assembler-filled delay slot

This cost an iteration and nearly shipped, so it is recorded in full.

Left to emit its own return, GCC writes `jr $ra`; the assembler then appends a
`nop` to fill the delay slot **after the function's symbol size has been fixed**.
The object measures 0x20 and the symbol says 0x1c:

```
$ psp-nm -S build/eboot.elf | grep func_0009C260
0009c260 0000001c A func_0009C260      <- symbol: 28
$ psp-objdump -h build/.../func_0009C260.o
  0 .text         00000020              <- code: 32
```

**One such function is harmless.**  The linker pads to the next symbol's address
and the bytes come out right, which is why adding `func_0009C260` alone still gave
a byte-identical image.  **Two of them cost eight bytes of `.text`**, and every
section after them shifts: the module image went from 100 % to 55 % identical, with
no error anywhere in the build output.

The fix is to write the return inside the asm block so GCC counts the `nop`:

```c
__attribute__((noreturn))
f32 func_0009C260(Node *self) {
    ...
    __asm__ __volatile__(
        ".set noreorder\n\t"
        ... 
        "2:\n\t"
        "jr    $ra\n\t"      /* GCC must see these two to size the symbol */
        "nop\n\t"
        ".set reorder\n\t"
        : ...);
    __builtin_unreachable();
}
```

`noreturn` is doing double duty here: it stops GCC appending a return *and* it is
what lets the `nop` be counted.

**The mistake was weakening `verify_c.py` first.**  It had reported `size 28 != 32`
for these two, correctly, and I read that as a measurement convention and relaxed
it to accept a shortfall when the missing bytes were all `nop`s.  That relaxation
is wrong, and the guard it duplicated is not a convention at all - it is the thing
standing between this bug and a broken image.  `tools/check_symbols.py` now checks
it directly, before the image comparison, so the failure is reported as the one
function that caused it instead of as half a million differing bytes.  Its negative
test is in the commit: breaking `func_0009C260` on purpose makes it report
`recorded 0x20, symbol 0x18`.

### The packed flag bytes are four-state properties, not booleans

The work list had this as "the one-bit accessors over them will name the individual
booleans".  That framing was wrong.  `tools/flag_table.py` decodes the whole array,
and **20 functions** reference `sym_001E1B98`, not the two that were known.

The array is 128 bytes at `0x001E1B98`, each entry packing several independent
properties into one byte, **indexed by a value in a 128-wide domain taken from a
data stream**.  It is almost entirely runs:

```
[0x00]      1  0x00   -
[0x01..09]  9  0x20   bit 5
[0x0a..0e]  5  0x28   bits 3,5
[0x0f..20] 18  0x20   bit 5
[0x21]      1  0x88   bits 3,7
[0x22..30] 15  0x10   bit 4
[0x31..3a] 10  0x04   bit 2
[0x3b..41]  7  0x10   bit 4
[0x42..47]  6  0x41   bits 0,6
[0x48..5b] 20  0x01   bit 0
[0x5c..61]  6  0x10   bit 4
[0x62..67]  6  0x42   bits 1,6
[0x68..7b] 20  0x02   bit 1
[0x7c..7f]  4  0x10   bit 4
```

**What the index is remains open, and one plausible reading has been tested and
rejected.**  `func_001434C0` walks a byte stream, looks up `flags[base + c]` for each
byte and continues while bit 3 is set - and it separately compares that same byte
against `0x2B` and `0x2D`.  That looks exactly like a character-class table driving a
number parser, with `0x2B` and `0x2D` being `+` and `-`.

It is not.  `flag_table.py --domain` checks it in both possible alignments and both
fail: **no digit has bit 3 set.**  So `0x2B` and `0x2D` are values in the index
domain rather than `+` and `-`, and the domain is a 128-value token or enum space.

That also means the "one entry per object class" phrasing this section used is a
guess with nothing behind it, and has been withdrawn.  What is established is the
shape of the table and the regularity below; what each index means is open.

**That table was wrong, and it took two mistakes to get there.**  The search that
produced it matched only `andi $v0, $v0, mask`, so it missed `func_00143838`, which
masks with `0x01` in `$t0`.  Widening the register pattern then found the opposite
problem: nearly every function appeared to query all eight bits, because
`andi $reg, $reg, 0xFF` is how a byte is widened and has nothing to do with the
table.

The fix is to require the register a mask reads from to be one a byte load has
written, and to treat `0xFF` as uninformative because it keeps every bit.  With
that, all twenty functions are accounted for and the bit census is
(`flag_table.py --asked`):

| bit | asked by | who |
| --- | --- | --- |
| 0 | 9 | `0010C90C`, `0010CFC0`, `00121CAC`, `0012CC4C`, `00140A58`, `001434C0`, `00143838`, `00144050`, `00149724` |
| 1 | 6 | `0010CA08`, `0010CFC0`, `00121CAC`, `00140A58`, `001434C0`, `00149724` |
| 2 | 12 | `0010CFC0`, `0010DC0C`, `0010E1CC`, `0010E3A0`, `0010E6F4`, `00121324`, `00121824`, `00121CAC`, `00140A58`, `00140A74`, `001434C0`, `00149724` |
| 3 | 6 | `00109040`, `0010CFC0`, `00116434`, `00121CAC`, `001434C0`, `00149724` |
| 4 | 1 | `0010CFC0` |
| 5 | 2 | `0010CFC0`, `00120E4C` |
| 6 | 1 | `0010CFC0` |
| 7 | **0** | nothing |

**`func_0010CFC0` is the interesting one.**  It masks with `0x01`, `0x02`, `0x03`,
`0x20`, `0x04`, `0x10`, `0x08`, `0x07` and `0x44` - it is the only function that
touches bits 4 and 6, and the only one that touches six of the seven.  Nineteen
masks in one body is not a set of property tests; that is a **serialiser or debug
dump walking the table field by field**.

Which has a consequence worth stating plainly: **bits 4, 5 and 6 are not read by
anyone who is using the value.**  Bit 5 has one other reader, `func_00120E4C`.  Bits 4
and 6 have none.  So they are set by the table and emitted by the dumper, and
nothing in the shipped game consults them - which is itself a fact about the build,
probably a set of flags reserved and then never finished.

And bit 7 is the one nothing asks for at all: set on exactly one entry, index 33,
and read by nobody.

**And `flags[i] & 0x07` only ever produces 0, 1, 2 or 4.**  Not 3, not 5, 6 or 7 -
because bits 0 and 1 are never both set anywhere in the table, and bit 2 never
combines with either.  So `func_00140A58` returns a **four-state property**, not a
boolean.  The distribution:

| state | classes | indices |
| --- | --- | --- |
| 0 | 66 | the first half of the table |
| 1 | 26 | 66..91 |
| 2 | 26 | 98..123 |
| 4 | 10 | 49..58 |

Two things follow from the shape of that distribution.  **States 1 and 2 occupy two
contiguous 26-entry blocks exactly 0x20 apart**, mirrored, and in each the *first
six* additionally carry bit 6 - so the bit-6 set is a property of a specific group of
six entries layered on top of a property of twenty-six.  Two independent facts about
whatever the index describes, encoded in one byte each.

And **bits 4, 5, 6 and 7 are set on 32, 32, 12 and 1 entries respectively**, which
made them look unread.  The census above now says what is actually true: bit 5 has
one reader beyond the dumper (`func_00120E4C`), bits 4 and 6 have none, and bit 7
has none at all - it is set on exactly one entry, index 33, and read by nobody.

Two corrections to my own reading along the way.  Counting the `0x01` run by hand
gave nineteen entries when the script says twenty, and counting the `0x20` run gave
27 when it is 32 - because `0x28` carries bit 5 as well and I had counted the runs
without decoding the bits.  The script exists so that this does not have to be done
by eye - and the mask census needed fixing twice for the same reason.

### The PSP import table: 223 empty stubs and a table nobody can read yet

`.rodata.sceNid` has been on the work list as "read it for the import list and name
the PSP API stubs".  Read.  The finding replaces an assumption:

**Every stub is empty.**  All 223 of them, across 26 `.sceStub.text.*` sections,
are exactly:

```
0x001b0128: jr    $ra
0x001b012c: nop
```

`tools/nid_table.py` checks all 223 rather than trusting the pattern.  That is not a
stripped-out call and not a dead import - it is a PSPLINK module *before loading*.
The loader matches each entry of `.rodata.sceNid` against the NIDs of the loaded
libraries and patches the corresponding stub with the real address, so in the file
on disc each one is a placeholder that returns immediately.

So the stub **code** carries nothing, and the only thing the sections give is the
library names, which are already in the section names.  What is genuinely new is the
correspondence, which the tool records: stub index, address, library, and the NID
words.  26 libraries, biggest first:

| library | stubs | library | stubs |
| --- | --- | --- | --- |
| ThreadManForUser | 29 | UtilsForUser | 7 |
| sceMpeg | 27 | SysMemUserForUser | 7 |
| sceSasCore | 19 | ModuleMgrForUser | 7 |
| sceUtility | 16 | sceNetAdhocMatching | 9 |
| IoFileMgrForUser | 13 | sceNetAdhocctl | 9 |
| sceAtrac3plus | 11 | sceAudio | 9 |
| sceGe_user | 10 | sceLibFont | 9 |

**The entry size of `.rodata.sceNid` is unresolved and the tool says so.**  223
stubs, 892 bytes, 223 32-bit words - so one word per stub divides exactly, while a
PSP NID is 64 bits and would give 111 entries plus four bytes over.  The obvious
test does not settle it: PSP NIDs have their top bit set, and neither reading
satisfies that (103 of 223, or 53 of 111 - about half either way, so the
assumption about the top bit is wrong, not the layout).

Naming the individual imports needs an external NID-to-name table.  This project
does not have one, and filling in 223 names from memory is exactly the kind of
confident guess that has no place here.  `nid_table.py --words 2` reads the table as
64-bit NIDs for whoever wants to match one.

### CodeWarrior hoists the first loop test above the loop's own setup

`func_0014402C` is a `strlen`.  What is odd about it is the order:

```
lb   $a2, 0($a0)      the first character
beqz $a2, done         if NUL, stop ...
move $a1, $a0          ... otherwise remember where we started
addiu $a0, $a0, 1
loop: ...
done: subu $v0, $a0, $a1
```

**The guard is before the instruction that initialises the value the return
subtracts.**  On the empty-string path `$t1` is still whatever the caller passed in,
so the result is not zero and `strlen("")` would return nonsense.

The same shape appears identically in `func_00143A18`, which is a different function
in a different part of the binary.  So this is a **compiler habit, not a
coincidence**: CodeWarrior peels the first iteration out of the loop, emits its test
before the loop's prologue, and does not notice that the prologue also sets up the
result.  The reading that fits is that the caller never passes an empty string, so
the broken path is dead - which is checkable, not guessable: both callers found for
`func_00143A18` pass a non-empty literal, one of them a `.rodata` symbol.

Worth recording as a hazard when reading any loop here: **the peeled test is not
part of the loop, so a guard that appears to guard the loop's initialisation is not
guarding it.**

### Two branch targets in the `.s` files are not where they look

Chasing `func_00143A18` cost an iteration, and the reason is worth writing down.
`asm/eboot/*.s` places branch targets as `.Leboot_XXXXXXXX` labels, and reading the
label's position in the file is not the same as reading the target.  Two of the
five branches in that function target addresses the label placement makes look
elsewhere:

| branch | `.s` label sits at | real target |
| --- | --- | --- |
| `beql $a2, 0` | after the next two instructions | `addiu $a0, $a0, 1` |
| `beqz $t0` | after the `move $t1` | `jr $ra` |

With the wrong targets the trace said the cursor advanced by two per iteration,
which is impossible for a string scan.  With the real ones it advances by one.
`tools/disasm_range.py` disassembles on the fly and prints the displacement, and
the label name is the address in hex, so both agree - **trust `disasm_range.py` over
the `.s` file when a branch target matters.**

### Ending the asm block early steers the C return into the delay slot

Every function so far ends its asm block one instruction short so GCC will schedule
the final store into `jr $ra`'s delay slot.  `func_000F8F88` shows the same trick
works for the *return value* rather than a store, which is the opposite case: the
block ends before the `move $v0, $zero`, leaving it as the only candidate.

```c
__asm__ __volatile__(
    "ori %[one], $zero, 1\n\t"      /* the flag value */
    "sw  %[one], 0x4(%[self])\n\t"   /* the flag store */
    "sw  %[val], 0x0(%[self])\n\t"   /* the value store */
    : [one] "=&r"(one)
    : [self] "r"(self), [val] "r"(value)
    : "memory");

return 0;                            /* move $v0, $zero -> the delay slot */
```

Written as plain C, GCC emits the `move $v0, $zero` *first*, because `$v0` is not
live across either store and so there is nothing making it wait.  So the rule
generalises to: **GCC hoists anything it can prove is not live across the block, so
the way to keep an instruction late is to make it the block's only remaining
candidate for the slot.**

### Strength reduction: no single recipe, just the fewest shifts

Three functions now carry a hand-strength-reduced multiply, and the constants are
derived three different ways:

| function | constant | derivation |
| --- | --- | --- |
| `func_0009D658` | 92 | `(32i - i) * 4 - 32i` = `31*4 - 32` |
| `func_0009D5BC` | 92 | the same |
| `func_00097B54` | 168 | `(64i - 8i) + 2*(64i - 8i)` = `3 * 56` |

Both take four instructions and neither uses `mult`.  92 came out of a subtraction
because 31 is one below a power of two; 168 came out of a doubling because 56
divides by three.  There is no general formula to apply - the compiler simply
picked whichever of shifts and adds was shortest for the number in front of it, so
the multiplication has to be transcribed per function rather than generated.

**`func_00049BC4` is the counter-example that settles it.**  Its stride is 140264,
which falls out of no combination of shifts and adds, and there it uses a real
`mult` + `mflo`.  So the rule is not "always strength-reduce" - it is applied when
it happens to be shorter and skipped when it does not, and which of the two cases
you are in is not visible until you look at the constant.  A helper that generated
the multiply would have to decide that per function too.

### A 32-bit read assembled byte by byte, and why

`func_000E9BB8` reads four bytes as a little-endian `u32` with four `lbu` and three
shifts, where one `lw` would do.  The reason is not slowness but two constraints a
`lw` cannot satisfy: it assumes the host's byte order, and it traps on unaligned
addresses.  A file-format engine reading data other tools wrote cannot rely on
either.

It is also four loads and three shifts **on a CPU that is itself little-endian**,
where this sequence and a `lw` produce identical results.  So the source is
probably not "read a u32" at all - it is assembling a value from parts, and the
engine's file readers are byte-oriented by design.  Worth following up: if the same
routine appears with the shifts the other way round, that is a big-endian twin and
the pair would name the file format's byte order.

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
* **Two hard registers need two variables even when the value is related.**
  `"+r"(base), "=&r"(base)` for `$a0` and `$v0` gives "invalid hard register usage
  between output operands" - the constraint is on the *register*, not the value.
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

`tools/c_candidates.py` ranks every function not yet in `config/matched_c.txt` by how
likely they are to be reproducible, weighting floats and branches against.

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

The rule of thumb from the three hundred and thirty that work: if the function has no
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

**And then `func_0002D630` wasted three build cycles re-deriving it.**  The recipe
was in this section and the failure mode it warns about is exactly the one it is
about, and it happened anyway: the store was made to go through a *separate*
operand (`[d] "=&r"(desc)` for the value, `[n] "r"(node)` for the pointer), so
GCC had no reason to think the block wrote `$a0` at all, and the return copy
floated to the top of the function.  Making the pointer an in-out operand -
`"+&r"` - fixed it, and that is the same rule as `"+r"`, not a new one.

Two things worth separating out of that, because both cost an attempt:

* **Putting the whole body in the asm does not work.**  With `jr $ra` and the
  delay-slot copy written out, the block ends on an instruction that is not a `nop`
  and the symbol comes out 28 bytes against the original's 20.  This is the same
  landmine as [GCC does not count an assembler-filled delay
  slot](#the-landmine-gcc-does-not-count-an-assembler-filled-delay-slot): the
  compiler has nothing after the block to attribute the last slot to.
* **Binding the result to a hard `$v0` after the block does not work either** - if
  the goal is to *move* it there.  The dependency that creates is on `node`, and the
  block claims not to read `node`, so the copy is still free to move above it.

### Reading `$v0` out of the block instead of putting the receiver in it

`func_0012C2D0` is the case the recipe above does not cover: the `move $v0, $a0`
is in the **middle** of the body, at instruction 10 of 14, because the return value
does not depend on any of the stores.  So the copy cannot be left to C at all - it
would be emitted either at the top or in the delay slot, and the original has it in
the middle - and it has to be written into the asm.

Which then leaves a second copy problem.  Once the asm writes `$v0`, returning
`node` from C makes GCC emit a *second* `move $v0, $a0`, and the built function is
four bytes too long with the extra copy sitting before the last two stores.  The
fix is the "the value is already there" form:

```c
register Obj *ret asm("$v0");   /* uninitialised: the block put it there */
return ret;
```

An uninitialised hard register used as a return value produces no instruction at
all, which is exactly right here.  The rule already recorded - never initialise a
hard register you did not compute - reads oddly until it is used for this, and
then it is the only way to write "do not touch this".

The same trick is what made `func_0007E024` work.  Its final store is in the delay
slot and the value it stores has to come from `$f12`, which the block loaded.  Left
to C the load goes to `$f0` and costs an extra register, because the compiler cannot
see the block's four writes to `$f12`.  Loading in the block and reading an
uninitialised `register float last asm("$f12")` in C is what pins both halves to
the same register - two instructions, one register, and the block has to end on the
load for the size to come out right.

### `setjmp` and `longjmp`, found by asking which registers are callee-saved

`func_00140AC4` stores twenty-three values into a structure and returns 0:

```
sw  $s0 .. $s7        0x00 .. 0x1C
sw  $sp, $fp, $ra     0x20, 0x24, 0x28
swc1 $f20 .. $f31     0x2C .. 0x58
```

That is exactly the set of registers the o32 ABI says a **callee** must preserve, and
nothing else - no `$a0`-`$a3`, no `$t0`-`$t9`, no `$v0`/`$v1`, no `$f0`-`$f19`, which
are precisely the ones it may clobber.  A function that saves a list of registers is
usually saving registers it happens to need; one that saves *that* list is saving a
machine context.  `func_00140B28` restores the same 0x5C bytes and is the other half:
it ends with `bnez $a1` then `addiu $a1, $zero, 1`, so it substitutes 1 for a zero
argument - the rule that `longjmp` never returns 0.

**So the module uses `setjmp`/`longjmp` for non-local exit.**  That is worth more than
the two functions, because it tells us what to expect elsewhere: functions with
unusual exit paths may not have unusual control flow at all, they may be setting up a
jump.  `find_loops.py --size 120` is the place to look for the callers.

`$gp` is not saved, which most MIPS `setjmp` implementations also skip: under o32 it
is a fixed module-wide register rather than something a call can change.

The two functions have one caller each - `func_001129E0` for the save and
`func_0011296C` for the restore - so there is exactly one save site in the module.
That bounds it: this is a single guarded operation somewhere, not a pervasive
mechanism.

### Only three of 7,500 labels are not function entry points

`tools/entry_points.py` asks one question of every `glabel`: **does the first
instruction read a register the caller need not have preserved?**  In o32 `$t0`-`$t9`
are caller-saved, so a function that reads one before writing it is reading whatever
was left over - which means either the label is not an entry point or it is one half
of a tail call.

The answer is **3 out of 7,500**, which is the reassuring result: spimdisasm's
function splitting is sound almost everywhere, and an o32 signature can be assumed
for the rest.  The three are `func_00101260`, `func_00140AC4` and `func_0014EAAC`, and
each turned out to be a real `jal` target - so they are genuine functions that take
their real argument in `$a0` and merely *also* touch a register the ABI leaves
undefined.  `func_00140AC4` is `setjmp`: it has to save `$s0`-`$s7` because those are
the caller's values, so it reads them without ever having written them.  That is not
an ABI violation, it is the one legitimate case.

The tool is worth keeping for the opposite reason: it is the cheap test for "this label
is not what its name says", and the first version of it was wrong - it only read the
second comma-separated operand, so it saw no register inside `0x8($t0)` at all and
reported zero for everything.  The addressing-mode registers are the ones it is
looking for.

### `func_00143A18` is `strstr`, and the open question about it is closed

This one was left undone for several iterations on the grounds that nobody could say
what the source was, and writing C would have been transcription rather than
decompilation.  With the branch targets recomputed it reads as an ordinary
hand-rolled substring search, and it is now transcribed byte-exactly.

The thing that made it look wrong was a misread of where the outer loop begins.
`move $a3, $a1` appears **once before the loop and once inside the back edge's delay
slot**, and the `bnez` targets the instruction *after* that `move`.  Reading the `.s`
file's labels puts the loop head one instruction earlier than it is, which makes the
haystack cursor appear to advance twice per iteration and land on every second byte.
It advances once.  That is the same failure as
[the branch targets in the `.s` files](#two-branch-targets-in-the-s-files-are-not-where-they-look),
seen from the other end.

What the function actually is, and why it is worth having:

* **It is `strstr` written out.**  The compiler would emit a call to a library routine,
  so this is the source having been written by hand - which is why it needs two loops
  instead of one.
* **An empty needle loops forever.**  `beql $a2, $zero, check` nullifies its delay
  slot, so the increment is skipped; `check` reloads the same character and branches
  back, finds the same empty needle, and repeats.  A real `strstr` returns the
  haystack.
* **A failed search returns `strlen(haystack)`, not -1.**  The outer loop stops with
  the cursor on the terminator, and the return is that offset.  So a caller
  distinguishes "found at n" from "not found" against a length it already has.

Neither edge case is a transcription error; both are properties of the original.  The
`beql` is also why the loop cannot be written in C at all: GCC will not produce a
branch-likely from a plain `if`, and the empty-needle behaviour needs exactly that
"skip the increment" shape.  So the loop is written out with `.set noreorder` scoped
per branch, and the final `subu` is left to C so it lands in the return's delay slot.

### `func_0018DDAC` is not an unlink; it is a constructor missing its identity

`func_00046CE4` writes four words:

```
ori   $a2, $zero, 0x5        sw $a2, 0x0($a0)     0x00 = 5
lui   $a2, 0x1F / addiu -0x6AB8   sw $a2, 0x8($a0)  0x08 = &0x1E9548
addiu $a3, $zero, -0x2       sw $a3, 0x4($a0)     0x04 = -2
sw   $a1, 0xC($a0)                                0x0C = the argument
```

and [`func_0018DDAC`](#link-is-not-a-ring-buffer) writes **the same two values to the
same two offsets** - `-2` at 0x4 and `&0x1E9548` at 0x8 - and nothing else.

**So `func_0018DDAC` is not a removal.**  It is the same two initialisations without
the identity: putting the node back into the unlinked state without changing what kind
of node it is or who owns it.  A constructor that writes `{-2, &global}` and a reset
that writes `{-2, &global}` cannot be constructing and removing, and having both is
what settles it - the earlier reading of `func_0018DDAC` as "unlink a node" was an
inference from the shape of a list with no pair to check it against.

**Keep the tag's width straight.**  This node is `{ word tag, s32 state, void *link,
void *owner }` with the tag a full word at 0x0.  `func_0014C958`'s node had a `sb` tag
at 0x4.  Two similar-looking four-word structures, and the difference is the width of
the tag and which offset it sits at.

### 782 of 7,500 functions are byte-identical to a sibling

`tools/duplicate_bodies.py` hashes every function body straight out of the
instruction words spimdisasm recorded, groups them, and reports the groups.  The 223
PSP import stubs are excluded because all of them are `jr $ra` and their names were
lost in the original link, so their sameness measures the linker, not the programmer.

**107 distinct bodies are shared by more than one function, covering 782 functions -
10.4 % of the module.**

| group size | groups | functions |
| --- | --- | --- |
| 2 | 66 | 132 |
| 3 | 13 | 39 |
| 4 | 7 | 28 |
| 5 - 8 | 10 | 63 |
| 9 - 19 | 4 | 55 |
| 30 - 36 | 4 | 131 |
| 64 | 1 | 64 |
| 110 | 1 | 110 |
| 162 | 1 | 162 |

**What this changes is the amount of distinct work, not the amount of work.**  107
groups stand between the reader and 782 functions: transcribing one member of each
group byte-exactly settles the rest, because they are the same bytes.  That is a real
reduction in what has to be worked out by hand, and it is also a trap - 782 functions
that "just copy an existing one" is not the same claim as 782 functions understood,
and only 15 of them were in C before this iteration.

### 516 functions are one instruction wide, and the module has an accessor layer

A body of exactly 8 bytes is `jr $ra` plus one more instruction, so that second
instruction is the whole of the function.  **516 functions - 6.9 % of the module - are
of that size**, and the census by what they do is more informative than the total:

**272 of the 516 - 162 empty plus 110 `return 0` - are the module's base-class
answers.**  `jr $ra; nop` is a `void` method with nothing to do; `jr $ra; or $v0,$zero,
$zero` is a predicate whose answer is no.  Both are the same thing: a hierarchy where the
base implementation of almost every method does nothing or says no, and the real work
lives in the few hundred functions that override them.  Neither number means much alone;
together they describe the module's shape.

| | count | what it is |
| --- | --- | --- |
| `jr $ra; nop` | 162 | an empty body, `void` |
| `jr $ra; or $v0,$zero,$zero` | 110 | `return 0` |
| `jr $ra; move $v0,$a0` | 50 | returns its argument unchanged |
| `jr $ra; ori $v0,$zero,1` | 36 | `return 1` |
| `jr $ra; lw $v0, k($a0)` | ~60 | one-field getter, one per offset |
| `jr $ra; sw $a1, k($a0)` | ~15 | one-field setter, one per offset |
| `jr $ra; mtc1 $zero, $f0` | 6 | `return 0.0f` |
| the rest | ~77 | one each |

**358 of the 516 return a constant or their own argument.**  Nothing else happens.  A
module of this size is mostly plumbing, and the plumbing is accessors: 110 functions
whose whole content is `return 0` is what a class hierarchy of default answers looks
like - a virtual method whose base implementation says no - and 162 empty bodies is the
same idea for `void` methods with nothing to do.

**The constants form a ladder.**  There are single-instruction functions returning 1,
2, 3, 4, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC and 0x10, one apiece, and then 0x20, 0x4000 and
0x8000.  A run of consecutive values each returned by its own function is an
enumeration being projected onto integers, one accessor per enumerator.  It is the
same shape as the getter table below it, where each distinct offset also gets its own
function.

**And there is exactly one function in the module that is not a return**: 8 bytes
reading `j func_00081DB0` with a `nop`, a tail call.  One in 7,497.

### The 64 functions that return a byte they wrote to the stack

The largest group after the empty ones is 64 functions sharing this 20-byte body:

```
addiu $sp, $sp, -0x10
sb    $zero, 0x0($sp)
lw    $v0, 0x0($sp)
jr    $ra
addiu $sp, $sp, 0x10
```

**It stores one zero *byte* and then loads a *word* from the same address.**  Only the
low byte was written, so the three bytes above it are whatever was already on the stack,
and the function returns them as part of its result.

**How the callers use it: 155 of 155 keep one byte, and that is measured.**
`tools/return_width.py` follows `$v0` from every in-module branch to this group - 155
branches to the 64 functions - and reports what each caller does with it.  The answer is
uniform: **every one stores the word to a stack slot and reloads it with `lb`.**  None
uses all four bytes, and none ignores it.

```
jal   func_170654
sb    $a1, 0x39B($sp)     the delay slot: an argument, unrelated
move  $a0, $s0
sw    $v0, 0x3A4($sp)     the result, stored wide
...
lb    $a1, 0x3A4($sp)     ... and read back one byte at a time
```

**That uniformity is what makes the wide load safe, and it changes the reading.**  The
obvious guess - one function compiled carelessly, with a caller that happens to read a
byte - is wrong, because the *callee* does exactly what the caller does: write one byte,
read four.  This is **a convention of the codebase, not a defect in one function**.  A
one-byte-wide value is stored and reloaded as a word throughout, and every reader takes
only the byte it wants.  The three uninitialised bytes are dead by convention, which is
why 64 copies of the shape survive in a shipped build and not one of them misbehaves.

**So the source was probably not returning a `char` at all.**  A `char` would have been
widened with `lb` or `lbu`, not `lw`.  What fits is a one-byte field the compiler kept
in memory and widened with a word load because it had no reason to narrow.  That remains
a reading rather than a conclusion; the measurement is not in doubt.

#### The tool was wrong three times before it was right

Every version produced a confident, plausible, false count, which is exactly the failure
mode a census is meant to rule out.

| version | what it looked at | what it reported | why it was wrong |
| --- | --- | --- | --- |
| 1 | the instruction after the `jal` | 144 sites narrow to a byte | that instruction is the **branch's own delay slot**, storing an unrelated argument |
| 2 | the instructions that read `$v0` | 142 sites discard the result | the value is not in a register: it is `sw $v0, 0x2C($sp)` then `lb $a0, 0x2C($sp)` |
| 3 | `$v0` **and** the memory it is spilled to | **155 of 155 keep one byte** | — |
| 4 | the same, over *every* duplicated body | five of the largest groups "discard the result" | **the callee never writes `$v0`**, so there was no return value to discard |

**Version 1 manufactured the very evidence the tool existed to check.**  Taken at face
value it would have "confirmed" the hypothesis with the wrong number for the wrong
reason.  The fix that mattered was following the value through *memory* as well as
through registers, which is the one place these call sites hide.

**Version 4 is the worse one, because it looked like a result.**  Run over all 107
groups, it reported that the 32-function 704-byte group and the 30-function 408-byte
group each "discard the result" - 32 and 30 facts about callers, which read as a finding
about call sites and were in fact a statement about `void` functions that never touch
the return register.  A census that reports what it does not know is worse than one that
reports less, because the number looks like an answer.

The tool now says so explicitly per group:

```
704 x32   32 discards the result  [callee never writes $v0 - no return value]
```

and it no longer warns about `beqz` on a group whose callee returns a whole word.  That
distinction is not pedantry: a `beqz` on a fully-written zero is safe, and a `beqz` on a
value with three garbage bytes above the low one is not.  Warning about both is warning
about neither.

The tool prints example call sites alongside its counts for the same reason throughout:
**a bare count looks identical whether it was measured or guessed**, and these four runs
showed it can be both.

### What the width census says about all 107 groups

Running the question over every duplicated body at once separates the groups that return
something from the groups that do not, and the split is stark:

| group | body | what the callers do |
| --- | --- | --- |
| 8 x162 | `jr; nop` | no return value - the empty bodies |
| 704 x32, 408 x30, 96 x19, 104 x11 | real code | no return value - `void` |
| **20 x64** | `sb`/`lw` one byte | **155 of 155 reload one byte** |
| 8 x110 | `return 0` | 17 use four bytes, 9 `beqz`, 12 ignore |
| 8 x36 | `return 1` | 24 `beqz`, 7 ignore |

**The 64-group is the only one in the module where a return value is consumed
byte-narrowly, and it is the only one with the `sb`/`lw` shape.**  That is the thing the
census was for: the narrow-reading convention is real but it is *local*, not a property
of the whole module.  Read as a property of the module it would have been wrong.

### Three singletons: one empty frame, one `break`, one `stub`

`func_00025594` allocates a 0x30-byte frame, saves six registers into it, and returns:

```
addiu $sp, $sp, -0x30
sw    $a2, 0x18($sp)   sw $a3, 0x1C($sp)
sw    $t0, 0x20($sp)   sw $t1, 0x24($sp)
sw    $t2, 0x28($sp)   sw $t3, 0x2C($sp)
jr    $ra
addiu $sp, $sp, 0x30
```

`tools/empty_frames.py` is the census and the answer is **one out of 7,497**.

**What it says is what the function used to do.**  The saved registers are `$a2`, `$a3`
and `$t0` to `$t3` - all caller-saved, which is exactly the set that has to survive
*across a call*.  Saving callee-saved registers is bookkeeping; saving caller-saved ones
is only ever done around a call.  So this function used to call something.

**And it does not save `$ra`, which is the other half of the same evidence.**  A frame
built to hold a return address across a call has to spill it, or the call's return would
overwrite the saved `$a3`.  There is no `sw $ra`.  So the call is gone: inlined to
nothing, with the frame kept because the compiler had already built it.

That is the most likely sequence and it is written down as a sequence, not a conclusion
- an empty body is also what a function whose every statement was a macro expanding to
nothing would look like.  What is not a guess is the register set: these six are saved
because of a call, and there is no call left.

**So the module's three one-off shapes are all the same kind of finding**: the single
`break` of [the previous section](#the-only-break-in-the-module-and-it-is-a-callback-slot),
the single empty frame, and the pair of copies.  Each is one function out of 7,497, and
in each case the singleton is more informative than the count - a shape that appears
once is something specific, not a convention.

### Two globals in two regions, from one function

`func_000E1000` sets `0x1A2238` to 3 and `0x11574` to -1, and the second address is
built as `lui $a1, 0x1` + `sw $a0, 0x1574($a1)` - 0x10000 plus an offset, far below
every other global found so far.

**That is not a mistake, it is the only cheap way there.**  There is no `lui $reg, 0x0`,
so anything below 0x10000 has to be addressed as a `lui` of 1 plus a positive offset.
`func_00097200` reaches `0xC9E4` the same way.  So the low region is not a second
globals page that some other code uses differently - it is the same page, addressed the
only way the ISA allows, which means **the two addresses in this function are more
alike than they look**: both are ordinary globals, and the module simply has data below
0x10000.

That in turn retires something recorded earlier.  The note about "two globals regions"
claimed the lower one was separate because nothing had reached it before; the correct
statement is the reverse - it is reachable in one instruction from zero, so any address
below 0x10000 is cheap and the distinction was never real.

### Two lerps, and why neither of them can be written in C

`func_00197414` and `func_001AF16C` are `out = a + t * (b - a)` over three and four
floats, written out rather than called - the same reason the module has a hand-rolled
`strstr`.  Both matched on the first attempt as whole-body asm blocks, and both are
transcribed that way because **there is no way to produce them any other way**:

* **The spills are most of the function.**  The difference goes to a stack temporary at
  `sp+0xC`, is reloaded a component at a time, multiplied, spilled again, reloaded,
  added.  Eight loads and eight stores where three registers would do - and the reload
  is not a register shortage, `$f13` to `$f18` are all free at that point.
  Written as one expression gcc emits twenty-one instructions; with `volatile` on the
  local, twenty-nine.  **Neither produces the spills.**  `volatile` gets closer only by
  accident, because forcing the first store out incidentally forces the rest, and that
  is a coincidence rather than a control.
* **The frame is not one gcc will build.**  0x20 bytes with the temporaries where the
  original puts them; it chose 0x10 for the same local.  The
  [frame trick](#loops-work-and-the-rule-that-makes-the-frame-possible) - not listing
  `$sp` as clobbered so gcc emits no prologue - is only safe for a function that never
  returns, and these do.

So the body in both files is the machine code and the comment is the decompilation, the
same bargain as `src/eboot/syncSkeleton_27D0.c` and as `func_000103AC` before it.  The
arithmetic is three subtractions, three multiplies and three adds, and any reader can
check it against the disassembly in one pass.

**The fourth component costs a register and moves the temporary.**  With three floats
the difference temp is at `sp+0xC`; with four it is at `sp+0x10`, because it is sixteen
bytes and has to clear the sixteen the products occupy.  The frame is 0x20 either way
but what lives where is not.  And `$f0` appears in the four-component version and not
the three: six loads into `$f13` to `$f19` leave `$f20` alone and the seventh needs
somewhere, and `$f0` is what is free - the o32 argument-save registers, which a callee
is supposed to treat as dead.  Legal either way, and the sort of thing that only shows
up at four components.

**There are exactly two of them.**  A loose filter - three or more of each of `sub.s`,
`mul.s` and `add.s`, and six or more `lwc1` - matches **115** functions, which is not a
family but "float arithmetic is common".  Tightened to the actual shape, equal counts
of all three and `5n` loads and `3n` stores, it matches **two**.  Checking the count
before writing the word down is the lesson from
[the copy census](#a-copy-is-a-scheduler's-answer-and-there-are-only-two-of-them),
applied one iteration later and this time in advance.

**Both files were missing from the tree while this section claimed them.**  The
paragraph above says both "matched on the first attempt" and were "transcribed that
way" - and neither `src/eboot/func_00197414.c` nor `src/eboot/func_001AF16C.c` was
there.  A report entry is not a file, and nothing cross-checked the prose against the
directory.  Both are written and byte-exact now.

They came back out of `tools/c_shapes.py --show`, which lists instruction sequences
rather than function names, so it cannot see that a function has already been written
up in the report - which is also why it was worth transcribing the two *other*
functions that had a shape and looked like more lerps before assuming this pair was
the only pair of its kind.  That near-miss is what the gap guard is for:
`tools/check_report.py` walks the backtick-quoted symbol names out of this file and
the README and lists the ones with no `src/eboot/<name>.c`.  It does not decide
whether a mention was a *claim* - plenty are not, `func_001129E0` and the 223 import
stubs among them - so it prints the line and lets a reader judge.  It reports 31
names with no file out of 97 mentioned, and each of those 31 needs a human to say
which kind it is.

**And a near-collision the guard found rather than a gap.**  `func_0011354C` - an
abort site in the guard's error table - and `func_0001354C` - a 28-byte wrapper this
project transcribed - differ by one leading zero, and the first has 212 bytes against
the second's 28.  Two symbols four orders of magnitude apart in size and one
character apart in name is the kind of thing that gets mixed up silently, because
every tool here prints addresses in hex with a fixed width and the names carry the
same tail.  Worth knowing before either is looked up by eye.

### A copy is a scheduler's answer, and there are only two of them

`func_00103E88` copies six words and the interesting part is how, not what:

```
lw $a2, 0x0($a0)    lw $a3, 0x4($a0)    lw $t0, 0x8($a0)
sw $a2, 0x0($a1)    lw $a2, 0xC($a0)    sw $a3, 0x4($a1)
lw $a3, 0x10($a0)   sw $t0, 0x8($a1)    lw $a0, 0x14($a0)
sw $a2, 0xC($a1)    sw $a3, 0x10($a1)   sw $a0, 0x14($a1)
```

Six values through three registers, and the last one is loaded **twice** - once into
`$a3` and again into `$a0`.  The rotation is a scheduler's answer to a twenty-four byte
copy, not a source-level choice: the source is one assignment.  `func_000EE56C` is the
same thing at sixteen words, in groups of three, with five stores and no load after
them at the tail.

**And that is all of them.**  `tools/copy_family.py` counts every function that is
nothing but a pointer adjustment, loads from it, stores to a second pointer and a
return: **two out of 7,497.**  An earlier note here called it a family, on the strength
of having seen both in the shapes queue, and two is not a family.

The absence is the more useful half of the result.  **This codebase almost never copies
a structure inline.**  It copies through pointers, field by field, or through generated
copy constructors - which is what `-ffunction-sections` leaves lying around and what the
`stub` and unnamed symbols in the vector census are made of.  So the recipe for these
two is worth writing down and there is nothing to generalise from it.

**The duplicated load is what makes them unwriteable in C.**  Two assignments to the
same address, and GCC does one load into `$v0` - it is strictly better and the bytes are
wrong.  So the last three instructions are spelled out, and `lw %[s], 0x14(%[s])` works
because the register is its own base: a pointer being overwritten by the word just past
what it pointed at.  That is the third distinct way a single register has had to be
described in this directory - a hard `register` variable, a register named in the
template because it overlaps an operand, and now a variable whose *type* changes
meaning mid-function.

**And the source offset is inside the structure, not at its start.**  `addiu $a0, $a0,
0xC` makes this a getter shaped like a copy: the receiver is a larger object and the
field's position is baked into pointer arithmetic.  Written as `src->w[0]` with the
offset folded into the struct, GCC emits `lw $v0, 0xC($a0)` and there is no `addiu` at
all - one instruction short, and every offset after it wrong.  `func_000EE56C` does the
same with `0x110`, which means something occupies 0x110 bytes of its receiver before
the field it copies does.

### The only `break` in the module, and it is a callback slot

`func_00130830` is four instructions:

```
break 768
nop
jr    $ra
nop
```

**It is the only function in the module that starts with a `break`** - one of 7,497.
A `break` with a code is how a compiler marks a path it proved unreachable, and 768 is
0x300, inside the software-break range rather than a hardware one.  So the body was
never written.

What makes it more than a curiosity is the single caller, and it does not call it:

```
lui   $a0, %hi(func_00130830)
jal   func_00142910
addiu $a0, $a0, %lo(func_00130830)
```

`func_00130AE8` takes **its address** and hands it to `func_00142910`, and a few
instructions later does the same with `str_exit_FE94` and `func_00130810`.  So
`func_00142910` takes a function pointer and is being fed a sequence of them, and this
trap is one of the values in the sequence: **a placeholder in a callback table**, a slot
that had to exist at a fixed address because something registers a pointer to it, whose
body was never filled in.

`func_00142910` is also four hundred bytes below `func_001429F0`, which is where an
abort falls through when it has no handler at either nesting level.  A registration
function and a last-resort error path as neighbours in the link order is what would be
expected if both belong to the same shutdown machinery - which is a guess, but the
alternative is that they are unrelated functions that happen to link together.

### Who uses the vector unit: 32 functions, and the four TUs they live in

`tools/vector_unit.py` is the census.  **32 of the 7,497 functions use the PSP's
vector coprocessor, 542 vector instructions in all** - about 0.4 % of the module's
127,000 instructions.  That is small enough to read, so the claim that the renderer and
the skeleton code use it and nothing else does is checkable rather than a first
impression.

And the four translation units that own the vector code are the four that kept their
names in the original link:

| vector instructions | function |
| --- | --- |
| 64 | `renderMeshInstances_122C` |
| 61 | `syncSkeleton_0FDC` |
| 47 | `drawing_0C04` |
| 31, 28, 22, 15 | `renderCommon_01B8`, `_0580`, `_1398`, `_1618` |
| 26, 15, 12, 12 | `syncSkeleton_01EC`, `_21A0`, `_2120`, `_27D0` |

Three of `syncSkeleton_*` and five of `renderCommon_*`, `renderMeshInstances_*`,
`drawing_*`: **rendering and skeletons, and nothing else.**  That is the answer to the
question the two transcribed functions raised, and it is the same answer the survivor
names give for everything else - the parts worth naming are the parts worth naming
because somebody outside the file used them.

**The opcodes say what it is being used for, and it is not matrix multiplication
alone.**  `vrsq.s` appears eight times - reciprocal square root, which is distance
attenuation in a lighting calculation, four vertices at a time.  That is the reason to
want a vector unit at all, and it is only worth having if you are shading a lot per
frame.  `svl.q` and `svr.q`, the swapped load and store, are how the unit transposes,
five of each, and `vmmul.q` appears six times, so matrices are being multiplied and the
transposes are for that.  `vscl.t`, `vmul.t` and `vadd.t` together account for 55 uses:
the `.t` suffix reads through the transpose file, which is what lets one prepared
operand feed several separate operations, as the two transcribed `syncSkeleton`
functions do.

### The vector unit, and the register names gas will and will not take

`syncSkeleton_27D0` and `syncSkeleton_2808` are the first code in the project that
uses the PSP's vector coprocessor, and they are the first files in `src/eboot/` whose
names came from the original link rather than from a placeholder.

**What they are.**  Three quads of four floats is a 3x4 matrix, sixteen bytes per row.
`syncSkeleton_27D0` broadcasts three scalars out of `$vfs0`-`$vfs2` with `vscl.t` and
scales each row; one instruction does four multiplies, so a whole matrix scales in
three.  `syncSkeleton_2808` is the same fourteen instructions with `vmul.t` against
`$vf10`.  **That is skinning**, and it is the clearest statement available of what the
vector unit was put in this console for.

**The register names are spimdisasm's, not gcc's.**  `$vfs0`, `$vf20`, `vfs0` and
`$vfs00` are all rejected as `invalid operands`.  `S100` and `R200` parse - and then
fail only on *class*: `lv.s` wants a single register, `lv.q`/`sv.q`/`vscl.t` want a
quad one.  So the rule is to use the disassembler's spelling and match the class to the
mnemonic.  Finding that took a sweep of nine candidate spellings, which is worth
recording because guessing at one per build cycle is how the previous two attempts
went.

**gcc cannot name the vector file at all** - `"$vfs0"` is rejected as a clobber - so
the blocks declare no clobbers and no operands.  That is only safe because the block is
the entire function.  A function with real code around a vector operation would have no
way to tell the compiler what it touches, which is a structural limit and not a trick.

**And the delay-slot rule inverts under `noreorder`.**  With `reorder` on, gas fills the
return's delay slot by moving the preceding store across the branch, giving 52 bytes.
With `noreorder` it neither fills nor inserts one, so the `nop` has to be written out
explicitly or the symbol comes out 52 bytes the other way.  **Under `reorder` the slot
must not be written; under `noreorder` it must be** - opposite rules, four bytes apart,
and both were confirmed by `try_func.py` reporting `MATCH` at the wrong size before
`check_symbols.py` caught it.

That last point deserves its own note: `try_func.py` printed `MATCH: 0 differing words`
on a function that was four bytes short.  It compares the overlapping region and says
nothing about length.  **`verify_c.py`'s size check is the one that catches this**, and
it must not be relaxed - the same conclusion as the delay-slot landmine, reached from a
new direction.

**Why these two are asm rather than C.**  psp-gcc has no vector types on Allegrex:
there is no `float4` and no operator that lowers to `lv.q` or `vscl.t`, so there is no
C spelling of the function at all.  Unlike the rest of the directory the body here *is*
the machine code, and the comment above it is the decompilation.  That is recorded
rather than disguised, and it is why `syncSkeleton_2808` keeps three `lv.s` loads whose
results nothing reads - dropping them would give the same behaviour and the wrong
bytes, and would hide the fact that the function is not self-contained: `$vf10` holds
the operand and the caller must have left it there.

### `movz` is the answer to most conditionals, and that is the problem

`func_000A9A00` is `field ? 0 : -24`, and psp-gcc will not write that as a branch:

```
addiu $v1, $zero, -0x18
movz  $v0, $v1, $a0
```

`movz` is an Allegrex conditional move - it was added to the ISA precisely so this
idiom would not need a branch - so the disagreement here is **one instruction shorter
than the original**, not a different register choice.  Same family as the `ins` and
`negu` notes above and just as fatal: a four-byte difference is four bytes.

**This cannot be fixed from C.**  `__builtin_expect` only changes the *prediction*, not
the decision to if-convert.  The branch has to be written out, with `.set noreorder`
so the `addiu` stays in the nullified slot, and `bnel` - not `beql`, because `bnel`
deadens the slot on the branch-taken path and that is the path the original takes.

Suppressing GCC's epilogue then needs `noreturn`, which is a lie: the function does
return, but the block already contains the `jr $ra`.  Without the attribute the
function is eight bytes too long.  Same trick as elsewhere in this directory.

**And the delay-slot rule turns out to have a second case.**  Writing the block as

```
jr $ra
nop
```

gives **seven** instructions and a 28-byte function.  Leaving the `nop` out gives the
right six and a 24-byte function with the slot filled by the assembler.  So the
[landmine](#the-landmine-gcc-does-not-count-an-assembler-filled-delay-slot) - an
assembler-filled slot not being counted - does not apply when the block is the last
thing in the function.  It applies when the compiler has an epilogue of its own to
attach the slot to.  Both were verified by `check_symbols.py` and `check_image.py`
rather than reasoned about, and they give different answers.

### The guard: a nested, re-entrant `setjmp` sandbox with a four-value error code

Following the `setjmp`/`longjmp` pair to its two callers closes a mechanism, and it is
the most substantial thing the project has found about how the engine is *shaped*
rather than about any one function.

**The runner, `func_001129E0(self, fn, arg)`:**

```
addiu $sp, $sp, -0x90
sw    $zero, 0x70($sp)          the result, zeroed up front
sw    $a0,  0x74($sp)           self
sw    $a2,  0x78($sp)           fn
sw    $a1,  0x7C($sp)           arg
lw    $a1,  0x50($a0)           the handler currently installed
sw    $a1,  0x10($sp)           ... kept as the record's "previous" field
addiu $a1, $sp, 0x10
sw    $a1,  0x50($a0)           install this frame's handler record
jal   func_140AC4               setjmp
addiu $a0, $sp, 0x14            ... at record + 4
bnel  $v0, $zero, restore       longjmp'd back in?  skip the call
lw    $a0,  0x74($sp)
lw    $a2,  0x7C($sp)
jalr  $a2                       fn(self, arg)
lw    $a1,  0x78($sp)
restore:
lw    $a0,  0x74($sp)
lw    $a1,  0x10($sp)
sw    $a1,  0x50($a0)           put the old handler back
lw    $v0,  0x70($sp)           return the result
```

**Two `addiu`s four bytes apart are what crack it.**  The record installed at
`self->f50` starts at `sp+0x10`, and `setjmp` is handed `sp+0x14` - so the jump context
lives at offset 4 of the handler record, not at its start.  Cross-checked against
`func_0011296C`, which jumps from `&self->f50->f4`: the same 4.

**The result slot is inside the record, and that is not a coincidence.**  The record
starts at `sp+0x10`, the context is 0x5C bytes, so the context ends at `sp+0x70` -
which is the slot that was zeroed at entry and loaded into `$v0` on the way out.
`func_0011296C` does `sw $a0, 0x60($a2)` with `$a2` the record, and `record + 0x60` is
that same slot.  So:

```
record { Guard *prev;      /* +0x00 */
         Jump    ctx;      /* +0x04 .. +0x5F, 0x5C bytes */
         s32     result;   /* +0x60 */ }
```

The abort reason is written *into the return slot of the frame that is being
unwound*, which is why the runner returns it without any other bookkeeping.

**The abort, `func_0011296C(self, code)`:**

```
lw  $a3, 0x50($a0)             the installed handler
beqz $a3, forward              none installed: hand it outward
sw  <code>, 0x60($a2)          record->result = code
lw  $a0, 0x50($a1)
addiu $a0, $a0, 0x4            &record->ctx
jal func_140B28                longjmp
ori $a1, $zero, 0x1            (the longjmp value; the code is already saved)
```

and when nothing is installed it walks `self->f10->f50` and `jalr`s that handler - so
**handlers nest**, and an abort with no handler at this level is forwarded to the
enclosing one.  Only when there is no handler at either level does it fall through to
`func_0011429F0(1)`, which reaches the global object at `0x1E1F8C` - the same object
whose field 0x58 is the random-number state from *Recognised constants* above.  So the
"abort with nowhere to go" path reports through the object that owns the RNG, which is
what a debug or logging facility on the main game object would do.

**The error code is a four-value closed set.**  `tools/abort_codes.py` lists it:

| code | abort sites | who |
| --- | --- | --- |
| 1 | 1 | `func_00112754` |
| 3 | 2 | `func_0011BA2C`, `func_00120F8C` |
| 4 | 1 | `func_00116264` |
| 5 | 4 | `func_00112754`, `func_00112CDC`, `func_00113458`, `func_0011354C` |

Eight abort sites in seven functions, every reason an immediate built in the delay
slot.  **Code 5 has four sites**, which makes it the likely catch-all; codes 3 and 4
have two and one.  Since a normal completion returns 0, an enum over this mechanism
would have five values.

The three callers of the runner - `func_001137A8`, `func_001139C8`, `func_0011A22C` -
all do `bnez $v0, <error label>` straight after.  That is the whole point of the code:
**a call that cannot fail in-band gets its failure out-of-band instead.**  A function
pointer can be anything, so the only way to report a failure from one is to jump.

**Why `func_001129E0` is not transcribed.**  Its body is ordinary C - install, setjmp,
call, restore, return - and the mechanism above *is* the decompilation.  What does not
give is the 0x90-byte frame with spills at 0x70, 0x74, 0x78, 0x7C and `$ra` at 0x80.
The [frame recipe](#loops-work-and-the-rule-that-makes-the-frame-possible) only works
for a function that never returns, because its trick is to stop GCC emitting a
prologue of its own; this one returns.  Matching those spill slots means writing the
whole frame in asm, which is transcription.  So it is documented and left alone, which
is the same line drawn for `func_00143A18` before it was understood - except that this
one *is* understood, and what is missing is a byte-level detail rather than a meaning.

### Two globals regions, and a third that may be a counter

**This section was wrong and is replaced by the note on `func_000E1000` above.**

`func_00097200` writes to `0xC9E4`, built as `lui 0x1` + `addiu -0x361C`.  At the time
this was the only function found reaching below the module's data page - the others go
to `0x19Exxxx` and `0x1Exxxx` - and the conclusion drawn was that the module has at
least **two globals regions**.

It does not.  `func_000E1000` writes to `0x11574` as `lui 0x1` + `sw 0x1574`, and
there is no `lui $reg, 0x0` on this ISA, so anything below `0x10000` *has* to be
addressed as a `lui` of 1 plus a positive offset.  One instruction, no penalty.  So the
low addresses are the same page reached the only way the ISA allows, and calling them a
second region was reading a constraint as a convention.

The useful thing the original observation got right is the negative one: the module's
globals are spread over a wide range and the tool that finds setters has to handle both
spellings.  That much stands.

### A tag byte beside the pointer, not inside it

`func_0014C958` builds a node of `{ pointer, byte 4, pointer }`.  The byte sits
between the two pointers and is a constant, so it is a type tag stored *next to* a
pointer rather than packed into one.  The alternative - a tagged pointer, where the
low bits of the address carry the type - is ruled out by the arithmetic rather than
by taste: no address here is masked, shifted or tested, so the address is used whole.
Storing the tag beside it costs five bytes instead of four and needs no masking
anywhere, which is a trade a codebase makes for convenience more often than for
space.

The tag being a fixed 4 rather than a parameter says this constructor makes exactly
one kind of node.  There will be a family of these with different tags, and 4 is the
fifth in whatever numbering the source used - which is the kind of thing that would
be settled by counting the siblings, not by reading this one.

The same function also corrected a reading: the block it fills is **not** all zeros.
Eight of the nine words are `$zero` and the word at 0x10 is the third argument.
Reading the run of `sw $zero` as "clear the block" misses a parameter entirely.  The
tell was in the tooling - the shape string lists that store as `sw9` against the
zeros' `swgt0`, and a plain store where a conditional store was expected is worth
looking at.  Worth remembering generally: **`swgt` in a shape string is a guess by
the classifier, and where it is wrong it is because the store is not conditional.**

### When a register has to be named in the template and cannot be an operand

`func_000D3490` holds the second argument in `$a1` for one instruction and then
overwrites it with the counter.  So the two values must share a register, and GCC
refuses: *"invalid hard register usage between earlyclobber operand and input
operand"* - the counter needs `"=&r"`, and an earlyclobber may not overlap an input.

The way out is the same one already used for `$zero` and `$f12`: **name the register
in the template text** and keep it out of the constraint list.  The argument is then
documented by a comment instead of by an operand, which is a fair trade for a
register that is live for exactly one instruction.

This is the second distinct case where a hard register cannot be an operand - the
first was a `register` variable that also needed to be one - and they have different
fixes.  Overlap needs the template; the double-role case needs two variables.  Worth
keeping the two apart, because they look the same from the error message.

### Recognised constants, and the pseudo-ones to leave alone

`func_00143408` is `state = state * 1103515245 + 12345` in thirteen instructions,
with the mask applied *after* the store.  0x41C64E6D and 0x3039 are the two
constants from the C standard's own `rand()` example, so this is a linear
congruential generator and not a hash that happens to use them.

Two details in it are worth writing down because the plausible-looking alternative
is wrong:

* **Only `mflo` is taken**, so the recurrence is modulo 2^32 and the state keeps all
  32 bits.  The mask is on the *return*, not on the stored state, so the period is
  2^32.  The standard's version does `(state >> 16) & 0x7FFFFFFF` instead and keeps a
  masked state; that is a different generator and would not produce these bytes.
* **`mult`, not `multu`.**  Same low word either way, so it is the same result - but
  it says the source treated the state as signed.  Second function in the repository
  where a `mult` survives because the constant resists strength reduction.

Getting it to match took four attempts, and two of them are worth recording because
they look like progress:

* `return (s32)(value & 0x7FFFFFFF)` became `ext $v0, %[v], 0, 31`.  Changing the
  return type to `u32` did **not** help - GCC recognises "clear the sign bit" in
  both forms.
* Laundering the constant through an empty asm (`__asm__("" : "+r"(mask))`) stopped
  the `ext` but made GCC rebuild the mask with `lui` + `ori`, four bytes too long.
* What worked is reading the mask out of `$v0`, where the block had already built it,
  as an uninitialised `register u32 mask asm("$v0")`.  The block's own
  `addiu $v0, %[v], -0x1` is the mask, so the constant is free, the register is
  right, and the `and` is the only spelling left.

The general lesson is that **hiding a constant from the compiler and reusing one it
already computed are different tools with different costs.**  The first costs two
instructions; the second costs none, and it is available whenever the original built
the constant in a register it still holds.

Two of the same batch are the opposite case - a value that looks like an
identification and is not.  Four of the constants in `func_00102A54` end in `3F8000`
and read at a glance as `1.0f`, `-1.0f`, `0.25f` and `-255.0f`.  **They are not
floats**: their exponent fields are 0xFA, 0x00, 0x06 and 0x0C, and only a value near
0x7F800000 has a normalised exponent.  Recognising a constant by its hex shape is
the same failure as naming a function by its address, and it took a decode to catch.

### `slt` versus `sltu` is visible in the bytes

`func_00100DC0` ends in `slt $v0, $v0, $a0`, and with both operands `u32` GCC emits
`sltu`.  It is not a cosmetic difference: the left operand is a global loaded
unchanged while the right is `limit - (second - first)`, which is negative exactly
when the function is about to return true.  The unsigned form would answer the
opposite question.

The same function is the clearest example so far of **an arithmetic detour that has
to be transcribed rather than simplified**.  Its last four instructions compute
`limit - (second - first)` and ask whether `limit` is less than that, which - while
nothing overflows - is exactly `first > second`.  Written as `first > second` in C
the function is one instruction instead of four, correct, and nowhere near the
original bytes.  So the C says `bound < delta`, and the identity is a note about the
code rather than a replacement for it.

### A field that overlaps a word: `func_0012C2D0`

Not a matching lesson but a layout finding worth recording, because it is the kind
of thing that cannot be written as a struct:

```
sw    $a3, 0x10($a0)    the id, a full word at 0x10
sb    $a2, 0x13($a0)    a flag, one byte at 0x13 - inside that word
```

0x13 is the **top byte of the id word**, so the two stores write the same four bytes
twice, word first and byte second.  No layout puts a `u32` at 0x10 and a `u8` at
0x13 side by side, so `func_0012C2D0.c` writes the flag by offset
(`((u8 *)node)[0x13]`) and stops the struct short of it.  The order matters: the
byte goes second, so the flag is the field that wins and the id is the word it sits
on top of.  An earlier version of that file put a three-byte pad between them and
GCC emitted `sb $a2, 0x17` - a reminder that `u32` at 0x10 already occupies 0x11 to
0x13, so padding *after* it starts at 0x14, not 0x11.

So: the recipe is the recipe, and it is worth reading before transcribing rather
than after.  spimdisasm writes this instruction as `or $v0, $a0, $zero`, not as
`move`, so the group `delay_slots.py` reports as `or $v0, $a0, $zero` is 99 strong
and already includes it - there is no separate `move` census to keep.

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

### A `noreorder` region that spans a label grows the function by four bytes

`func_001A9CF8` is the first function here with more than two labels in one asm
block, and it produced 96 bytes where the original is 92 - with the last four
bytes being a `nop` *past the symbol's own size*, since `.size` correctly said
0x5C.  Everything in the block was right; the assembler had added a word.

The cause is how the `.set noreorder` regions line up against the labels.  This
is a two-instruction region:

```asm
        .set noreorder
        4:
        jr     $ra
        nop
        .set reorder
```

and this one produces the same instructions plus a trailing `nop`:

```asm
        .set noreorder          # opened several instructions earlier
        beqz   $a1, 5f
        nop
        .set reorder
        2:
        ori    $a0, $zero, 0x1
        5:
        andi   $v0, $a0, 0xFF
        4:                     # a label, with noreorder still open across it
        jr     $ra
        nop
        .set reorder
```

Reopening `.set noreorder` immediately before the last label fixes it and leaves
the instruction stream identical, so the cost of getting this wrong is invisible
in a diff - it shows up only as a size mismatch, which is why
`verify_c.py`'s length check is not optional.

Two things are worth recording beyond the fix.  The failure needed *both* more
than one branch and a label between them, which is why 378 functions did not hit
it: every other function in the tree has a single branch or a single label.
And the symptom is a word outside the symbol, so a tool that compared only the
symbol's bytes would have reported a match - `try_func.py` did report `MATCH: 0
differing words` on this function while `verify_c.py` correctly called it
`size 96 != 92`.

### The accessor layer: a flags word at 0x18 and a sentinel float at 0x1C

`tools/flag_accessors.py` is a census of the C++ boolean accessors, written
because five of them turned up while transcribing a pair of vector-copy helpers.
The first attempt matched too loosely and reported nonsense - grouping a
*clear* of a sixteen-bit range with a *set* of the single bit just above it,
because "lowest set bit of the mask" is not a grouping key when the mask is a
complement.  It now matches the exact instruction sequence and groups by
(offset, mask, direction), and it reports:

```
    offset          mask     bits   kind  functions
      0x64    0x80000000       31    get  func_00012F3C
      0x18       0x20000       17    get  func_001A9ACC
      0x18      0x100000       20    set  func_001A9D54
      0x18      0x100000       20  clear  func_001A9D68
      0x18      0x100000       20    get  func_001A9D80
      0x18       0x40000       18    set  func_001A9D94
```

Nine functions in all, of which six are plain and three also write the float at
0x1C.  Three findings, in order of how much they are worth:

**Bit 20 is the only one whose accessor family survived intact** - getter, setter
and clearer, twenty bytes apart at 0x1A9D54, 0x1A9D68 and 0x1A9D80.  Bit 17 kept
only its getter and bit 18 only its setter.  That asymmetry is about the link,
not about the class: an accessor that was inlined at every call site leaves only
the out-of-line copies, and which direction that is depends on how each was
spelled at its call sites.

**Two of the three masks need `lui` and one does not,** which is the only
difference in instruction count between them and explains why the pair at
0x1A9ACC/0x1A9D80 is five instructions and not four.  Bits 18 and 20 do not fit a
signed 16-bit immediate, so they take a shift-by-sixteen first; bit 7 in
`func_001A9C40` does, so it takes an `ori`.  The mask is not always the same
shape, and the width of the flag says which.

**`func_001A9CBC` reads `$f12` without writing it,** and that is the
interesting one.  On this ABI the first floating-point argument arrives in
`$f12`, so the function takes a float and assigns it to the field at 0x1C -
the `lui`/`mtc1` pair that materialises the -1.0f sentinel in its two neighbours
is simply absent, and the caller's value is stored instead.  Nothing in the
instruction stream says "parameter"; it says "a float register the code did not
set", and only the ABI turns that into a signature.

The three combined accessors set and clear *different* bits - `func_001A9C98`
sets bit 12 and clears bit 13, `func_001A9CD4` does the reverse - which is what a
pair of boolean members initialised to opposite values looks like, and it is why
the query beside them (`func_001A9CF8`) is a three-way test rather than a
single-bit test: bit 13 alone is one state, a real float is another, bit 12 alone
the third, and each of the four reachable combinations is one of the four
accessors.

### The fixed-point blend at 0xE8EA8, and a rounding idiom that will not compile

`func_000E8EA8` came out of a size check rather than the work queue: the symbol
recorded 0x60 bytes and the file claimed 0x14, so the C was a transcription of
the wrong function - of the five instructions the disassembly tool had shown for
a `lw`/`sra`/`lhu` that is not there at all.  The real 24 instructions are a
fixed-point blend:

```
    f + (v - 128) * (128 - |f - 128|) / 128
```

with `f` in `$a1` and `v` in `$a0`, both narrowed to signed 16 bits by an
`sll`/`sra` pair, and the result narrowed the same way.  `0x80` appears three
times, as the centre both inputs are measured from, as the ceiling of the
distance, and as the scale of the division - a value centred on 128 and
quantised in eighths.

The division is the part that cannot be written in C.  `sra 7` then `srl 25` is a
32-bit shift in two pieces, and doing the second half *logically* is what keeps
the sign: the result is all ones when the product is negative and zero when it is
positive, truncated to seven bits.  Added before the `sra 7`, that is the standard
signed division by 128 that rounds towards zero instead of towards negative
infinity.  Written as one `sra 32` it does not assemble; written as `/ 128` in C
psp-gcc produces something else entirely, and `-1 / 128` is 0 in C anyway, which
is the rounding the original deliberately does *not* want for `-128`.

The first `sll`/`sra` pair is a no-op on an already-correct argument and is kept
because it is in the original.  That is worth saying plainly, because it is the
kind of instruction a decompilation "should" remove and this project's whole
premise is that it should not.

### A cross product at 0xEBC88, and why it is not on the vector unit

`func_000EBC88` computes the cross product of the `f32[3]` at offset 0x30 of its
first argument and the `f32[3]` at the start of its second, into the vector its
third argument points at.  All six products are computed and three of them are
dead, which is what a cross product written straight from the algebra looks like:

```
    x = a.y*b.z - a.z*b.y      $f16 = $f12*$f13 - $f14*$f15
    y = a.z*b.x - a.x*b.z      $f13 = $f14*$f17 - $f19*$f13
    z = a.x*b.y - a.y*b.x      $f12 = $f19*$f15 - $f12*$f17
```

It is worth having as a second data point for `tools/vector_unit.py`'s claim that the
vector coprocessor is confined to rendering.  A cross product is exactly what a
vector unit exists to do, it sits in the physics-adjacent part of the module rather
than the rendering part, and it is six scalar `mul.s` and three scalar `sub.s` with
`$f12` to `$f19` - the eight scalar FP temporaries of the o32 ABI - as destinations.
All six products are live at once, so every one of the six destinations is distinct,
and from the `sub.s` on the same registers are reused for the three results.

The shape is also the only one in `tools/c_shapes.py --done` with 20 instructions of
float arithmetic and no branch, which is how it came to be looked at.

### `collision_1210`: two volumes tested in one call, sign-only, no multiply

`collision_1210` at 0x1B1A30 is 192 bytes with sixteen `lh`s, twelve `subu`s, ten
`and`s and not one multiply or branch but the return.  Written out as arithmetic,
with the first argument's twelve signed halfwords as `a[0..11]` and the second's
eight as `b[0..7]`:

```
    s1 = (a[0]-b[4]) & (a[1]-b[5]) & (a[2]-b[6])
       & (b[0]-a[6]) & (b[1]-a[7]) & (b[2]-a[8])

    s2 = (a[3]-b[4]) & (a[4]-b[5]) & (a[5]-b[6])
       & (b[0]-a[9]) & (b[1]-a[10]) & (b[2]-a[11])

    return ((s1 >> 31) & 1) | ((s2 >> 31) & 2)
```

Each `s1` is the sign of a product of six differences, three taken one way and three
the other, so its sign bit is the parity of six comparisons - which is what an
orientation or interval-overlap decision needs, obtained without multiplying.  The
answer is two bits: which of two volumes overlapped the one in the second argument.
`b[3]` is never read, and `a2 = b + 8` is what makes `b[4..6]` reachable as a group.

**The caller is what settles "two volumes".**  There is exactly one, `collision_0C18`,
and it calls with `$a0 = this` and `$a1` from the stack, then does

```
    lbu   $s1, 0x1C($s4)
    jal   collision_1210        ; with `move $a0, $s4` in the delay slot
    or    $a0, $s1, $v0
    sltiu $a1, $a0, 0x16
    beqz  $a1, ...
    ...
    jr    $at
```

on `byte_at_0x1C | result` - a **22-way jump table**.  A one-bit "collided" answer
would be redundant with a byte-sized field; a two-bit one is not, and the jump table
has room for it.  That is the whole argument, and it is an argument from the call
site rather than from the function.

**What is not settled, and is recorded as open in the file: which end of each
interval is the low one.**  The code asks only for the *sign* of six differences, and
the answer's meaning flips depending on which of `a[0]` and `a[6]` is the minimum.
Nothing in the 192 bytes says, and settling it means reading a 752-byte caller.

### Naming the register rule that took a function to get wrong

`func_00140B28` is the `longjmp` half of the pair with `func_00140AC4`, and it took
one attempt and one discovery:

> **A callee-saved register in the clobber list makes GCC *save* it.**  The obvious
> transcription of a function that restores `$s0`-`$s7` and `$f20`-`$f31` lists them
> as clobbered.  psp-gcc then emits a prologue that spills `$f31` down to `$f24`
> before the block, and the function comes out 56 bytes long with a frame it does
> not have.  Declaring them clobbered tells GCC the block destroys them - it does -
> but GCC's own convention is that a *callee* preserves them, so it preserves them
> around the call site it thinks the block is.
>
> The fix is the omission `func_00140AC4` already makes: clobber `memory` and nothing
> else.  A callee-saved register a whole-body block overwrites needs no declaration
> when nothing follows the block to read it, which is what `noreturn` plus
> `__builtin_unreachable()` guarantee.

It is the mirror image of the `$sp` rule, and worth having side by side.  `$sp` is
not callee-saved and listing it makes GCC emit a frame; `$s0` is callee-saved and
listing it makes GCC emit a *save*.  Both push a prologue in front of a block that is
supposed to have none.

`$ra` and `$sp` cannot be left to C here either, which is the one place this function
differs from its `setjmp` half: the `jr $ra` has to branch to the address *loaded
from offset 0x28*, not to the one the function was called with, so the block ends
with its own `jr $ra`.

### Nine live functions write into the code section

`func_000F93E8` computes `0x0EB850 + 28 * index` and stores to it.  The stride is
`index << 5 - index << 2`, which is 28, and the base is a bare `lui 0xF` /
`addiu -0x47B0` pair with R_MIPS_HI16 and R_MIPS_LO16 relocations on both halves
against the same target.  Nothing about that is odd.  What the address *is* is.

**It is not data.**  0x0EB850 is sixteen bytes into `func_000EB840`'s prologue - the
instruction there is `move $s1, $a1` - and the words that follow carry two
R_MIPS_26 relocations at 0x0EB85C and 0x0EB884, which is to say two `jal`s.  A
record at index 1 would land on `sw $ra, 0x34($sp)` and index 4 exactly on
`func_000EB8A4`'s entry point, so the structure cannot be one.

**Nine functions do it.**  `tools/stride_table.py` is the census, and it reports:

```
0xeb850: 9 functions materialise it; verdict code.

  func_000F924C func_000F9274 func_000F92A0 func_000F92F0 func_000F93E8
  func_000F9414 func_000F9438 func_000F9544 func_000F9590
```

All nine index or walk it with stride 28, and `func_000F924C` zeroes 32 records of
it in a loop with `addiu $p, $p, 0x1C` in the branch's delay slot.

**And this one is reached.**  There is a real `jal func_0F93E8` at 0x0EAAA8, its
delay slot fed by `lbu $a0, 0x7E($a0)` - a byte out of the object, 0 to 255, with
nothing masking it.

So the module contains nine live functions writing into its own code section at an
address derived from a byte field a caller supplies.  **That is recorded, not
resolved.**  Three readings fit the bytes and cannot be told apart from inside any
one of these functions: the accessors are unreachable in practice; the structure was
optimised away and its storage reused; or the original link laid a data object over
code.  The last is the least likely.  The first is not provable either.  What *is*
provable is the arithmetic, and that is what `src/eboot/func_000F93E8.c` transcribes.

### The census behind that, and what it turned up instead

`tools/stride_table.py` asks a narrower question than the one that found the cluster:
**which functions materialise the same address**, and is that address code or data.
It reports 5,357 function uses over 1,203 addresses that two or more functions build.

Two things about how it got there are worth recording, because the first is the same
mistake this project has already made twice.

* **Filtering on the instruction *shape* does not work.**  The first version matched
  a shift pair, or an `addiu $p, $p, k` sitting in a delay slot, and found **1,231
  functions over 819 addresses**.  That is "addressing is common", not a family -
  the same error as `tools/flag_table.py`, and the lesson is again that the *filter*
  was wrong rather than the count being too low.
* **Most of what it finds is not an address.**  A `lui`/`addiu` pair whose result
  lands inside no section is a 32-bit constant.  Deciding that per group is what
  makes the tool's largest entries legible, and they are the module's
  four-character chunk tags:

  ```
  0x66727573  137 uses  "surf"
  0x64687367  108 uses  "gshd"
  0x65766177  107 uses  "wave"
  0x20626d78  105 uses  " xml"
  0x20686d78  105 uses  " hmx"
  0x20736d78  105 uses  " smx"
  0x61746473  105 uses  "std"
  ```

  That is an independent confirmation of `tools/tags.py` from a different direction.
  The tags are not in a table; they are immediates, and this says which ones the
  module leans on most - `surf` a hundred and thirty-seven times over.

### `func_000E8F08`: the other half of the fixed-point layer

`func_000E8EA8` - written up above - blends a value against a fraction centred on
128 and scaled by 128.  Sixty bytes after it, `func_000E8F08` multiplies three signed
16-bit fixed-point values together:

```
    return (s16)((u32)((u32)(s16)a * (s32)(s16)b * (s32)(s16)c) >> 23);
```

Two `mult`/`mflo` pairs, and **each multiply truncates to 32 bits** - the second sees
the low word of the first, not the full 64-bit product.  That is why it cannot be one
C expression: `(u32)a * (u32)b * (u32)c` promotes back to a wider type and gives a
different answer, and only `(u32)a * (u32)b` keeps the truncation in the source.

The shift is 23, which is 32 - 9, so a value of about 4096 stands for 1.0: the low
word of a Q12 product has to come down by nine bits to put the binary point back.
And there are **two `nop`s** between the first `mflo` and the second `mult` - the
multiply latency being spent narrowing the third argument rather than waiting.  Under
`.set noreorder` they have to be written out; under `.set reorder` the assembler
hoists the `sll`/`sra` pair into the slot and the instruction order changes.

Together the two are the module's fixed-point layer, and they are why this project
reads the `sll 16 / sra 16` idiom in this corner of the binary as "make it a `short`"
rather than as dead code.

### A packer with a channel that is dead for every input

`func_000706A8` reduces two channels of a packed word to five bits each, puts the
upper one at bit 5 and a constant 0x8000 at the top:

```
    return 0x8000 | ((a0 >> 3) & 0x1F) | (((a0 >> 11) & 0x1F) << 5);
```

0x8000 in bit 15 is how the PSP's video formats carry "there is an alpha bit here",
so this is a packer for a two-channel-plus-flag format.

**And one pair of its instructions is dead, provably.**  `andi $a1, $a0, 0xFF` leaves
eight bits, `srl $a1, $a1, 19` shifts those eight bits clean out of the word, and
`sll $a1, $a1, 10` shifts zero back.  The result is used; it is always zero.

That is worth a paragraph of its own because **it is a different kind of dead code
from every other instance found so far.**  The redundant `sll`/`sra` in
`func_000E8EA8` and the three unused products in `func_000EBC88`'s cross product all
compute values that are *used* - a compiler proved a redundancy and left the
arithmetic in place.  This one computes a value that is used and is always zero: the
compiler chose the mask 0xFF where the source's own mask must have reached bit 19,
and narrowed the channel out of existence.  The third channel, bits 16 and up,
therefore never reaches the output.

It is the clearest instance so far of "byte-exact" and "correct" being different
questions, which is the distinction this project keeps running into.

### Two functions that are one expression, factored

`func_00049B68` computes `a * 0x23E8 + 0x743A8`.  `func_00049BEC`, thirty-six bytes
further on, computes `a * 0x23E8 + b * 0x80F8 + 0x2108`.  The relation is exact:
`0x743A8 + 0x2100 == 0x2108`, so the second is the first plus a second term, and its
four-instruction tail is the first inlined with the difference left over.

Neither constant is a power of two and neither divides 0x800, so this is not an array
index with a stride.  **What it indexes is not settled here.**  There are three
callers: `func_00049C30` adds 0x800 to the result before returning, and `func_0006882C`
passes it to `func_143730` alongside a pointer loaded from `0x0BF1C1C` and the
constant 0x800.  So the result is an index and 0x800 goes with it - but nothing in
seventeen instructions says what the index is an index *into*, and that is the honest
end of it.

Both truncate their products to 32 bits (`mult` then `mflo`), which is why neither can
be one C expression: `(u32)a * 0x23E8` promotes back to a wider type on the second
multiply and gives a different answer.

### A static constructor that writes 480 by 272, four times

`func_0014EBA4` is 268 bytes of stores and a three-instruction loop, and it is worth
more for what it writes than for how it does it.

It fills **four identical 0xFC-byte records** at 0x6C770, 0x6C86C, 0x6C968 and
0x6CA64, and this is what is non-zero in each:

```
    +0x00  4        +0x50  0xFFFF
    +0x10  100      +0x58  1
    +0x18  1        +0xF8  -1
    +0x1C  480
    +0x20  272
```

**480 by 272 is the PSP's framebuffer**, and that is arithmetic rather than a guess:
0x1E0 and 0x110 appear in the body and nowhere else, in fields one word apart, and
again side by side in the 0x48-byte header at 0x6C728.  What the engine *calls* these
records is not established - display modes, framebuffer descriptors and texture
formats all fit, and four identical entries are as consistent with "four slots,
differentiated later" as with "four the same".  The 4 at +0x00 and the 0xFFFF at
+0x50 are the kind of sentinel that says "not set yet", which is the one hint the
bytes give.

**The record stride is 0xFC and that is settled by the loop, not assumed.**  Two
cursors step in parallel, `$a1` at the record starts and `$a2` 0xF8 behind them, so
`$a2`'s store lands on the *last* word of the same record rather than the first word
of the next.  A stride of 0xF8 would have been the other reading; the `+0xFC` on
both settles it.

Two scheduling details worth keeping:

* **The constants are hoisted out of the loop into `$t5` through `$t0`,** which is
  why the loop body contains no `ori` or `addiu` pair at all - twenty-eight stores and
  nothing else.  A C loop would rematerialise each constant unless GCC proved the
  loop-invariant hoist itself, which it does; writing them out is what makes that
  visible.
* **The loop label goes before the counter decrement, not after it.**  The branch
  targets the `addiu $v1, $v1, -1`, so the decrement is the first instruction of the
  body and runs four times.  Putting the label after it - the obvious arrangement -
  runs the body five times and shifts the branch by one instruction.  That was the one
  wrong word in this function's first attempt.

### The code-section writers are a lot more numerous than one function

The 0x0EB850 cluster above turned out to be the small end of something, and
`tools/code_writers.py` is the census of it: **every function that materialises an
address with `lui`/`addiu`, lands inside `.text`, and then loads or stores through
it.**

Three clusters are hand-checked - the containing function was disassembled to its own
`jr $ra`, so "inside the body" is a measurement rather than an inference from the
size map:

```
  base       uses  lands in              called
  0xe9728      12  func_000E9680+0xa8   12/12
  0xe97a8       9  func_000E9780+0x28    9/9
  0xeb850       3  func_000EB840+0x10    3/3
```

**Every function in all three is referenced by an R_MIPS_26 relocation**, so none of
them is dead code - which removes the reading that would have made this uninteresting.
The 0xE9 cluster's writes are index-bounded (`slti $a1, $a0, 0x20` in
`func_000E7614`), so they land in a 128-byte window that straddles the end of
`func_000E9680` and the start of `func_000E9780`.

**The unfiltered count is 1,024 functions at 455 bases, and that number is an upper
bound, not a finding.**  It is what the test gives when "lands inside a nominal
function body" is inferred from splat's `nonmatching` sizes, which are "distance to
the next label".  Where the module keeps a data block after a function, the block is
attributed to that function and every address in it is reported as landing inside
code.  That is the size map's limitation showing through, and the three clusters
above are the ones where disassembly settled it the other way.

**So there are two separate things and they are not yet joined.**  Plenty of this
module's writable data plainly does live inside `.text` - `func_000E5524` toggles a
one-bit flag at 0x5196C and remembers the old value at 0x51968, which is a pair of
ordinary globals in the middle of the code section.  And three addresses inside
genuinely live code are written by two dozen live functions.  Both statements are
verifiable from the bytes.  Whether they are the same phenomenon or two, is not
established, and this file does not say it is.

### A toggle that returns nothing

`func_000E5524` flips a one-bit flag and remembers what it was:

```
    previous = flag; old = previous; flag = (previous + 1) & 1;
```

over the two adjacent words at 0x51968 and 0x5196C.  **Nothing in its twenty bytes
writes `$v0`**, so it is a `void` function and declaring it anything else would be a
guess.  That it records the old value is what makes it `void`: the point is that
0x51968 holds the answer afterwards, and a caller wanting the new one can read
0x5196C just as well.

`(previous + 1) & 1` rather than `previous ^ 1` is worth a line.  The two are the
same for a one-bit value, so the choice is free, and the compiler picked the
increment - which suggests the source spelled the flag as a small integer or a
`++` rather than as a `bool`.  The `addiu` goes to a scratch register so that `$a1`
stays live for the store at 0x51968, which has to happen *before* the flip is written
back.

### Three functions that are mostly about where their data lives

`func_000F9274` and `func_000F9590` are two more of the 0x0EB850 cluster: one does
`flags |= 0x10; field4 = value`, the other `return record[0x14]`.  The first is the
only one of the nine that *reads*, and its two halves are kept apart for as long as
the code can afford - the flags word is read into `$a2` so that `$a1` is free to
receive the second argument and the store at +4 happens before the `ori`.

`func_000E5A24` copies the two adjacent words at 0x599A8 and 0x599AC into two
caller-supplied pointers and returns 1.  **Returning a hard-coded 1 with no condition
anywhere in the twenty bytes** says this is a getter whose failure is not
representable, not an operation that can fail.

`func_000E7F9C` is three stores sharing one index - a halfword array at 0x0E97C8, a
byte on the object at +0x30, and a pointer into a word array at 0x0E9728 - and its
`$a1` is written twice and read once, so it cannot be an asm input at all.  The
parameters are named in the comment and the block clobbers `$a0` through `$a3`
outright, which is the trade `func_0000BEC0` makes.

### Two constructors, and a `lui 0x0` that does nothing

`func_0009674C` sets six fields and returns `this`; `func_00024D04` sets four, three
of them pointers sixteen bytes apart in `.rodata`, and returns `this`.

The first has a **`lui $a1, 0x0` that is dead**: 0x17BC fits a signed 16-bit
immediate, so the `addiu` alone would do, and the `lui` is there because the source's
constant went through the same `%hi`/`%lo` path as the one at +4 - which does need
both halves, since 0x18E80 does not fit.  Written as one `addiu` the function would
be 24 bytes instead of 52.

That is the same observation as the dead channel in `func_000706A8` seen from the
other side: there the *arithmetic* was dead, here the *high half* is, and in both the
compiler had a cheaper spelling available and did not take it.

### Three functions, and a census that says the first two were not the worst

`func_000C3470` is a 4x4 matrix scale - sixteen floats in, multiplied by one factor,
sixteen out - and it is **632 bytes**, thirty-nine instructions per element, for one
`mul.s` each.  Byte-exact on the second attempt; the one wrong word was the last
`sw`, which `.set reorder` hoisted into `jr $ra`'s delay slot.

What it does with the values is the point:

* the four source rows are copied from `$a1` to `sp+0x50`, `sp+0x70`, `sp+0x90` and
  `sp+0xB0` - sixteen `lwc1`, sixteen `swc1`;
* each row is reloaded, multiplied by `$f12`, and spilled to a *second* set at
  `sp+0x40`, `sp+0x60`, `sp+0x80`, `sp+0xA0` - so the unscaled copy is kept and never
  read again;
* the sixteen scaled values are reloaded into `sp+0`..`sp+0x3C`;
* and only then are they loaded a **third** time, this time with `lw`, and stored to
  `$a0`.

**Three passes over the data for one multiply each, where load-multiply-store would
do.**  The frame is 0xC0 bytes: four rows unscaled, four scaled, one output block, all
live at once.  Nothing forces it - `$f13` to `$f17` are free throughout.

**The last sixteen loads use `lw` and the last sixteen stores use `sw`** on values
last written by `swc1`.  That is the clearest evidence in the function that the
compiler was copying rather than computing: a float already in a float register
would be one `swc1`, and the `lw`/`sw` pair exists because the value was last
written on a *different* path through the compiler's data-flow model.

`renderMeshInstances_1060` and `renderMeshInstances_1034` are in the vector-coprocessor
translation unit, so they have no C spelling at all and are machine code for the same
reason as `syncSkeleton_27D0.c`.  The interesting parts:

* **three consecutive `lui 0x2B00`s are dead** in `1060` - each overwritten by an
  `lwr` two instructions later, before anything reads it.  The most concentrated
  instance of the pattern `func_0009674C` shows once.
* **the `lwr` offsets are 1, 5 and 9**, which is how a 4x4 becomes a 4x3: bytes 1-4,
  5-8 and 9-12 of each sixteen-byte row, so component 0 - the homogeneous w - is
  dropped.  `lwr` is the unaligned load, the only way to reach those bytes.
* **`cache 0x18, 0x3C($a1)` before the stores** is a cache hint on the command
  buffer, at the same offset in both functions, so it belongs to the "about to write
  to the command buffer" convention rather than to either operation.
* **`cache`'s sub-opcode field is five bits**, so 0x18 is the whole instruction
  encoding; reading it as a flag or a size would be inventing structure that is not
  there.

**And the census says the two lerps were not the extreme case after all.**
`tools/spill_frame.py` counts every function that moves floats through its frame -
three or more `swc1`, six or more `lwc1`, some floating-point arithmetic - and there
are **236**.  The three this project transcribed sit at 6 stores / 6 loads, 8 / 8 and
48 / 40, while the worst is `func_0013C8A4` at **4 stores and 28 loads across 2,028
bytes**.

That is worth recording as a correction rather than as a discovery.  The write-up on
the lerps calls their spilling extreme, and the count says they are middling: they
were found by the work queue, not by being the worst of anything.  And the ratio is
not the discriminator anyway - `func_000C3470` has *more* stores than loads, so what
makes these three need machine code is not the proportion of stack traffic but the
fact that the compiler spilled where it had registers free, which is a decision
rather than a register shortage.

The tool also had to learn something to count any of this: **the loads are not
against `$sp`.**  `func_000C3470` has forty-eight `swc1`s with `$sp` as the base and
not one `lwc1` with it, because the compiler materialises each frame offset into a
register once - `addiu $a2, $sp, 0x40` - and loads through that.  Counting `$sp`
alone gives 48 and 0.
### Six more byte-exact, and a census that overturned a guess made in a comment

The four functions this iteration added are `func_0009232C`, `updateNodeGraph_0E48`,
`func_0014EAAC` and `func_00080758`, plus `sortAndCullScene_1080` and
`sortAndCullScene_10BC`.  Three of them say something the others do not.

**`updateNodeGraph_0E48` is a fifth member of the four-member copy family, and it is
the odd one out.**  `func_00150988`, `func_00193358`, `func_001965E8` and
`func_00196608` are all `lwc1`/`swc1` pairs copying a three-float vector, all taking
the source pointer straight from `$a0`.  This one has the same eight-instruction shape
but reads `$a0` from offset 8 of `$a0` first and adds `0x1C` to *that* - a member
vector one dereference deeper.  **So the family is five, and only one of the five came
through the link with a name**: `updateNodeGraph` is a surviving CodeWarrior symbol, 28
of them in the module, while the other four are `func_`-prefixed placeholders because
`tools/orig_names.py` finds no name for them.

**The first draft of that comment drew a conclusion from the missing names, and it
was backwards.**  It read the split as a header boundary - "the four that share a
shape share a translation unit, and the one whose header is elsewhere has its own
unit" - and checked it two ways before trusting it.  `asm/eboot/` holds one `.s` per
function, so the build cannot show a unit boundary at all; and silence in a symbol
table is not evidence about a header.  An argument from a missing name to a real fact
has the direction wrong.  What survives the check is only the narrower sentence above.
This is the third time in this project that a plausible claim about *why* code has a
shape failed on contact with how the names were actually obtained.

**`func_0014EAAC` is a byte-stream writer whose mask does nothing.**
`and $t4, $a3, $t9` masks a 32-bit value by 0xFFFFFFFF and returns it unchanged.  The
mask itself is built with `lui $v1, 0xFF` then `ori $t9, $v1, 0xFFFF`, where one
`ori $t9, $zero, -1` would do.

That last sentence nearly became a claim that this was a fourth instance of the
dead-`lui` pattern already recorded for `func_0009674C` and `renderMeshInstances_1060`.
**It is not the same pattern, and the file says so.**  In those two the `lui` is
*discarded* - loaded, then overwritten before anything reads it.  Here the `lui` is
live: it is an operand of the `ori`.  Counting them together would make the discarded
pattern look three times commoner than the two functions it actually appears in, and
"a pattern rather than coincidences" would be an easier sentence to write than the
truth.

**`func_00080758` contains an instruction pair that looks like a cancelled pair and
is not.**  `xori $a2, $a2, 0x8` followed by `addiu $a2, $a2, -0x8` reads as
`x ^ 8 - 8`, which for a nibble is `+1` when bit 3 is clear and `-15` when it is set.
Dropping the `addiu` would give `+8` for half the inputs.  The subject being
incremented is not `$a2` but the *nibble* `(a1 >> 4) & 0xF`, which is put back where
it came from - so it is a per-field counter whose step size depends on its own current
value, and the three-instruction step is what that takes.  Written down because the
instance looks exactly like a mistake, and is not.

### The census that mattered: `tools/branch_load.py`

`sortAndCullScene_10BC` builds a value conditionally with the load in the delay slot
of a branch-likely:

```
addiu $v0, $zero, -0x2
bnel  $a1, $zero, . + 4 + (0x1 << 2)
lw    $v0, 0xEC($a0)
```

A likely branch runs its delay slot only when taken, so this is a conditional load
with a default in one branch and no label.  **The comment I wrote for it first claimed
there was no other function in the module doing this, on the grounds that the next
symbol in the same translation unit is not one.**  That is a true statement about the
neighbour and a worthless one about the module, and the census says:

```
 38,336  beq / bne / beql / bnel in the module
  3,214  the likely forms, 8.4 %
  4,041  loads sit in some branch's delay slot
    616  functions with a load in a *likely* branch's delay slot
     43  of those set the destination to a constant first
```

**43, not 1.**  The 43 are the narrower shape the comment was actually about - a
literal written into the destination immediately before the branch, so the branch is
choosing between the literal and the loaded value - which makes it a
`value or sentinel` accessor.  `func_00052950`, 0x00052950, is one of them and is the
same three instructions with a different default, `ori $a2, $zero, 0x0` against
`sortAndCullScene_10BC`'s `addiu $v0, $zero, -0x2`.

**The 8.4 % is what makes the idiom legible rather than a coincidence.**  The likely
form exists on this ISA to do exactly this - make the delay slot conditional - so if it
were a scheduling accident it would be common.  It is used in 3,214 branches and not
more, which fits an idiom only available when the branch's sole purpose is to guard
its own delay slot.  **What the sentinel means is still not established**: the defaults
include at least `-2` and `0`, and 43 instances of a shape say the shape is
deliberate, not what the literal is for.

**Two things the census had to get right, and one of them was wrong first.**

The opcode tables are cross-checked against rabbitizer's own `getOpcodeName()` and
`isBranchLikely()` - 4 branch opcodes and 7 load opcodes, no mismatches - because a
wrong opcode produces a plausible count rather than an error.

And `_writes_const` was passing its hand-written checks while being wrong about what
they covered: given a `lui` followed by an `addu` rather than a load, it reported
`True`, because it read the register out of whatever word it was handed and never
checked that the word was a load.  **Six hand-written cases now, including "a non-load
word", which is the one that was failing.**  The lesson is the same one the `$sp`
counter in `tools/spill_frame.py` produced earlier: a helper that trusts its caller's
argument class will answer about the wrong thing without complaining, and the only
way to notice is a case that is the wrong class.

### A gas trap worth recording

`sortAndCullScene_10BC` was four bytes too long at first.  The cause is the spelling of
the target:

```
"bnel  $a1, $zero, . + 4 + (0x1 << 2)\n\t"   <- 28 bytes, a plain bne plus a nop
"bnel  $a1, $zero, 1f\n\t"                   <- 24 bytes, bnel
```

**With a computed displacement gas does not recognise the branch as "likely"** and
expands it to a non-likely branch plus an inserted `nop`, which is four bytes and one
extra instruction - and the delay slot then holds the `nop` instead of the load, so the
function is also *wrong*, not merely long.  Writing the target as a local label is what
makes the likely form survive.  This is a fourth member of the family of places where
`.set` and pseudo-instruction spelling change the encoding, after the `jr $ra` delay
slot rules.
### The flag-mask rule, confirmed at its boundary

`func_001A9ABC` is sixteen bytes and tests one bit:

```
lw   $a0, 0x18($a0)
andi $v0, $a0, 0x8000
jr   $ra
sltu $v0, $zero, $v0
```

**`tools/flag_accessors.py` did not find it, and the tool had stated the rule that
explains exactly why it should have.**  Its docstring said the mask is always built
with `lui`, "because these are all bits above 15" - and `0x8000` is bit 15, the last
bit `andi`'s zero-extended immediate can reach.  So the tool had no case at the
boundary, which left the rule consistent with a second explanation the bytes also
allow: that psp-gcc simply prefers `lui` for flag masks.  **Bit 15 picks between those
two explanations and it picks the first.**

The evidence is a pair sixteen bytes apart on the same word:

```
0x001A9ABC  func_001A9ABC  andi $v0, $a0, 0x8000     bit 15
0x001A9ACC  func_001A9ACC  lui  $a1, 0x2 / and       bit 17
```

**Nothing else about the two differs.**  With the fourth shape added the table reads:

    spelling    bits found
    andi        3   15
    lui         17  18  20  20  31

Six accessors before, **eight now**, with the split falling exactly where the rule says
and no accessor at bit 16 in either column.

**Two bugs were sitting in the tool behind that missing shape**, and neither could
have been found by reading the code:

* the fallback that looks for an immediate mask tested opcode `0x0D` (`ori`) and not
  `0x0C` (`andi`) - so it could never see an `andi`-masked accessor, the one
  instruction that defines the shape.  It was dead code that *looked* like it handled
  the immediate case;
* the length filter required five instructions, which is correct only when a `lui` is
  present.  **An `andi`-masked getter has no `lui` and is four**, so the filter rejected
  it even after the first fix.

Both are plausible lines in a plausible branch.  They were found because a function
transcribed by hand - bit 15, the one bit the tool's own rule predicted would need an
immediate - did not appear in the tool built to find it.  **That is the whole argument
for transcribing rather than only mining.**

### The `value or sentinel` count was wrong by more than seven times

Last iteration's census reported **43** functions where a branch-likely chooses between
a literal and a value, and this iteration's two functions are both that idiom with the
delay slot holding something that is not a load:

```
func_000BF49C   ori $v0, $zero, 0x0 ... beql + move $v0, $a0
func_000BAB78   ori $v0, $zero, 0x0 ... bnel + addu $v0, $a1, $a0
```

**A delay slot does not have to hold a load.**  `branch_load.py --all` counts any
register write in one:

```
  616   functions, load in a likely delay slot      (43 with a constant default)
1,173   functions, any register write in one       (311 with a constant default)
```

**So 43 described only the loads, and the idiom is at least seven times commoner than
that.**  The slot holds `move` 295 times, `addu` 111, `andi` 130, `ori` 88 - anything
that writes the register the branch is choosing between.  The idiom is not
"conditionally load", it is **"conditionally materialise a value, defaulting to a
literal"**, and the load-only framing was an artefact of the tool.

**What it means is still not established, but the best-supported reading is an
address.**  `func_0000E04C` guards `addiu $s1, $a0, 0x8` with `bnel $a0, $zero`, so
`s1 = node ? node + 8 : NULL` - "the field at +8, if the link exists".  And
`func_0000EBDC`, 696 bytes, does the whole thing **three times into three different
registers**:

```
ori $fp, $zero, 0x0 ... lw $a2, 0x0($a1) / bnel $a2, $t1 / addu $fp, $a1, $a2
ori $s7, $zero, 0x0 ... lw $a2, 0x0($a1) / bnel $a2, $t1 / addu $s7, $a1, $a2
ori $s5, $zero, 0x0 ... ...                    / ... / ...
```

**Three in one function is what makes it an idiom rather than three coincidences**, and
it is the reason the general reading is now "base plus index" rather than "a constant
the branch proved".

**That in turn corrects a claim in `func_000BAB78.c` itself.**  Its first draft called
the delay slot's addend "the constant the branch has just proved" and treated that as
the whole story.  It is true there and it is the degenerate case: the test is equality
against 1, so the index and the tested constant are the same register and `$a0` fills
both roles.  In `func_0000EBDC` the roles are distinct - `$t1` is what `$a2` is compared
against, `$a2` is what is added - so the idiom is about indexing, not about proving.

### A census that widened downwards, and how the tell worked

The first `--all` run reported **265 functions, fewer than the 616 it was meant to
extend**, with no loads anywhere in the breakdown.  A census that goes *down* when it
is widened is not a surprising result to argue about - it is a broken predicate, and the
number itself said so.

The cause was one line: `_dest` asked rabbitizer's `getDestinationGpr()` for the
destination, **which raises for `lw` because `lw` has no `rd` field at all**, and
`insn.rt` is a `RegGprO32` object rather than an integer, so the obvious fixes both
fail silently.  It now asks rabbitizer only *which field* is the destination and reads
the bits itself.  **The lesson is the same one the `$sp` counter in `spill_frame.py`
produced:** a helper asked about the wrong argument class answers confidently about
the wrong thing, and the only reliable detector was a case that should have been
impossible.

The same class of bug bit `_writes_const`, which had been hardened to demand a load and
therefore *could not be used by `--all` at all*.  The fix was not to remove the guard
but to write a second predicate for the general case - `_default_before`, which walks
back from the branch to the last write of the destination register instead of looking
one instruction back.  **One instruction back is right for a load, whose destination
is also its `rt`, and wrong for everything else:** `func_000BF49C` sets its NULL
default six instructions before its `beql` and the one-instruction rule reports no
default for it.

### Four more functions

`func_00080758` already claimed a "field counter whose step size depends on its own
value".  `func_001160C0` and `func_0010FFF4` are the pair that makes the **reload**
question answerable, and they answer it in opposite directions:

* `func_0010FFF4` loads its cursor `twice` - `lw $a1, 0x8($a0)` at the top and again
  after two stores - because the store went through a pointer the compiler had loaded
  from memory and it cannot prove the write missed the cursor;
* `func_001160C0` stores through `$a0` and then **reuses** `$a0` without reloading,
  because its store is at a *fixed offset* from the same base and no second pointer is
  involved, so non-aliasing is provable.

**So the reload is neither redundancy nor a compiler whim: it is the difference between
a provable and an unprovable aliasing question.**  `func_001160C0` also reads a word at
+0x14 and stores its low byte at +5 with nothing between them, which is a
source-level narrowing and not something the compiler introduced.

`func_00080758`'s dead masks are now four of a kind across this tree - `and` against
0xFFFFFFFF in `func_0014EAAC`, `and` against 0xFFFFFF0F in `func_00080758`, and
`sltiu 1` followed by a dead `andi 0xFF` in `func_000BF49C`, which is the first of the
four where the redundant mask is a *zero*-extending `andi` rather than a full-word
`and`.
### A dead mask in 168 functions, found by transcribing two

`func_000FBFD4` is four arithmetic instructions and twenty-eight bytes:

```
lw    $a1, 0x208($a0)
lw    $a0, 0x20C($a0)
xor   $a0, $a1, $a0
sltiu $a0, $a0, 0x1
andi  $v0, $a0, 0xFF        <- dead
jr    $ra
sltiu $v0, $v0, 0x1
```

**`andi ... 0xFF` after `sltiu ..., 1` cannot change anything**: `sltiu` against an
immediate of one produces exactly 0 or 1.  `func_000BF49C` has the same pair, and the
two are 0xBF apart in address with nothing else in common.

**`tools/boolean_shapes.py` counts it: 168 functions.**  The examples the tool shows
are the same shape throughout - `xor`, `sltiu 1`, `andi 0xFF`, then `bnez` or `beqz` -
which is the module's standard spelling of "is this zero" with the mask left in
**every one of the 168**.  So this is not a slip in one function but a compiler habit
visible at scale, and it is only visible at scale: the two instances that prompted it
were found by hand.

**The calibration is the useful part: those two functions are 1.2 % and 5.6 % of
their respective totals.**  Transcribing finds instances; counting says how many.

### The flag-accessor census was missing a whole spelling

`func_001A9B78` is `flags_18 |= 4` plus a float store, four instructions:

```
lw   $a1, 0x18($a0)
swc1 $f12, 0x34($a0)
ori  $a1, $a1, 0x4
jr   $ra
sw   $a1, 0x18($a0)
```

**`flag_accessors.py` does not match it**, and the reason is the same threshold that
last iteration's bit-15 case settled.  The tool's `get`, `set` and `clear` all build
the mask with `lui`, because every mask it had found was above bit 15.  Bit 2 fits a
halfword, so the mask is `ori $a1, $a1, 0x4` - and

> `ori rt, rs, imm` computes `rt = rs | imm`, so with `rt == rs` it is a read-modify-write
> in **one** instruction.  The `lui` spelling needs a *separate* register for the mask
> (`lui $t0, HI` then `or $a1, $a1, $t0`) because `or rd, rs, rt` has two distinct
> inputs and neither can be both the destination and the value being preserved.

**So the accessor length tracks the mask width, and the instruction the immediate form
saves is exactly the scratch register the wide form needed.**  `boolean_shapes.py`
counts the immediate form at **18 functions**, against the eight rows the census finds
for the wide form - **so more than half the immediate-width flag setters in the module
were invisible to the census that exists to find them.**

It also carries a third combination the census does not model: a flag setter that also
writes a float, but at offset 0x34 from `$f12` rather than -1.0f at 0x1C.

### Two censuses, both wrong first, both plausibly wrong

The counts above were wrong before they were right, and wrong in the way that produces
a number you would print without hesitating.

**The `sltiu` filter tested the REGIMM sub-opcode field against 9**, which is what the
standard MIPS table says.  **In this assembler's encoding `sltiu $a0, $a0, 1` is
0x2c840001, where bits 25-21 and bits 20-16 are both 4** - so no known member can say
which half carries the sub-opcode, and testing for 9 matches nothing in the module.  It
reported **zero** for a shape present in 168 functions.  The tool now matches on the
immediate and the shared register instead, and says in its own docstring that this may
admit other REGIMM forms with an immediate of 1.

**The `ori` filter required register `$a0` and reported 192 functions**, which were the
float-constant idiom: `lui $a0, 0x3F7D` then `ori $a0, $a0, 0x70A4` is 0x3F7D70A4 as a
float, and there are 192 of those in the module.  The register is not reliably `$a0`
either - `func_001A9B78` loads the flags into `$a1` because `$a0` is the object pointer
- and the missing condition was that the preserved value must have come from a **load**.
That left 18.

**And the counts are per function, not per occurrence**: 267 and 31 occurrences sit in
168 and 18 functions.  Quoting the occurrence count would have made the idiom look
twice as common as it is in the places a reader would go looking, since a function that
tests eight fields for zero is one function seen eight times.

### A tool gap that hid a real cluster, and an address I got wrong

`func_00102C84` copies three words to a fixed address built as `lui $a3, 0xF` then
`addiu $a3, $a3, -0x3898`.  **I computed that as 0x0C768 and went looking for a writer
of it.  It is 0x0EC768**, and `tools/code_writers.py` already listed it with this
function as its single writer.  The tool was right and the arithmetic was wrong, for
the third time in this project that a hand-computed `lui`/`addiu` pair disagreed with
a tool.  `addiu` sign-extends, so a negative low half subtracts from a *sixteen-digit*
high half and the result is nearly always a digit longer than intuition expects.

The second half of that investigation found a real gap.  **`base_of` in
`tools/stride_table.py` only matched `lui $rX` / `addiu $rX, $rX, lo`** - where the
`addiu`'s destination is the register the `lui` wrote.  `func_00102D34` uses the other
spelling, `lui $t0, 0xE` then `addiu $a1, $t0, 0x2168`, which keeps the base and
copies the address elsewhere.  **That form was invisible, and it hides a cluster of
three writers naming 0x0E2168.**  `base_of` now takes `any_dest`, off by default
because in a linked image nothing marks which `addiu` completes a symbol, so
`$base + 4` matches too; `code_writers.py --any-dest` goes from 1,024 functions at 455
bases to **1,242 at 515**.

**What 0x0EC768 and 0x0E2168 are is not established, and the honest position is
narrower than it looks.**  Both are real symbols - six and nine relocations target
them, every one from a `lui`/`addiu` pair, one of which belongs to the writing function
itself - and both land inside `.text`, before the containing nominal function's own
last `jr $ra`.  **That is the same test the three clusters in `code_writers.py --real`
pass, and it is not enough to distinguish a deliberate runtime patch from a data block
whose label the symbol map lacks.**  splat's function sizes are "distance to the next
label" and there is no label at either address, so the nominal body cannot be trusted
there.  At 0x0EC768 the bytes are a `nop` in a delay slot and at 0x0E2168 a `jr $ra`,
**which looks more like a data block than a patch target - but that is an impression
and not a measurement**, and the file says so.

### `func_00102D34`: the `lui`s that supply the byte `lwr` does not

Seventeen words copied to 0x0E2168, then sixteen `lwr` to the command buffer.  The
`lwr` offsets are 1, 5, 9, 13 and then 0x11, 0x15, 0x19, 0x1D - four per row, 0x10
between rows.  Each is preceded by `lui $t0..$t3, 0x3F00`, and those four constants are
**not** dead:

> `lwr` loads only from the given address to the next word boundary - three bytes at an
> offset of 1.  So the **low byte of each register is whatever the `lui` left**, which is
> `0x00`, and the `lwr` fills bytes 1, 2 and 3.  Each output word is therefore
> `0x3F00_00xyz`, which read as a float is in [1.0, 1.5) because 0x3F000000 is exactly
> 1.0f.  **The module decodes 24-bit fractions to floats here**, one word each,
> sixteen of them.

**This is the first instance in this tree of a "dead" high half quietly supplying part
of the value**, and it is the opposite of the dead `lui`s recorded elsewhere - there
the whole register was overwritten, here the high half is dead and the low byte is
load-bearing.  Omit the four `lui`s and the low byte becomes the previous `lwr`'s
output, which is a previous row's data.

Whether the values really are three bytes at that spacing is **not** recoverable from
the bytes: three-byte values at offsets 0, 3, 6, 9 would not be at 1, 5, 9, 13, and the
fourth read of each row lands inside the fourth four-byte field rather than at the
start of a fifth value.  What the offsets say is that the reads are deliberately
unaligned and deliberately strided, and why is not.

### The other three

`func_000F9C78` indexes an array of pointers with `sll 2` and reads one field of the
element - three dereferences deep.  **The stride is `sll 2`, the same spelling
`sortAndCullScene_1080` uses for a record of twelve bytes**, and the module has no
`mult` for any of these.

`func_0018A650` builds a five-word record and **materialises the source offset once** -
`addiu $a3, $a0, 0x1A0`, then three `lwc1` through it.  That is the same decision
`func_000C3470` makes at the other end of the module with its stack offsets, and the
same decision `func_00102D34` makes seventeen times with one `addiu` and sixteen stores
against a base.  **Forming a base pointer for a run of consecutive accesses is a habit
here, not a coincidence**, and it is the shape to look for when a function looks longer
than its arithmetic warrants.

And both of them agree with `func_0010FFF4` last iteration: **neither writes the cursor
back**, so the caller owns the advance and `func_0010FFF4`'s self-advance is the
exception rather than the rule.
### A pair where one function is a strict superset of the other

`func_00123588` and `func_00123560` are 0x28 apart and the first is inside the second:

```
func_00123588                  func_00123560
lw   $a2, 0x4($a1)            lw   $a2, 0x4($a1)
lw   $a3, 0x0($a0)            lw   $a3, 0x0($a1)   <- $a0 becomes $a1
sw   $a2, 0x4($a3)            sw   $a2, 0x4($a3)
lw   $a3, 0x0($a0)            sw   $a3, 0x0($a2)   <- reload not needed
sw   $a3, 0x0($a2)            sw   $a0, 0x4($a1)
sw   $a0, 0x4($a1)            lw   $a2, 0x0($a0)   <- new
                                sw   $a2, 0x0($a1)   <- new
jr   $ra                      sw   $a1, 0x4($a2)   <- new
sw   $a1, 0x0($a0)            jr   $ra
                                sw   $a1, 0x0($a0)
```

**Two of the four differences are consequences, not choices.**  The second load reads
`$a1` rather than `$a0` because the first unlink has already repaired that link, so
re-reading it would read a value the function has just written.  And the reload that
`func_00123588` needs disappears, because in this version `$a3` is still live from the
first unlink.

**The three added instructions are one unlink**, in the same `prev`-then-`next` order as
the first three - so the function is **two calls to the same four-instruction idiom
with one node shared**, and the eight extra bytes are the second call plus one load.

Read whole, it moves a node from one list into another and puts the other node where it
was: a splice across two containers, sixteen fields, all at offsets 0 and 4 of four
different objects.  **`func_00123588` on its own is a swap of two nodes' positions, not
an unlink** - an unlink would leave one of the four stores out - and the swap reading
rests on all four stores being present.

**The reload in `func_00123588` is a third kind.**  `func_0010FFF4` and
`func_000F7DA8` reload because a store through a memory-loaded base could alias the
value; here the value is reloaded because **it cannot survive its own use** - the
compiler would have to store from a register that the immediately preceding store had
just written through.

### `tools/base_pointer.py`: 687 functions, and my own comment was wrong

Five functions in this tree form a base pointer once and then run consecutive accesses
through it - `func_00102C84`, `func_00194ADC`, `func_000E3C24`, `func_0018A650` and
`func_00102D34` - and five instances cannot tell you whether that is a habit of the
module or of those five.

**850 functions do it: 687 through stores, 275 through loads, 112 through both.**  All
five hand-found ones are members, and `func_00102D34` has the longest store run found
at fifteen.  **So a function's length here is mostly a property of its codegen, not of
its data**, which is the opposite of what the three-word copies look like from the
inside.

**And the count contradicted a comment I had already written.**  `func_0018A650.c`
claimed the habit was store-side and cited the store count as evidence - but that
function forms `$a3 = $a0 + 0x1A0` for three **`lwc1`s**, and its three stores go
through `$a1`, which arrives as an argument and is never computed.  **It is only in the
load group.**  The file now says so, and this is the third time in this project that a
tool's output contradicted a claim in a file comment rather than the reverse.

**The tool's first version counted 4, and none of the five were among them.**  It
required every store to follow the `addiu` with nothing in between, on the reasoning
that a straight line is what makes the form worth noticing - **and `func_00194ADC` puts
`lw $a1, 0x8($a1)` between its `addiu` and its first store, as four of the five do.**
It also insisted the run start at offset 0, which `func_00102D34` cannot satisfy
because its first store goes through the *pre-`addiu`* register.  **Both were the test
being more specific than the shape.**  The habit this project has is to check that a
filter finds the examples that motivated it before counting anything with it; this one
was counted before that check, and the check is now written into the tool's docstring.

### The record format has more than one shape

`func_00110314` is `func_0010FFF4` with the tag changed from 3 to 2 and the payload
changed from a float to a word.  Same eight-byte record, same layout - payload high,
tag low - same cursor reload, same advance by eight.

**The reload is not optional in one and merely harmless in the other.**  Here the
payload arrives in `$a1` and the cursor is then reloaded into `$a1` itself, which
destroys it - but the payload is already stored by then.  `func_0010FFF4` carries its
payload in `$f12`, a different register file, which a reload cannot touch.  **The same
instruction sequence is correct for both because the two payloads live in different
register files.**

**All three of the record builders write the record's first word first** - `func_00110314`,
`func_0010FFF4` and `func_0018A650` - which is not the order a reader would guess and
is a shared habit rather than three coincidences.

### `func_000F7DA8`: a ring buffer, and `mfhi` where `mflo` usually goes

`*out = base[index]; remaining--; index = (index + 1) % limit;` - the `div` computes a
quotient nobody reads and a remainder that becomes the new index.  `div` and not
`divu`, again by function field 0x1A.

**The counter decrement is wedged between the `div` and the `mfhi`**, four
instructions from the instruction that writes `$hi`.  Nothing in between touches `$hi`,
so the value survives, but the ordering is a fact about the scheduler rather than about
the source.

**`ori $v0, $zero, 0x0` is written and never read.**  The function's results are the
store through `$a1` and the store in the delay slot; the remainder is left in `$a1`.
Under o32 a non-void function returns in `$v0`, so one reading is that it returns 0
*and* updates the ring, and the other is that the store is dead.  **The bytes cannot
settle it** - nothing here reads `$v0` and no caller is in the module - so it is
recorded as a zero being written to the return register and nothing more.

### `func_00093EEC`: two addresses of different kinds in one breath

Four words to 0x0E21C0, and a sixteen-bit counter at 0x01D4D00 incremented.  **The two
are not the same sort of thing and that is why both are written down separately.**

0x01D4D00 is in `.data` with seven relocations naming it: a real global counter.  0x0E21C0
is inside `.text`, 0x50 bytes into `func_000E2170`'s nominal body and before that
function's own last `jr $ra`, with five relocations naming it - **the same shape as
0x0E2168, which `func_00102D34` writes seventeen words to, and the two are 88 bytes
apart.**  So the module has at least three addresses written from code that sit in
code: 0x0E2168, 0x0E21C0 and 0x0EC768.

**What they are is still not established, and the position has not moved.**  splat's
function sizes are "distance to the next label", there is no label at 0x0E21C0, and the
same test that admits the three verified clusters in `code_writers.py --real` cannot
separate a runtime patch from a data block the symbol map has no label for.

The counter's `lh`/`sh` pair is signed and can go negative, but **the signedness is
real for the increment and irrelevant for the store** - the arithmetic is the same
modulo 2^16 either way - so a signed load is one instruction wider than needed and
says only that the field was read as a signed short.
### One family, one member that returns, and the instruction it spends instead

`func_00150730` copies three floats and returns the destination.  It is the sixth member
of the three-float copy family, and the only one that returns anything.

**The five void copies use the other spelling of the same copy, and the two forms are
both eight instructions:**

```
func_00150988   addiu lwc1 swc1 lwc1 swc1 lwc1 jr swc1
func_00150730         lwc1 move swc1 lwc1 swc1 lwc1 jr swc1
```

**So this function did not drop the base pointer to make room for the return - it spent
the instruction the base pointer would have occupied on the return instead.**  In the
void five the eighth instruction is the `addiu`; here it is `move $v0, $a0`.

**That is the one place in the module where the choice between the two copy forms is
visible inside a single family**, which is why it is worth having found, and it is also
the reason not to read `base_pointer.py`'s 687 as a rule.  The count says which form is
commoner; this instance is the exception, and `func_00055974` - below - is the case that
still needs explaining.

### The same function uses both spellings, twelve instructions apart

`func_00055974` copies a three-word node through a formed base (`addiu $t0, $a0, 0xAC`)
and four floats through immediate offsets (`0x0`, `0x4`, `0x8`, `0xC` of `$a2` into
`0x68`, `0x6C`, `0x70`, `0x74` of `$a0`).  **One `addiu` and eight accesses against
eight accesses with immediates is a tie on instruction count, and for the float half the
compiler took the immediate form.**

So the 687 is not a preference the compiler has and sometimes loses - **it is a choice it
makes in one function and not the other.**  What decides it is not established, and the
obvious candidates all fail to explain this instance: the word copy needs a fresh
register (`$t0`) and the float copy would too, so register pressure is not it; `$a0` is
needed again for `sb $a1, 0x10A($a0)`, so preserving the object pointer is a reason but
not a difference between the two halves.  **The observation stands and the mechanism does
not, and writing down a mechanism here would be inventing one.**

The function also sets two adjacent flag bytes at 0x10A and 0x10B to 1 - a hundred and
seventy bytes past the end of the float block and sixty-two past the end of the word
block.  **Two `sb`s and not one `sh`, so it is a pair of flags and not the halfword
0x0101**, and their offsets being unrelated to either copy says they belong to a third
thing the function only ever turns on.

### A flag setter the census has no shape for

`func_00012F14` writes a boolean into bit 31 of the word at offset 0x64:

```
self->word_64 = (self->word_64 & 0x7FFFFFFF) | ((arg & 1) << 31);
```

**`flag_accessors.py` models `get`, `set` and `clear` - each one operation - and this is
two, clear then set.**  Its `combined` path looks only for an accessor that also writes a
float.  So the census does not see it, and **forty bytes away `func_00012F3C` reads the
same bit of the same word and the census does find that one.**

That is precisely the situation the census's own docstring warns about when it says a
getter with no setter is evidence the other half was inlined away at every call site.
**Here the setter exists and the census cannot see it**, so the pairing it reports as
half-missing was not half-missing at all.  `boolean_shapes.py` gains a third shape and
counts **10 functions**, with the search bounded to eight instructions and to before any
branch - `func_00012360` is 372 bytes and contains two of these masks back to back, so
"some `or` later" would pair the first with something unrelated.

**That shape took two wrong versions to get right**, and both matched zero:

* requiring the `or` to write the same register the `and` did - in `func_00012F14` the
  `and` clears into `$a2` and the `or` writes a *fresh* `$a1` that reads `$a2`;
* requiring the `addiu -1` to complete the mask immediately before the `and` - an `andi`
  sits between them.

**A census shape needs its known member checked against it before it is counted, and both
versions failed that check in the direction of finding nothing** - which is the safe
direction to fail in, and still not the direction to fail in twice.

**And `andi $a1, $a1, 0xFF` at the top of the function is dead**: four instructions later
`andi $a1, $a1, 0x1` masks to bit 0, which subsumes it.  **A sixth dead mask, and a
third kind** - not the `sltiu`-then-`andi` pair of `func_000FBFD4`, not the all-ones mask
of `func_0014EAAC`, but a narrowing to a byte immediately followed by a narrowing to a
bit of that byte.

The clear mask is 0x7FFFFFFF built by `lui 0x8000` + `addiu -1`, where `lui 0x8000` +
`ori 0xFFFF` would give 0xFFFFFFFF - the *set* mask.  **The choice between `addiu -1` and
`ori 0xFFFF` is the whole difference between clearing bit 31 and clearing everything**,
and the compiler did not mix them up, which is worth noting because
`renderMeshInstances_1060` builds 0xFFFFFFFF the other way.

### A fourth code-written address, and a two-line change that found a quarter of them

`func_00102E6C` copies four floats to **0x0EC7E8**, and the `%hi`/`%lo` pair is split
across a store's offset field and an `addiu`:

```
lui   $a1, 0xF
swc1  $f12, -0x3818($a1)      the low half is the store's offset
addiu $a1, $a1, -0x3818        and here again, for the remaining three
```

**0x0EC7E8 is 128 bytes past 0x0EC768, which `func_00102C84` writes three words to, and
both land in the same nominal function** - `func_000EC5E8`, at +0x180 and +0x200.  **That
pairing is the strongest evidence this project has for these addresses being a data
region rather than a patch target**: two independent initialisers, in different
translation units, writing into one nominal function body a hundred and twenty-eight
bytes apart.  It is still not proof, and the limit is unchanged - splat's sizes are
"distance to the next label", there is no label at either address, and
`func_000EC5E8`'s nominal body may simply be swallowing a data block the symbol map does
not name.

**Finding it needed `code_writers.py` to look at the float opcodes.**  Its `store_bases`
listed seven integer load and store opcodes and omitted `lwc1` and `swc1`, so a function
writing nothing *but* floats named no load or store base at all and was rejected before
its address was ever considered.  **With two opcodes added the census goes from 1,024
functions at 455 bases to 1,458 at 675** - a quarter of the total out of a two-line
change, and the fifth such gap in this tool in as many iterations.

### A Gram-Schmidt step, and the contrast it provides

`func_000C8EC4` is `out = a - (dot(a,b) / dot(b,b)) * b` - the component of `a`
perpendicular to `b`.  **The division happens once**, not three times: the factor is
computed into `$f16` and multiplied into each component of `b`, which is the only part
not forced by the arithmetic.

**The two dot products are interleaved instruction by instruction**, not one after the
other.  That is load-latency hiding on a single floating-point multiply pipeline, not
code structure - six `mul.s` in a row would stall, so the two independent chains are
woven together.  Read as pairs it looks deliberate; no C would produce it.

**`$f0` is used as a general temporary** for `b.z`.  It is the return slot and
caller-saved, and the function has no return value - it writes through `$a0` and returns
`$f0` as scratch, which is why nothing restores it.

**There is no frame at all**, which makes this the useful contrast with `func_000C3470`.
Both are vector arithmetic; that one goes through 0xC0 bytes of stack three times over,
this one keeps three live values per vector in registers.  **The difference is not the
arithmetic, and that is the whole reason some float functions in this module can be
written as readable C and some cannot.**

### `func_000CD708`: two records, contiguous

The two destinations are 0x0C and 0x18 - twelve bytes apart, so **two consecutive
elements of a twelve-byte-element array**, both copied in one unrolled body.  **The
second `addiu` reuses `$a0` rather than adding to `$t0`**, which is only possible because
the first base is dead and the compiler knew it.

**Both halves are `func_00194ADC`'s shape, twice**, and it is a member of
`base_pointer.py`'s 687 with a longest store run of six.  Nothing checks whether the two
sources are adjacent, which is the only reason to mention it: written as two copies
rather than a loop, the aliasing question never arises.
### The cheapest pair of related functions in the module

`syncSkeleton_22B8` and `func_0012A9D0` are fifty-two bytes each and differ in **exactly
one instruction** - the thirteenth, the one in `jr $ra`'s delay slot:

```
syncSkeleton_22B8   ... vqmul.q R100, R200, R201 / svr.q / svl.q / jr $ra / nop
func_0012A9D0       ... vqmul.q R100, R200, R201 / svr.q / svl.q / jr $ra / move $v0, $a0
```

**The return costs nothing, because the delay slot was going to be a `nop` anyway.**

The pair `func_00123588` / `func_00123560` differs by eight bytes and three instructions
of genuinely different work; `func_00110314` / `func_0010FFF4` by a constant and a
payload type.  **This pair differs only in what the function hands back.**

**They are in different translation units**, and that is worth saying carefully because
it is a naming fact and not a unit-boundary one: `updateNodeGraph_0E48.c` records the
opposite caution at length.  Here both are byte-identical modulo one instruction, **so
whatever produced them was available to both, and the names tell us only that the
linker kept one of them.**  The operand registers *do* differ - `$a0`/`$a1` against
`$a1`/`$a2` - because one writes through its first argument, and that is a real
convention difference rather than a naming artefact.

### A bounded negative, and the tool that records it

Both functions store their result with two 64-bit stores of one quad's two halves,
**twelve bytes apart**:

```
svr.q R100, 0x0($a0)     the encoded immediate is 0x3
svl.q R100, 0xC($a0)     the encoded immediate is 0xD
```

**The immediates are three and thirteen because `svl`/`svr` encode offset minus three**,
the same convention as `lwl`/`lwr` - so a listing and the word disagree by three in both
stores.  **Two 64-bit stores of consecutive halves twelve bytes apart do not tile a
sixteen-byte vector**: a contiguous four-float store would need offsets 0 and 8.  So
either the destination has a four-byte gap at offset 8, or the two instructions write
halves in an order this project has not established.

`tools/vfpu_split_store.py` was written to settle it from the module and could not.
**Five functions use an `svr.q` + `svl.q` pair - `func_0012A9D0`, `func_0012DE0C`,
`func_0012DE70`, `syncSkeleton_2120` and `syncSkeleton_22B8` - and every one uses the
identical pair of offsets, 0x3 and 0xD.**  There is no second instance at different
offsets, so nothing here disambiguates which half each store writes.  **That is a bounded
negative and the tool says so**, rather than leaving the question open in a comment
where it would read as an oversight.

### A round trip that changes nothing

`func_0012D9F8` loads sixteen floats into four quads with sixteen `lv.s` and stores
them back with four `sv.q`.  **S200-S203 are one quad, S210-S213 the next, and so on, so
each `sv.q` writes back exactly the four floats that fed it** - and a byte comparison of
the sixteen words before and after finds no difference.  There is no arithmetic between
the last load and the first store, and the return's delay slot is a plain `nop`.

So it is not a computation.  Three readings and no way to choose between them from the
bytes: it exists to touch the memory and the vector cache; it is a stub the compiler
emitted before the operation using it went away; or it is a no-op by accident.  **What
can be said is the shape, and the shape is the vector-unit counterpart of
`func_000C3470`'s spill storm** - opposite cases, twenty instructions where zero would do
against sixteen floats through 0xC0 bytes of stack three times over.  **That one moves
floats through memory because psp-gcc would not keep them in registers; this one keeps
them in registers and does nothing with them.**

**And the vector unit is used the long way here.**  Sixteen `lv.s` loads where four
`lv.q` would do the same work, because `lv.s` is the only scalar load the vector unit
has and the source named sixteen separate floats.  `renderMeshInstances_1060` uses eight
`lv.q`, so **both spellings are present and the module does not always pick the
shorter one** - which is the same lesson as `func_00150730` last iteration from the
other side of the ISA.

### The listing this file was transcribed from was one instruction wrong

`func_0012D9F8` was first written with the fourth `sv.q` in the return's delay slot.
**That built 84 bytes against an original of 88 - one instruction short - and was wrong
about the function.**  `tools/c_shapes.py` had printed it that way:
`sv.q R203` at 0x0012DA44, then `jr $ra` at 0x0012DA48 and `sv.q R203` again at
0x0012DA4C.

`tools/disasm_range.py` prints four stores and then `jr $ra` and `nop` - the twenty-two
words the declared size says are there.  **The project's standing rule is to trust
`disasm_range.py` over a shape listing, and this is the case the rule exists for**:
the shape tool's rendering was plausible, readable and one instruction wrong, and
`verify_c.py`'s size check caught it in one pass.  `try_func.py` alone would have shown
a 4-byte shortfall rather than naming the cause, which is why the size check is not to be
weakened.

### A claim about `vqmul.q` that was withdrawn

Both new vector files first described `vqmul.q` as "the saturating form".  **That was not
supported.**  What the module shows is narrower: the `.q` suffix is the quad element
format - `lv.q`, `sv.q`, `vmmul.q`, `vpfxs` all carry it - and the leading `q` of
`vqmul` is a different modifier.

**The module contains exactly two `vqmul.q` instructions and both are in these two
functions**, so it is the only quad multiply here at all, against six `vmmul.q` - the
matrix form `renderMeshInstances_1060` uses - **and no `sat` anywhere in its 542 vector
instructions.**  So whether the multiply clamps is not established, and a saturating
variant would have been spelled with a `sat` this module never writes.  Both files now
say that instead of asserting the stronger and wrong thing.

### 456 of 456, and the last function was the one written in C

`tools/verify_c.py` now reports **456/456** and `config/matched_c.txt` lists 456
names against 456 files in `src/eboot/` - so for the first time there is no
documented-but-unmatched attempt left in the tree.  The module image is still
2,030,864 / 2,030,864 and `tools/check_symbols.py` still reports that all sizes
agree.

The function that closed the gap, `func_00000A48`, was **the last one still
written as ordinary C**.  It had been correct - `pos += delta`, normalise, then
clamp - and it measured 156 bytes against an original of 224.

**156 against 224 is not a scheduling accident, and it is worth being precise
about why.**  The original contains two float stores that go through the stack
for no reason any language can express:

    swc1  $f15, 0x4($sp)
    swc1  $f13, 0x8($sp)
    lw    $a3, 0x4($sp)
    lw    $t0, 0x8($sp)
    sw    $a3, 0x0($a2)
    sw    $t0, 0x4($a2)

Eight instructions for two stores, twice, and a reload from memory in the middle
of the second comparison.  GCC has no reason to emit either.  Since the rule is
that C is only promoted when it reproduces the bytes, the function was
transcribed rather than coaxed.

**This is the same shape as `func_000C3470`'s spill storm and the two are not the
same case.**  That one spills because psp-gcc would not allocate a register for
a value it needed.  This one spills in the middle of an expression: the float
unit hands results back through the frame so the integer unit can address them.
Calling both "the compiler spilling" would have been the easy summary and would
have been wrong about this one.

### `bc1t` is not a "likely" branch, and bit 0 is not a nullify flag

The two FP branches in `func_00000A48` encode as `0x45010010` and `0x4501000C`.
A first reading of those words - taken from the hex in the splat comments, which
are the raw bytes in order, not the assembled word - suggests `nd = 1` and that
`bc1t` annuls its delay slot.  **Both of those readings are wrong**, and the
wrong version changes the meaning of the function:

    bc1t  1f
      mtc1  $a2, $f12      ; loads EPS

If the delay slot only ran when the branch was taken, `$f12` would hold `EPS` on
the `len > 0` path and `0.0f` on the other, and the second test would mean two
different things depending on how control arrived.  **It does not.**  Bit 0 is
part of the word offset - `0x45010010` is offset 16, branch at `0xAA0`, target
`0xAE4` - and the assembler offers the nullifying forms separately as `bc1tl`
(`0x45030010`, bits 17 and 16 both set) and `bc1fl`.  So the delay slot runs
either way, `$f12` is `EPS` on both paths, and the second test is the plain
`len <= EPS` that the readable C said it was.

**The hex in a splat comment is the file's byte order, not the assembled word.**
Every word in this function disagreed by its two halves until the comparison was
made the right way round, which is the same trap as reading `lui`/`addiu` pairs
by hand: the tools were right and the arithmetic was not.

### One `nop` that was not a delay slot

With `.set noreorder` scoped to each branch, the build came to **228 bytes
against 224** - one instruction too many - and objdump put the extra word after
`mtc1 $a2, $f14`, which is not a branch and has no delay slot to fill.  Turning
reordering back on partway through the body is what produced it; with
`.set noreorder` over the whole body the assembler emits all 56 instructions
verbatim.

That single word also pushed the first `bc1t`'s offset from `0x10` to `0x11`, so
**the size check was reporting the symptom and the offset comparison was the
symptom.**  Every delay slot that exists here - the two `bc1t` slots and the one
after `c.le.s $f13, $f12` - is now written out explicitly in the source.

### What this iteration did not change

`asm/eboot/func_00166618.s` was found empty on arrival, which read as lost source
material.  **It was not**: `tools/split.py --build` regenerates the per-function
asm from the ELF, and it came back at 5,013 bytes with no diff against `HEAD`.
The tree also arrived with 5,491 files showing a pure line-ending change; the
same build pass normalised them, and the real diff is now 22 files plus 5 new
ones.

`tools/sync_counts.py` propagated 456 into both reports, and it left this behind
in `README.md`:

> 639 of the 639 files here are verified to compile to the original bytes; the
> remaining 1 is the only undecided attempt.

**The tool substitutes counts; it does not know that the sentence it substituted
into assumed an unmatched file.**  With zero unmatched the sentence contradicts
itself, so it is written out by hand now.  This is the third time a generated
count has been right and the prose around it stale, and the pattern is
consistent: the numbers should come from the data and the sentences should not
be assembled from the numbers.

### The renderer cluster, and an audit that found a constant misread

`tools/verify_c.py` reports **471/471**, `config/matched_c.txt` lists 471 names
against 471 files, and the image is still 2,030,864 / 2,030,864 with all sizes
agreeing.  This iteration did two things: audited the whole project, then spent
the rest on the geometry subsystem.

#### The audit

- `config/matched_c.txt` and `src/eboot/` agree exactly: no C file outside the
  verified list, no verified name without a file, no duplicates.
- `verify_c.py`'s size guard is untouched: `if len(got) != size: return False`.
- Every `nonmatching` size is 4-aligned, and every function's declared byte count
  equals its instruction-comment count.
- No symbol is defined by two different C files.

**Four functions looked wrong to the audit script and were not.**  Three have
comment words with fewer than four hex digits because they sit at low addresses,
and `syncSkeleton_2808.s` has nine alignment `nop`s *after* its `endlabel` that
belong to whatever follows.  The script was wrong in all four cases, which is the
same asymmetry as the `lui`/`addiu` arithmetic earlier: the tooling is right more
often than the hand arithmetic that tries to replace it.

#### What the audit found that was real

`src/eboot/func_00000000.c` described `0x43340000` as "the 2^28 scale of the
engine's fixed point world".  **That is false.**  `0x43340000` is exactly
**180.0f** - exponent `0x86`, mantissa 0.40625 - and 2^28 as a float is
`0x4D800000`, a different constant.

**The instructions were never wrong; the reading of them was, and the file still
compiles to the original 0x2C bytes.**  What made it worth fixing is that a "2^28
fixed point world" is a plausible-sounding story about this engine and nothing
supports it, so it would have propagated into every later file that reused the
constant.  180 is the degrees in a half turn and the same constant appears in
`func_0010260C`, so **an angle conversion is the likely reading - but that is a
reading, and the file now says so.**  The macro was renamed `SCALE_2_28_HI` ->
`SCALE_180_HI` and the rename touched only the comment and one operand.

This is the fourth documentation error found by auditing rather than by
decompiling, and the pattern is the same each time: **the bytes were right and the
sentence about them was not.**

#### Six functions in the renderer that compute nothing

`func_00102228`, `func_001022B0`, `func_001029E0` and `func_001029E8` are all
eight-byte `jr $ra; nop` stubs, and `func_00102230` and `func_0010224C` are
frame-and-`jal` wrappers around two of them.

**Taken together the six form a closed subgraph with no effect at all**, which is
a stronger statement than any one of them makes alone.  Four identical bodies
survived the linker unmerged, and two of the four are called by nothing while two
are called by the wrappers.  **A stub nothing calls and a stub something calls
compile identically**, so that split does not separate "unreachable stub" from
"feature compiled out on this platform" - and it is exactly the evidence that
leaves both readings alive.

#### The renderer object, read from its initialiser

`func_001028BC` is the initialiser behind `func_0010265C`, and it is the largest
function in this cluster read so far:

    func_0014CC2C(self, 8, 0x40, func_001ACBDC)   -> 8 x 0x40 array, constructed
    obj->[0x264] = 0.1f          (0x3DCCCCCD)
    obj->[0x268] = 7
    obj->[0x26C] = 0
    obj->[0x270] = &D_04020020   (four bytes of .rodata, not decoded here)
    obj->[0x27A] = 0x7FFF        (32767)
    obj->[0x27C] = 0
    then 8 iterations, stride 0x40, seeding (0, 0xC8000000) at +0x10 of each

The loop is what makes the four-argument call readable rather than a guess: it
starts at the object itself and walks exactly 0x200 bytes with a 0x40 stride, so
the array is at offset 0 and each element is 0x40 bytes.

**The bounds are lopsided and that is in the bytes.**  `0xC8000000` is exactly
`-131072.0f` and `0x7FFF` is exactly 32767: a floor of -2^17 against a ceiling of
2^15 - 1.  Any name for these fields has to keep the asymmetry.  **Whether the
pair is a clamp bound or a scratch coordinate is not settled** - both readings
fit - so the files record the words and not a purpose.

#### Offsets 0xC and 0xE are a half-resolution pair

`func_0010233C` truncates two floats to integers, halves both with `sra`, and
stores them as two `sh` at `0xC` and `0xE` of the render object.  That upgrades
the reading `func_00102280` could only offer: **it writes `0xE` and cannot see its
partner, this one writes both from a single call**, so the pair is a width and a
height at half scale.

The order is truncate-then-halve, and for a negative value those differ:
`trunc(-3.7)` is -3 and `-3 >> 1` is -2, where halving first would have stored -1.
**So negative odd dimensions round toward negative infinity twice over.**
`trunc.w.s` is a trap on out-of-range input, which is why the field is a signed
half-word.

#### A file format named in the binary: `surf`

`func_0010260C` passes `D_66727573` - the bytes `73 75 72 66`, **"surf"** - to
`elem_register_chunk_tag` with a record length of `0x3A` and an alignment of
`0x80`, immediately after caching `sym_001DB3B4 / 180.0f` and its reciprocal.

**This is the only function in the cluster that names a file format, and it is
what makes the cluster read as the renderer rather than as arbitrary integer
code.**  The two-way reciprocal cache is the same idiom as `func_00000000` and
now has two independent instances, which is evidence of a house habit rather than
a local accident.

#### A delay-slot idiom that recurs

Three of these functions put a useful instruction in a call's delay slot - a
`%lo` completing a `%hi`, a second `sra`, a second `swc1`:

    jal func_0014CC2C     / addiu $a3, $a3, %lo(func_001ACBDC)
    jal func_0010215C     / sra   $a1, $a1, 1
    jal elem_register_chunk_tag / swc1 $f12, %lo(sym_001DB3BC)($t0)

**In the last one the delay slot is the only reason a caller-saved register is
legal at all**: `$t0` dies at the `jal`, and the store that needs it has already
run.  Same pattern in `func_001023A0`, where `$s0` holds the *address* of the
global rather than its value precisely so the `lw` can sit in the slot and the
`sw $zero` after the call has a live base register.

#### Two report sentences that had to be un-numbered

`tools/sync_counts.py` reported "already in step" while `README.md` still said
456.  **The tool substitutes into sentences it recognises, and the sentence I
rewrote by hand last iteration was not one of them** - so a correct number sat
next to a stale one and the tool saw nothing wrong.  Both that line and
`c_candidates.py` "the remaining 7,485 functions" now name no number at all:

> Every file here is verified by `tools/verify_c.py` to compile to the original
> bytes ... The exact count is `wc -l config/matched_c.txt`, which is generated,
> and this sentence deliberately carries no number so it cannot go stale.

**That is the third fix of the same kind and the first one that removes the
hazard instead of correcting it.**  `sync_counts.py` is a good tool with a known
limit, and a number written into prose by hand is going to keep needing a human
to notice.  Saying where the number lives is more useful than restating it.

#### What thirteen more functions cost, and one that could not be promoted

The batch added `func_00000058`, `func_00000150`, `func_00000B58`, `func_00000F6C`,
`func_000026B8`, `func_00004F90`, `func_000061A0`, `func_000061E0`,
`func_00009448`, `func_0000946C`, `func_0000D478`, `func_00010134` and
`func_00010220`.  Most are the two-line prologue plus one `jal` shape, and they are
readable for what they say: **`func_000061A0` and `func_000061E0` are the same
constructor**, the first calling `func_000550E0` and the second storing the same
two global pointers before branching on a flag bit.  `func_00009448` and
`func_0000946C` are sisters differing only in the constant handed to
`func_00086574` - 0x1F against 0x20, which is 31 against 32 and reads as an index
into a fixed table.

**`func_000100F4` verified byte-exact and still could not be promoted.**  Its
inline `asm` reproduces all 0x40 bytes, `verify_c.py` says MATCH - and the link
then fails, because `asm/eboot/renderCommon_0580.s` reaches *into* the middle of
it:

    /* 1BCF58 001BCEE4 0100123C */  lui   $s2, %hi(.Leboot_00010128)
    /* 1BCF5C 001BCEE8 28015226 */  addiu $s2, $s2, %lo(.Leboot_00010128)

**`.Leboot_00010128` is a label inside `func_000100F4` that another translation
unit takes the address of**, and the original carries it in the `glabel` list for
exactly that reason.  A `__asm__` block inside a C file cannot export it: the
symbol is local to the emitted fragment, so `psp-ld` reports
`undefined reference to '.Leboot_00010128'` from the referring object.

**So the byte-exactness is not the binding constraint - the addressability is.**
That is worth stating plainly because it is the first case here where a function
was verified correct and still had to be rejected, and the honest reason is that
promoting it would delete a symbol the rest of the module links against.  The file
stays in `asm/`.  `func_00010134`, its near-identical neighbour that nothing
points into, promoted without difficulty.

**Two of the batch were dropped after failing**, and both failures are the size
guard doing its job rather than a near miss:

* `func_0000150C` (140 bytes) assembles 136.  Its `beqz` targets `.Leboot_00001578`
  and every other word matches; the missing four bytes are the second `swc1` pair
  in the vector copy, which GCC merges into the pair of `lw`s that reload from the
  stack.  **Not promoted: 136 is not 140.**
* `func_00000204` (168 bytes) came out at 164 after two attempts, the copy loop
  losing one instruction each time to register reuse.  **Not promoted.**

`func_00009378` was a third: correct instruction for instruction, but the `jal` to
`func_00009490` resolves to zero when the file is compiled standalone, so the
verified section was 100 bytes where the original is 104.  **The size guard caught
it and the file was deleted rather than kept as a "close enough" candidate.**

#### Twelve more: the shapes that carry their own documentation

The next twelve are all under 0x30 bytes and, taken together, they say something
the individual files cannot.  Four of them are one-call forwards:

    func_0010AA3C  call func_0010F7CC, then call func_001138F0 with both results
    func_0010B358  call func_0010B28C with a constant 1 in the delay slot
    func_00110ADC  unpack {0($a1), 4($a1)} into {arg1, arg2} of func_00113458
    func_0011BD2C  hand func_0011BB60 a stack slot and read the answer back out

**Those four are the same shape as `func_00009378` was and `func_001023DC` is** -
thin adapters that exist to give one function a different signature.  Their bodies
are almost entirely delay-slot work, and in three of the four the argument setup
*is* the delay slot.

Two more return a hard-coded constant and discard what the callee said:

    func_0010CD40  jal func_001085A0 / ori $v0, $zero, 0x1
    func_00110DE4  jal func_00112754 / or  $v0, $zero, $zero

**So `func_0010CD40` reports success unconditionally** - whatever
`func_001085A0` returns is never read.  These are the `void`-in-int wrappers the
engine is full of, and the distinction from `func_00112148` is worth naming:
that one *does* use the result, and `sltu $v0, $zero, $v0` casts it to a
boolean, destroying any distinction between 1 and 2 on the way out.

#### The chunk-tag pair, and a byte order that reads backwards

`func_0016F674` and `func_0019B9B8` are instruction-for-instruction identical
apart from one `lui`/`addiu` pair, and both call `func_000D90F4` after narrowing
the caller's third argument with `andi $a3, $a2, 0xFF`.  **That is the second
independent instance of this registration idiom**, which makes it the house
pattern for a chunk-table entry rather than a local habit.

The tags are `D_66727573` = **"surf"**, the third sighting of that name, and
`D_2161756C`.  The second one is the reason to write it out: the bytes are
`21 76 6C 61`, and read in the order they sit in the file that is
**"!vla" with the `a` last** - `!vlab` by the eye, and the trailing `a` is what
the eye keeps dropping.  **A four-character code with a leading `!` and a
trailing letter is the kind of thing a reader silently "corrects", so the file
records the bytes rather than the intended word.**

#### `func_00110014` and `func_00110AB4` fix the element size of the render array

Pushed together they pin down a data structure neither could alone:

    func_00110014  stores the word 3 and one int-as-float, then advances the
                   array pointer at 0x8($a0) by 8
    func_00110AB4  computes  0x8($a0) - (index * 8) - 8

**So an element is 8 bytes - a word plus a float, 4-byte-aligned with no padding
- and `func_00110AB4` indexes backwards.**  An index of 0 gives the element one
*before* the cursor, which is why the constant is `-8` and not `0`: this is the
address-of-element walk a pop loop performs, counting down from the current end.

`func_00110014` has no frame at all.  It reloads `0x8($a0)` a second time rather
than keeping the pointer, and stores the incremented value in the delay slot of
the `jr` - **tail-call-shaped code with nothing to tail-call.**

`func_001277FC` is a null guard with a twist: it loads `-4($a0)`, **the word
before the object**, and passes that onward.  That is a vtable or a leading tag -
the C++ `this - 1` adjustment - so `func_00127770` receives a base pointer, not
the derived object.

#### Nine more, and a fourth sighting of the 180.0f helper

`func_0012323C` and `func_00124854` are **the same instruction sequence**, word
for word, differing only in which three globals they touch:

    func_0012323C   sym_001DE690 / 694 / 698
    func_00124854   sym_001DE7A0 / 7A4 / 7A8

Both read a divisor, divide it by `0x43340000` (**180.0f**, not 2^28) and by
180.0f in the other direction, and store both quotients.  With
`func_00000000` and `func_0010260C` that is **four independent copies** of the
reciprocal-cache idiom.

**The repetition is the evidence, and it points somewhere specific.**  A helper
written four times identically is a template the original team pasted per class,
not code that was shared - if it had been shared, the linker would have one symbol
and the copies would call it.  Four sets of three consecutive globals, each
holding a divisor and its two reciprocals against 180.0f, is a **per-class
conversion rate table**.  180 is the divisor of a degrees-to-radians or
unit-normalisation step, and each copy belongs to a different class.

Whether the stored divisor is a length scale or an angle is **not settled by these
bytes**; what is settled is that four classes each carry their own.

`func_00112464` adds a fourth sighting of the 8-byte element size: it walks an
array of `{word, float}` pairs comparing each against a key, returning a boolean.
With `func_00110014` (push), `func_00110AB4` (pop) and `func_00112464` (search),
**the array and its three operations are now all readable**.

#### Three functions that read as inverses of each other

`func_00112594` registers the operation name `"concatenate"`, and only if the
incoming descriptor is not already id 4 does it first substitute `$a2` for `$a1`.
The `ori $t0, $zero, 0x4` exists solely to hold that constant for one `bne` -
**4 is a slot id, not a length or a tag.**

`func_00116B48` looks like a plain version compare and is not: on the *equal* path
it calls `func_00116990` first and discards the result, then returns 1
unconditionally.  **It is a compare-and-refresh, and the expensive path is the
cache hit** - the opposite of what the name suggests.  A refresh that failed
still reports a hit.

`func_0011631C` is the integer base-8 logarithm, and the way it computes the
logarithm is the interesting part: it divides by **2** in a loop rather than by 8,
so the loop counts halvings and the final `sll 3` converts at the end.  The
result is `(halvings << 3) | odd part`, packed into one word.

#### Seven more, and what the vtables say about the classes

Seven functions this pass, three of which are `func_0019D4AC`, `func_0019D508` and
`func_0019D564`: constructors identical but for one `lui`, each stamping a vtable
pointer into **0x18($a0)** before calling the base constructor `func_000B9C88`.
**None of them writes `$v0`** - they are `void` constructors that hand back their
argument in `$a0`.

**The constructors make the three classes look independent; the vtables say
otherwise.**  `sym_001EA3E8`, `sym_001EA4B8` and `sym_001EA588` are exactly 0xD0
apart, each 26 entries of 8 bytes (a null word plus a pointer), slots running from
+0x08 to +0xC8.  Compared entry by entry, **the three differ in exactly two
slots**:

    +0x08   the constructor itself: 0019d4ac / 0019d508 / 0019d564
    +0x30   000bbd84 / 000bbdc8 / 000bbe78

The other 24 slots are the same address in all three.  **These are siblings that
override one virtual method each**, the seventh virtual entry - not three classes
with their own interfaces.  Their overriding methods are wildly different sizes
(0x44, 0xB0 and 0x260 bytes) and all three begin by loading `0x8($a0)` and reading
a float from it, which is consistent with three ways of doing the same geometric
step rather than three unrelated operations.

**Recording this as a correction.**  The first version of these files claimed each
vtable was "0x34 entries" and that this showed "three classes with real virtual
interfaces".  Both were wrong, and the error came from reasoning about what a
0xD0-spaced vtable ought to contain instead of dumping the 26 entries and comparing
them.  **The measurement is 26 slots, two of which differ** - a much more specific
claim, and the opposite of the one the guess would have supported.**

`func_0019D11C` is the matching dispatch and explains why the adjustment field is
a *half-word*: it reads a thunk entry `{i16 adjust; void (*fn)()}` at `+8` from
what `0x0($a0)` points to, and adds the signed adjustment to `this` in the `jalr`
delay slot before transferring control.  **That is the multiple-inheritance
`this` fixup, and `lh` rather than `lw` is what makes the offset signed.**  The
`jalr` is a genuine tail call - the frame is torn down immediately after and no
return address is fixed up.

`func_001248D0` is the one genuinely geometric function here, and it is a
one-dimensional interval walker.  Five floats:

    0x0  current value          0x4  period / full scale
    0x8  scale factor           0xC  cursor, the field being advanced
    0x10 bound

It adds the incoming step to the cursor and, on overflow, rescales the value as
`(value / bound) * scale + value`.  **`bc1fl` is the nullifying form and the
nullification is load-bearing**: on the path where the cursor is still inside the
bound, the `ori $a1, 0x1` in its delay slot is *discarded*, leaving `$a1` at 0 and
sending the `beqz` to the rescale.  Read plainly, the flag means "no wrap
happened".  Note that 0x4 is used only on the non-wrap path and 0x8 only on the
wrap path.

`func_0019B9DC` registers the **same "!vlab" tag** as `func_0019B9B8` but through a
different entry point, `func_000D6B20` instead of `func_000D90F4`.  **So "!vlab"
has two registration paths and "surf" one** - the tag is not a key into a single
table, it selects how a handler gets installed.

`func_001AB9B8` is a vtable installer whose first store is dead in the original:
it writes `sym_001EDC30` into 0x0($a0) and then overwrites it with
`sym_00001C54` one instruction later.  **The second `beqz $a0` is also
unreachable** - the first branch already proved `$a0` non-zero.  Both artefacts
are in the shipped code, not in any reading of it.

#### Twenty-one functions: the shared vtable, read in full

With the vtables identified, the next step was to promote the methods they point
at - all twenty-three shared slots - and that turns the table from an address
list into something readable.

**The twenty-three shared slots are almost entirely constant answers:**

| slots | count | body |
| --- | --- | --- |
| +0x10, +0x18 | 2 | return 1 |
| +0x48 .. +0xC8 | 16 | return 0 |
| +0x48 | 1 | returns nothing at all |
| +0x20, +0x28, +0x38, +0x40 | 4 | real bodies, 224-348 bytes |

**`func_0018F668` is the one method that leaves `$v0` untouched** - the delay slot
is a `nop`, not an `or`.  Of 26 slots, then, twenty return a constant, one returns
nothing, four do real work, and the twenty-fifth (`+0x30`) is the single method
the three siblings override.

So the base class offers **a predicate interface that answers "no" to sixteen
questions and "yes" to two**, and none of these three classes touches any of them.
That is the shape of a set of capability flags where the defaults are almost all
negative - and it is a stronger claim than "the classes are similar", because it
says *they use the same interface and implement none of it*.

#### The two real shared methods, and a sentinel worth naming

`func_000B9D3C` (slot +0x20, 240 bytes) and `func_000B9E2C` (slot +0x28, 348 bytes)
open with the **same two-stage interning lookup**, and both use `bnel`, whose
nullification is the test itself:

    lw    $a1, 0x0($a0)         ; a tag at +0xC of the table header
    ori   $a2, $zero, 0x1
    bnel  $a1, $a2, .Lnext       ; if the tag is not 1, skip
      addu  $s2, $a0, $a1       ; else  s2 = header + tag

**So the constant 1 is a sentinel meaning "not interned yet".**  The arithmetic in
the delay slot runs only on the path the sentinel selects, which is the whole point
of using `bnel` rather than a branch plus a manual test.

The two then diverge.  `func_000B9D3C` calls `func_001224E0` once per index with a
fixed 4 as the third argument and strides by 4 - a callback-per-entry walk.
`func_000B9E2C` instead **contains a hand-written overlap-safe `memmove`**: the
`sltiu $a3, $a2, 0x4` / `bnel` pair chooses direction, and `sltu $a2, $a2, $a0`
after `addu $a0, $a2, $a0` is a pointer-arithmetic overflow test.  **The
destination register is only set after that check passes**, which is what makes the
copy correct on wrapped addresses - a naive `memcpy` would not have the check at
all.

`func_000B9E2C` also keeps a float register `$f20` saved and restored across the
whole body that only ever holds 0.0f.  **The accumulator is a constant**: it is
staged through 0x10($sp) and written into the float array at `0x8($s0) + $s6`,
so the array is being defaulted, not summed.

#### The sentinel appears three more times, and the table resolves into an interface

Finishing the four real bodies (`func_000B9F88` at +0x38 and `func_000BA16C` at
+0x40) made one thing clear enough to state: **the sentinel-1 "not interned yet"
test is in every real method in the table, three times.**

    func_000B9D3C  bnel $a1, $a2 / addu $s2, $a0, $a1
    func_000B9E2C  bnel $a1, $a2 / addu $s3, $a0, $a1
    func_000B9F88  bnel $a3, $a0 / addu $a1, $a2, $a3
    func_000BA16C  bnel $a3, $s3 / addu $a1, $a2, $a3      (twice)

Read as an interface, the twenty-three shared slots now name a coherent thing:

    +0x08  construct
    +0x10  is-yes   (constant true)
    +0x18  is-yes   (constant true)
    +0x20  walk entries, callback per index          func_000B9D3C
    +0x28  walk entries, copy to a buffer           func_000B9E2C
    +0x30  (the one slot the three siblings override)
    +0x38  look a name up, return index or -1       func_000B9F88
    +0x40  look a name up, copy 0x28 bytes of it    func_000BA16C
    +0x48  void                                     func_0018F668
    +0x50 .. +0xC8  sixteen constant-false queries

**It is a symbol-table interface**: intern, enumerate, and resolve, over a table
whose entries carry a `char[0x28]` name.  `func_000BA16C` copies exactly **0x28 =
40 bytes** - twice, in two delay slots - and that is where the name length comes
from.  Its two failure paths each write a single `sb $zero`, not 40 zero bytes:
**the buffer is assumed to already hold its old contents and clearing one byte is
what marks it empty.**

`func_000B9F88` returns **-1** on failure and has *three* separate sites writing
it, which means three distinct ways to give up: the table pointer was null, the
loop ran out, and the early bail.  Its inner loop is worth noting because it is a
mistake in the original rather than an idiom: on a failed comparison it jumps back
to the **table header** (`beqz $v0, .Leboot_000BA030`), restarting the whole
lookup, instead of continuing the search.  A comparison that fails mid-scan
restarts the scan.

#### The three overrides are unrelated to each other

The slot +0x30 methods are `func_000BBD84` (0x44), `func_000BBDC8` (0xB0) and
`func_000BBE78` (0x260).  Two of them are now transcribed and **they share no
shape at all** - one is a single call, the other reads two 12-byte records and
writes back three floats - which is why the three classes look independent even
though their vtables differ in one slot.

`func_000BBD84` passes `0x8($a0)`, `0x14($a0)`, `0x40($a0)` and `0x44($a0)` to
`func_00125410`, plus a boolean taken from **bit 16 of the incoming index**
(`lui $t0, 0x1` / `and $a1, $a1, $t0` is a sign mask that keeps one high bit, not
a 16-bit truncation).  The object holding state at 0x8 and again at 0x40 and 0x44
means **0x34 bytes of other fields sit between them**.

`func_000BBDC8` reloads `lw $a0, 0x8($s0)` three times while copying results out -
`lwc1` / `lw` / `swc1` three times over.  Nothing in that sequence clobbers `$a0`,
so **the reloads are redundant in any ordinary reading** and are recorded as
such rather than explained away.

#### The last override is a weighted 1-D solver, and getting it took four tries

`func_000BBE78` (0x260 bytes, the +0x30 override of `sym_001EA588`) is the only
one of the three that shares no shape with the others.  It advances a transform
node by a weight and, when the motion leaves the node's interval, pushes the
leftover to the parent and retries.  **Its node is a float interval where a
field that is exactly 0.0f means "unbounded"**, which is why every
`c.eq.s $f17, $f16` in the body is a *configuration* test and not a bound check:

    0x0C  lower limit (0.0f = none)     0x10  upper limit (0.0f = none)
    0x14  step size   (0.0f = none)     0x18  step size   (0.0f = none)
    0x1C  accumulated weight            0x20  position
    0x24  propagated parent position    0x28  resolved step (or 0.0f)
    0x2C  resolved node value            0x34  dirty byte

The same three-way cascade runs twice, once per direction, at `.Leboot_000BBF08`
and `.Leboot_000BBF70` - **the two copies are identical apart from their field
offsets**, which is the shape of a macro expanded with different arguments.

**The tail is a hand-rolled sparse bitmap.**  `sra $t0, $a1, 5` followed by
`sll $t0, $t0, 2` is `(bit >> 3) * 4` - the *word* index in bytes - and
`andi $a1, $a1, 0x1F` with `sllv $a1, $t1, $a1` is the bit index within that word.
**`0x1F` rather than `0x3F` is 32 bits per word on a 32-bit register**, and the
`1 << bit` is computed with a `sllv` against a register holding 1 rather than a
variable shift.  `lui $a2, (0x80000000 >> 16)` then `or $a1, $a1, $a2` sets
bit 31 of the flags word at 0xC($a0) on every path reaching the tail: **bit 31 is
the dirty bit.**

**This one took four attempts, and the reason is worth recording because the tool
did not help.**  Two of the four failures were me placing `.Leboot_000BC09C` at
the wrong place.  The label's *name* matches the address of a `div.s` in the
earlier clamp block, so the intuitive reading puts it there - and that is 0x1D
instructions from where it belongs, after the bitmap update.  **gas did not
report an undefined or misplaced label; it assembled the branch as a branch to
itself** (`4501ffff`) and the only symptom was one wrong word in 152.

The other two failures were label bookkeeping of my own making: renaming one of
two identically-named labels to `.Leboot_000BC09C_1`, then removing that label
entirely.  **`verify_c.py` caught all four, and the size guard never fired -
these were same-size, one-word differences, which is the case the byte compare
exists for.**  Trusting the splat's label *name* over the address it sits at is
the specific mistake; the fix was to read `tools/disasm_range.py` for the branch
target rather than inferring it.

#### The accessor census: 1,249 functions under 40 bytes, and what they are

Grouping every function of 0x30 bytes or less by its instruction shape gives a
census that is worth more than the individual files.  **1,249 of the 6,925
remaining functions are under 40 bytes**, and they are not miscellaneous - they
are six shapes:

| count | body |
| --- | --- |
| 379 | `jr $ra` / `nop` - void, returns nothing |
| 93 | `jr $ra` / `or $v0, $zero, $zero` |
| 33 | `jr $ra` / `or $v0, $a0, $zero` - return `this` |
| 33 | `jr $ra` / `ori $v0, $zero, 0x1` |
| 13 | `jr $ra` / `lw $v0, 0x24($a0)` |
| 9 | `jr $ra` / `lw $v0, 0x4($a0)` |
| 8 | `jr $ra` / `lw $v0, 0x0($a0)` |
| 6 | `jr $ra` / `lw $v0, 0x1C($a0)` |
| 6 | `jr $ra` / `mtc1 $zero, $f0` |

**So the module's smallest tier is almost entirely accessor and constant-return
boilerplate**, and the 123 single-field accessors are the part that carries
information: each one is a `(operation, offset)` pair, and the offsets are
field numbers.

Twenty-five of them are now transcribed, and **the address spacing separates
two different things** that look the same in the flat list:

* **Identical pairs 8 bytes apart are one inlined getter emitted twice** -
  func_0004E5CC/func_0004E5D4, func_00058078/func_00058080,
  func_00080288/func_00080290.  Three pairs, all at offset 0.
* **Adjacent getters with *different* offsets are one class's accessor
  block** - func_00194AA8, func_00194AB0, func_00194AB8, func_00194AC0 read
  0x00, 0x04, 0x18 and 0x1C, and func_00194AFC four functions later does
  `sw $a1, 0x14($a0)`.

That second group is a class layout: **fields at 0x00, 0x04, 0x18 and 0x1C are
read-only, 0x14 is read-write, and 0x08 to 0x10 are reached by no accessor at
all.**  A getter with no setter beside it is a field the class exposes for
reading only - the closest these eight bytes come to saying what a field is
for.

The 13 at offset 0x24 are the largest single-offset group and they sit in one
consecutive run from 0x000BD5F4 to 0x000BE8B8, then 0x000BEE9C, then a gap to
0x000C1490 and func_000C1498.  **`func_000BEBDC` and `func_000BED6C`, two of the
six +0x1C getters, are inside that run** - which is what makes the spacing a
per-class block rather than a coincidence, and it is why those two 0x1C
getters are the next ones worth writing.

#### Eight more, and the rule from the last pass gets a confirmation

Writing `func_000BEBDC` and `func_000BED6C` settled what the census could only
suggest.  **The run of thirteen 0x24 getters is not thirteen copies of one
accessor** - it is one class's block in which one member happens to have
thirteen getters and the member at 0x1C has two, and those two sit *inside* the
run.  So the block's offsets are 0x1C and 0x24.

The duplicated-pair rule from the previous pass also gets its first test away
from offset 0.  `func_00080584` and `func_0008058C` are identical adjacent
+0x18 getters, and they sit **directly inside another class's accessor block** -
which is what distinguishes "emitted twice" from "a second class that happens to
match".  The second reading is not merely unlikely, it is *unavailable* when the
pair is inside a block: a different class would not be emitted in the middle of
this one's accessors.

**The cleanest instance is the whole block at 0x000804B8-0x000805D4**, which is
six consecutive small functions:

    func_000804B8   bit 3 of 0x4($a0)      flag query
    func_00080514   lw $v0, 0x1C($a0)     getter
    func_0008054C   lw $v0, 0x20($a0)     getter
    func_00080584   lw $v0, 0x18($a0)     getter
    func_0008058C   lw $v0, 0x18($a0)     duplicate
    func_000805D4   addiu $v0, $a0, 0x8   base-pointer accessor

So this class has a flags word at 0x04, words at 0x18, 0x1C and 0x20, and a
sub-object at +8.  **0x20 is the only getter in the module that reads it**, and
`func_000805DC` immediately after the block dereferences 0x2C($s1) + 0x18, so
the object is at least 0x30 bytes with 0x24-0x2C having no accessor.

`func_000804B8` is worth a second look: `andi $v0, $a0, 0x8` followed by
`sltu $v0, $zero, $v0` is **a bitmask test cast to a real boolean** - the same
pair as `func_00112148`, and without the `sltu` the caller would have to know
that "true" is 8.  `tools/flag_accessors.py` already censuses this shape and
reports eight of them over eight (offset, mask, bit) rows, with bit 3 at offset
0x04 being exactly this function.

**One correction to a claim made here earlier.**  A draft of this section said
bit 3 "means the same thing in this class and in `func_000BBE78`'s", on the
strength of both using `andi $a1, $a1, 0x8`.  The census does not support that:
**every row in the accessor census is a different offset and a different bit**,
and the only two that share a bit number are both in the 0x18 halfword of a
different class (bit 20, `func_001A9D54`/`D68`/`D80`).  So `0x8` is one
accessor's mask and `func_000BBE78`'s is another's; **the coincidence is a
shared mask value, not a shared flag meaning**, and the file for
`func_000804B8` was left as it was rather than gaining the claim.

**A file worth keeping as the reference for this shape.**  `func_000804B8` was
already transcribed in a *readable* form - a typed struct and a named return -
rather than the bare `__attribute__((noreturn))` block that is the default
idiom here.  **A rewrite of it to match the surrounding files' style would have
made it worse**, so it was reverted rather than kept.  `func_001A9ABC` and
`func_00112148` are the same boolean-cast shape, and `func_001A9ABC`'s file is
the most explicit about why the cast exists at all.

#### The thirteen 0x24 getters are not a class - it is one type instantiated eleven times

The claim above that the 0x24 run is "one class's accessor block" is **wrong,
and this pass is what showed it.**  Listing what sits *between* consecutive
getters gives the real structure:

    func_000BD5F4  (8)   then 56, 160, 80, 68   before the next getter
    func_000BD768  (8)   then 56, 160, 80, 52
    func_000BD8CC  (8)   then 56, 160, 80, 68
    ...
    func_000C1490  (8)   then  8              (adjacent)
    func_000C1498  (8)

**The 8-byte getter is not a block member at all - it is the first slot of a
five-function group, and there are eleven of those groups.**  The spacing is the
gap between groups, not the length of a block.  `func_000BEBDC` and
`func_000BED6C` are not accessors of a different offset in the middle of a
class's block; **they are the first slots of groups seven and eight.**

What the groups share is a shape, not an object:

    group 1   8, 56, 160, 80, 68
    group 2   8, 56, 160, 80, 52
    group 3   8, 56, 160, 80, 68
    group 4   8, 56, 160, 80, 84
    group 5   8, 56, 160, 80, 240
    group 6   8, 56, 160, 84, 240
    group 7   8, 60, 160, 80, 108, 240
    group 8   8, 60, 160, 80, 108, 240

And the members are near-identical, differing in the global they name:

    func_000BD5FC  vs func_000BD8D4   14 words each, ONE word differs
    func_000BD724  vs func_000BD9FC   17 words each, IDENTICAL

The one word that differs in the 56-byte member is the `addiu` immediate -
`0xAA78` against `0xAC28` - resolving `sym_001EAA78` against `sym_001EAC28`.
The 160-byte member differs in two words: its own global, and one `jal`
target.  **Its other three `lui`s are the same in every group -
`sym_001E9E30`, `sym_001E9478`, `sym_001E9300`.**

**So this is one chunk type instantiated eleven times**, each instance owning a
global for its own data (`sym_001EAA78`, `sym_001EAC28`, `sym_001EAB50`,
`sym_001EAD00`, `sym_001EADD8`, `sym_001EAEB0`, `sym_001EAF88`,
`sym_001EB060`, `sym_001EB138`, ...) and sharing three common ones.  The
repeated `jal`s are the three tag registrations, and the varying tail is the
per-instance work.  **The same reading as the four 180.0f reciprocal caches:
pasted per class, not shared** - and here the paste is eleven deep.

Three members of group one are now transcribed.  `func_000BD5FC` calls
func_000A7A8C and stores `sym_001EAA78` at 0x18 of the object while clearing
0x24 - **which is why the getter at the head of the group reads 0x24, and
the group's own initialiser is what makes that field meaningful.**

`func_000BD724` copies two floats from 0x14 and 0x18 of the object at `0x8($s0)`
to 0xB0 and 0xB4 of the object at `0x24($s0)`.  **The destination object is at
least 0xB8 bytes, which is 0x94 more than the getter's 0x24 offset suggests** -
a concrete instance of a getter saying nothing about the size of what it hands
out.  It also reloads `$s0` in the epilogue even though `$s0` has been dead
since the last `swc1`; omitting that makes the function four bytes short, which
is **a different kind of necessity** from the redundant data reloads beside it,
and both are in the file.

#### The group's fourth member, and a pair of labels that sit between instructions

`func_000BD634` is the member that does the work, and it is the clearest thing
in the cluster: **four globals are stored into the same field, 0x18 of the
object, in sequence** - `sym_001EAA78` (the instance's own), then the three
shared `sym_001E9E30`, `sym_001E9478`, `sym_001E9300`.  Each store overwrites the
last, so **the final store wins and the first three are dead.**  Whether that is
a layout mistake or four registrations through a scratch slot the callee is
expected to consume is not decided by these bytes.

**Three `beqz $s1` branches are all dead, and that is decidable.**  `$s1` holds
`$a0`, and the `beqz $a0` at the top already returned when `$a0` was null - so
every later `beqz $s1` falls through.  Each one sits in a delay slot holding a
useful store, which is what makes them free artefacts of the scheduler rather
than tests.  **A dead branch whose delay slot is load-bearing is the reason
`.set noreorder` matters here**: under `reorder` gas would fill the slot and the
stores would be lost.

`jal func_000B9C88` with `$a1 = 0` is the **base-constructor call** that
`func_0019D4AC`, `func_0019D508` and `func_0019D564` also make, and the
`andi $s0, 0x1` before it is their flag test too - so this group belongs to the
same class family as the three sibling classes.

**This one took four attempts, all from label placement, and the failure mode is
worth recording because it is new.**  `.Leboot_000BD6AC` is the `andi` and
`.Leboot_000BD6B0` is the `beqz` immediately after it - **the two labels belong
*between* two adjacent instructions**, because the compiler wanted that `andi`
on the skipped path and a different use of the same slot on the fall-through.
Putting them the other way round moves every branch by one word and changes
nothing else about the function: same size, same instructions, three wrong words.
Read off the instruction stream, `.Leboot_000BD6AC` looked like it belonged
before the `or $a0, $s1, $zero` two instructions earlier, and that is where it
went first.

**This is the third time a label position, not a label name, has been the whole
problem** - after `.Leboot_000BC09C` and the two `.Leboot_000BC09C_1`
attempts.  The rule that follows from all three: **resolve the target address
from the bytes and count, do not place the label where the surrounding code
looks like it belongs.**  `verify_c.py` reported "3 of 40 words differ" every
time and the size guard never fired.

#### The eleven globals are eleven vtables, and there is a second vtable family

The per-instance globals the groups name turned out to be the key to the whole
structure.  Collecting every pointer the eleven wide members name gives fourteen
globals, and eleven of them are **exactly 0xD8 apart**:

    sym_001EAA78  sym_001EAB50  sym_001EAC28  sym_001EAD00  sym_001EADD8
    sym_001EAEB0  sym_001EAF88  sym_001EB060  sym_001EB138  sym_001EB2E8
    sym_001EB560

`0xD8` = 216 bytes = **27 eight-byte entries**.  Reading each 216-byte record as
a table of pointers settles what it is: **25 of its 27 entries are the same in
all eleven, and every one points into `.text`.**

So the eleven groups are not eleven instances of one type - **they are eleven
sibling classes, and `sym_001EAA78` is the vtable of the first.**  The two
globals `func_000BD634` also names, `sym_001E9E30`, `sym_001E9478` and
`sym_001E9300`, are a *different* set: they sit at irregular spacing (+0x178,
+0x9B8, +0xC48) and each is named by all eleven, so they are shared, not
per-class.

**Slot by slot, the eleven vtables differ in seven entries:**

| slot | distinct | membership |
| --- | --- | --- |
| +0x0C | 11 | each class's own `func_000BD7A8`-shape |
| +0x014 | 11 | each class's own |
| +0x01C | **5** | 7 leave the default, 4 override, no two of the 4 shared |
| +0x034 | 11 | each class's own |
| +0x04C | 2 | **10** leave the void default, **1** implements it |
| +0x064 | 2 | **10** return `this`, **1** returns 0 |
| +0x0D4 | 10 | ten of eleven set it |

and the other 19 are identical across all eleven.

**The membership counts turn this from a slot census into a taxonomy of the
family, and the answer is 7 / 3 / 1.**  Seven of the eleven classes
(`sym_001EAA78`, `001EAB50`, `001EAC28`, `001EAD00`, `001EADD8`, `001EAEB0`,
`001EB2E8`) **override nothing at all** - they take every inherited pointer.
Three (`sym_001EAF88`, `001EB060`, `001EB138`) **override slot +0x01C and
nothing else.**  One, `sym_001EB560`, **overrides all three** of +0x01C,
+0x04C and +0x064.

So this is one interface with three levels of engagement among its
implementors, and `sym_001EB560` is the only class in the family that is not
merely re-declaring it.  The two slots I had called "the only binary choices"
are in fact **10-against-1 both times, and the same record is on the minority
side in each**.

**Three of the four bodies at +0x01C contain a branch that can never be
taken.**  They call `func_000B9D34` - the default that is
`ori $v0, $zero, 0x1` and nothing else - and then test `beqz $v0` on the
result.  It is never zero, so the label each branch targets is unreachable,
and in `func_000BE4D4` and `func_000BE75C` that label is the path returning
0.  Only `func_000BFEB8` has a real test in it, and it turns out to
**return 1 on every path anyway** - `$s1` is set to 1 before anything
happens and every exit writes it to `$v0`, including the early-out when
`0x24($a0)` is null, which calls nothing at all.  **Two independent copies
of the same dead branch 0x280 bytes apart is what makes that a property
of the source rather than a coincidence.**

**So the +0x01C slot is not four behaviours of one question.**  Three of
the four overrides are `return 1` with extra work; `func_000BFEB8` is
`return 1` with different extra work; and only `func_000BE138` can answer
0.  **One of four overrides changes the answer, and the other three exist
to do side work on the way to the same constant.**  Six of the eleven
classes get that constant from a two-instruction stub, so the cheapest
implementation of this method is also the common one.

`func_000BE138` is the only override with a loop, and the loop is what
makes it a *real* implementation rather than a wrapper: it visits
`0x28($s4)` four-byte entries of the array at `0x2C($s4)`, calling
`func_000BA9FC`, a thunk, and `func_000C42C8` on each.  **The loop cannot
change the answer** - both its exits converge on `ori $v0, $zero, 0x1` -
only the amount of work.

`func_000BFEB8` has **six instructions in the middle that compute
nothing**: they build an index and an offset into `$a2` and `$a1`, and at
the join `.Leboot_000BFF04` the code reloads `$a0` from `$s0` and the
next `jal`'s delay slot overwrites `$a1` with `$v0`.  Neither register is
read again.  **The `bne` in the middle does branch and both destinations
arrive at the same place, so what it distinguishes makes no difference to
the result** - a test with real instructions on both arms and no
consequence on either.

`func_000BE4D4` and `func_000BE75C` are 25 of 27 words identical; the only
difference is two `ori $a1, $zero, <imm>` immediates, `0x7, 0x8` against
`0x8, 0x9`.  One body, two consecutive argument pairs through
`func_000BA26C`.  **Every function this family's vtables name at those
four slots is now promoted** - the default, both binary variants, and all
four overrides.

#### The thunk idiom turns up a third time, inlined

Inside `func_000BE138`'s loop there is the module's third instance of the
multiple-inheritance adjustment, and **the first one the compiler inlined
rather than calling a helper for**:

    lw    $a0, 0x18($v0)
    addiu $a0, $a0, 0xD0
    lh    $a1, 0x0($a0)      <- signed half-word
    lw    $a2, 0x4($a0)      <- function pointer
    jalr  $a2
      addu  $a0, $v0, $a1

Both ends of this idiom were already on record: `func_0019D11C` reads a
`{i16 adjust; void (*fn)()}` entry and adjusts `this` before its `jalr`,
and `func_000805D4` returns `this + 8` to hand a sub-object back to the
caller.  **Inlining the dispatch is what makes the layout visible
directly** - that the adjustment is a half-word and the entry is 8 bytes
are both readable here and were inferred there.  It also pins the thunk
array at **+0xD0** of whatever `0x18` points to.

**A correction worth recording, because the error was structural rather than
arithmetic.**  I first put the +0x01C split at 5-6 and the two binary slots
at an even split.  Both were wrong.  The 5 was a count of *distinct values*,
not of how many classes use the majority one, and reading it as the latter
turned a taxonomy into a flat five-way choice; the "2 distinct" cells hide a
10-against-1 split completely.  **A cell that records only how many distinct
values a slot has cannot support any claim about how the classes are
distributed - the membership list has to be printed.**

#### The +0x0C slot is a template, and ten of the eleven are the same function

Slot +0x0C is the constructor, and it turned out to be the cheapest slot
to close: **all eleven are now byte-exact.**

Nine of them are **38 of 40 words identical to `func_000BD634`**.  Two
words differ, and they are always the same two:

| word | what varies |
| --- | --- |
| 8 | the vtable installed - one `addiu $a0, $a0, %lo(sym_...)` |
| 12 | the callee - one `jal func_...` |

Everything else - four globals stored into field 0x18 with three of them
dead, three `beqz $s1` that never branch, the base-constructor call to
`func_000B9C88`, the bit-0 flag test, the two labels sitting between the
`andi` and the `beqz` around it - is byte-for-byte the same in all ten.
**That is a template expanded per class, and the eleven vtables are the
only thing that varies.**

The callees run strictly upward in vtable order, `func_0019E490` through
`func_0019F058`, **at irregular spacing from 0xE0 to 0x1D4 bytes** across
ten gaps totalling 0xBC8.  The ordering is a fact about the linker, but
it means the eleven constructors' callees form a contiguous run with
nothing interleaved.

**The eleventh is the template with two of the four stores deleted** -
32 words instead of 40.  `func_000BFBAC` drops the `sym_001E9E30` and
`sym_001E9478` stores and their two dead branches, leaving only
`sym_001E9300`, so its chain is two stores rather than four and its two
labels are where the other ten have three.  **The dead stores went from
three to one because two were deleted, not because the survivor started
mattering** - the last store wins either way, so this class behaves
identically to the other ten.

`func_000BFBAC` is also the class that overrides all three of +0x01C,
+0x04C and +0x064.  **The shortest constructor and the extra overrides
sit in the same record, and that is suggestive but not evidence** - dead
stores in a constructor and live entries in a vtable are unrelated code.
What *is* decidable is that this one class also answers 0 at +0x064
where the other ten return their argument.

**One mechanical note, because it cost a retry and the fix is the same
trap as `func_000BD634`'s.**  The first attempt at `func_000BD7A8` put
`.Leboot_000BD820` before the `andi` instead of after the `jal`'s delay
slot, and the build came back **156 bytes instead of 160** - the size
guard fired before the word comparison did.  That is the size guard
doing its job: a misplaced label between a `jal` and its delay slot
deletes a word.  Placing the two labels around the `andi`/`beqz` pair as
the other nine do fixed it on the second attempt, and the remaining
eight matched first try each.

#### Slot +0x014 varies in one word, and its eleventh member is not a template at all

Slot +0x014 is closed too - all eleven byte-exact.  **It is a narrower
template than +0x0C: nine of the ten siblings differ from the first in
exactly one instruction**, where the constructors needed two.

| | +0x0C | +0x014 |
| --- | --- | --- |
| variable words | 2 (vtable, callee) | 1 (callee only) |
| template size | 40 words | 20 words |

There is no per-class vtable pointer in this slot, only the per-class
callee.  The template asks `func_000A7AC8` whether to proceed, returns 0
early if it says no, otherwise calls the per-class function on
`0x18($s0)`, stores the count into `0x24($s1)`, and returns it cast to a
boolean with `sltu $v0, $zero, $v0` - **the caller gets "did anything
happen", not the count.**

**The tenth member, `func_000BDEC4`, is 84 bytes - one word longer -
and the word buys a second argument.**  In the other nine the `jal`'s
delay slot holds `lw $a0, 0x18($s0)` and the callee takes one argument;
here that load is hoisted above the `jal` to free the slot, and it
carries `or $a1, $s0, $zero`.  So `func_0019EA2C` receives the caller's
second argument too.  **Its callee is also not in the sequence the other
ten form** - they call `func_0019E4F8`, `5F8`, `6EC`, `7E8`, `8F0`,
`EB80`, `ED04`, `EE0C`, `EFE0` in order, and `func_0019EA2C` is
adjacent to none of them.  This is a real difference in calling
convention, not padding.

**The eleventh, `func_000BFC2C`, is 652 bytes and shares only the
opening.**  It asks the same question, but with **inverted polarity**:
the other ten use `bnez $v0` on `func_000A7AC8`'s answer and this uses
`beqz`.  So the two conventions disagree about what a non-zero answer
means.  Either the callee's result is being interpreted differently here
or the two sites were written against different helpers sharing a name;
**the bytes cannot say which.**

Its body is an **inlined 16-bit `memmove`**.  Every copy step is `lhu` /
`sh` with both pointers advancing by 2, and every length is halved with

    sra  $a3, $a2, 1
    srl  $a3, $a3, 31
    addu $a2, $a2, $a3
    sra  $a2, $a2, 1

which is **signed** division by two - an arithmetic shift plus a borrow
from the sign bit, not the `srl $a2, $a2, 1` an unsigned count would
use.  Three sites do it this way.  The overlap test is a three-word
pointer comparison (`xor`, `sltiu $a1, $a1, 0x1`, `andi $a1, $a1, 0xFF`),
and skipping it skips the whole second phase - **so the expensive half
only runs when a copy actually happened.**  The limit is
`0x28(base) + count` advanced by `0x24(base) * 2`, which **fixes the
layout of the object at `0x14($s0)`: an element count at 0x24 and a base
pointer at 0x28, both 16-bit-strided.**

**Two calls to `func_00170654(0, 0)`, both results discarded.**  Before
each call the code does `sb $zero` to a stack slot and `lb`s it straight
back into `$a0` and `$a1` - the CodeWarrior idiom for materialising a
default-constructed `bool` - so both arguments are provably zero at the
call.  Each return is stored, read back with `lb`, and written to a slot
that **is never read again.**  The calls are made for effect and their
answers dropped.

**With +0x0C and +0x014 closed, five of the seven varying slots are now
fully read.**  Only +0x034 remains, and it is the one slot with no
template: its ten pending members run from 52 to 384 bytes and no two
of them share a shape.

**A correction, and it is the second time this family has caught me
conflating two neighbours.**  I read the survey output and gave
`func_000BDCA4` a size of 0x54 with a paragraph about padding and a
merged relocation.  It is 0x50 like its nine siblings; the 84-byte
function is `func_000BDEC4`, two entries down the same column.  The
paragraph was wrong about the size, the uniqueness and the padding, and
`verify_c.py` matched the file as written - **because the file was right
and only the prose was wrong.**  A tool cannot catch that, so the rule
is the one already in this report: read the recorded size from the
splat, not off a table of neighbours.

#### Slot +0x034 is a record copy, and the destination is far larger than the getter suggests

Slot +0x034 is the last of the seven varying slots.  Five of its eleven
members are byte-exact so far; the remaining four run from 212 to 384
bytes and are the two `updateNodeGraph` pairs.

**The slot's common shape is a hand-unrolled record copy**, four words
per field:

    lw    $a0, 0x8($s0)      the source
    lw    $a1, 0x24($s0)     the destination
    lwc1  $f12, <src>($a0)
    swc1  $f12, <dst>($a1)

so **a member's length is `9 + 4 * (number of fields)`**: 13 words for
one, 17 for two, 21 for three.  `func_000BD898` copies one float,
`func_000BD9FC` two, `func_000BDB70` three, and **`func_000BD9FC` is
identical to `func_000BD724` word for word** - the third member of the
slot duplicates the first.

**`func_000BDB70` reads its source fields out of order**: the loads are
`0x1C`, `0x14`, `0x18` while the stores are `0xB0`, `0xB4`, `0xB8` in
sequence.  So this one was written by hand in a different order than the
two-field members and the compiler preserved it - **and it is why the
slot cannot be called a copy loop.**

**`func_000BDCF4` and `func_000BDF18` differ in six immediates and all
six move the same way: every destination offset is 4 higher in the
second.**  Same three 12-byte blocks at `0x18`/`0x24`/`0x30` going to
`0xF8`/`0x104`/`0x110`, same two-word copy, same float, same
instruction for instruction everywhere else.  **The two classes store an
identical record layout one word apart in the destination** - whether
that is a differing header or two consecutive array elements is not
decidable from the offsets.

**Both contain a clamp, and the `l` in `bc1tl` is the whole of it.**
`$f12` is loaded with `mtc1 $zero`, so it is 0.0f; `c.le.s $f13, $f12`
asks whether the loaded value is at most zero; and `bc1tl` is the
*nullifying* branch, so:

    f13 <= 0.0  ->  branch taken,  delay slot nullified, $f13 unchanged
    f13 >  0.0  ->  branch clear,  delay slot runs,     $f13 = 0.0

**So the stored value is `min(x, 0.0f)` - a clamp from above.**  Written
as plain `bc1t` the same source would clamp from below, and the nullify
bit is the only difference between the two.

**The size of the object at `0x24($s0)` keeps growing, and this is now
measurable three ways from this one slot:**

| member | highest offset written | object is at least |
| --- | --- | --- |
| `func_000BDB70` | 0xB8 | 0xBC bytes |
| `func_000BDCF4` | 0x128 | 0x12C bytes |
| `func_000BDF18` | 0x12C | 0x130 bytes |

**0x130 bytes is 0x10C past the 0x24 offset of the getter that returns
it.**  The earlier note that `func_000BD724` implies 0xB8 was right but
was the smallest of these; the same field is now pinned at more than
three times that by its other users.  **A 0x24 accessor in this class
family describes nothing whatsoever about the size of what it hands
out** - and this is the fourth independent function in the cluster to
disagree with that getter about the size.

**There is a second vtable family, and it has the same shape.**  The three
`sym_001EA3E8` / `sym_001EA4B8` / `sym_001EA588` records found earlier are
26 slots at a 0xD0 stride with two differing entries; these eleven are 27
slots at a **0xD8** stride with seven.  **Both put a constructor at slot
+0x0C** - `func_0019D4AC`-shaped in one family, `func_000BD7A8`-shaped in the
other - and both are built out of the same run of `func_0018F6xx` default
stubs, so the two families share a base class.  **Slot +0x0C is the
constructor in every vtable found in this module so far**, and
`func_000BD634` - the function transcribed two passes ago - is itself one of
these constructors.

`func_00198140` settles that relationship on its own.  It is slot **+0x60**
of the three-record family, where it is invariant across all three classes,
and slot **+0x064** of the eleven, where that same slot index **splits**.
Both are the twelfth slot.  **A slot that is invariant in one family and
variable in the other is a virtual the first declined to override and the
second did**, in some of its leaves - the flat-inheritance signature, and it
is visible without resolving a single base-class pointer.

#### `addiu $v0, $a0, 0x8` is the other half of the thunk idiom

`func_000805D4` returns `this + 8` and is **not** a field getter - it hands back
a pointer into the object so the caller can treat the sub-object as an object
and call accessors on it.  The counterpart is `func_0019D11C`, which reads a
*signed half-word* adjustment out of a thunk and adds it to `this` before a
`jalr`.  **Between them the two ends of the multiple-inheritance adjustment are
both visible in the module: one hard-coded at a compile-time offset, one computed
per-object from a vtable entry.**  Because `func_000805D4`'s offset is a
constant, its sub-object sits at a fixed place in the class rather than at a
position that varies - which is the difference between single inheritance with
a base class and a genuine multiple-inheritance thunk.

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
tools/find_loops.py          find the functions with a backward branch, smallest first
tools/check_symbols.py       every C-linked function reports the size it should
tools/nid_table.py           the PSP import table: stubs, libraries, NID words
tools/flag_table.py          decode the packed flag bytes and list their readers
tools/flag_accessors.py      census the C++ boolean accessors: mask, offset, direction
tools/check_report.py        names the report writes about that have no src/eboot file
tools/stride_table.py        addresses several functions materialise; code or constant
tools/code_writers.py       functions that name an address inside the code section
tools/vfpu_split_store.py  staggered vector stores: svr.q with svl.q
tools/spill_frame.py         float functions that move values through the frame
tools/branch_load.py         branch-likely delay slots: loads, or any register write
tools/boolean_shapes.py     dead zero-test masks, and immediate-width flag setters
tools/base_pointer.py      forming an offset once, then a run of loads or stores
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

**The module's symbol table is *not* stripped.**  That is the correction, and it is
large enough to be worth having found this late rather than not at all.  3,882 of the
15,977 symbols in `config/eboot.symbol_addrs.txt` carry a name from the original
CodeWarrior link; only the other 12,095 are names this project invented.  Every claim
below that started from "names are lost" was wrong in the same direction.

`tools/orig_names.py` counts them and classifies by section:

| section | surviving names |
| --- | --- |
| `.rodata` | 3,532 |
| code sections | 350 |
| `.data` and elsewhere | the rest |

**The suffixes are uniquifiers, not part of the names.**  1,295 names end in `_NNNN`
where `NNNN` is a hex VRAM address, because two objects with the same C identifier get
different addresses and PSPLINK disambiguated them that way.  `sortAndCullScene_1078`
and `sortAndCullScene_1844` are one function name linked twice.

**Why only some survive.**  A name is kept when something *outside the same translation
unit* refers to the symbol.  The game was built with `-ffunction-sections`, so each TU
became its own output section - which is where the names `collision`, `drawing`,
`renderCommon` in `config/eboot.splat.yaml` came from in the first place.  **So the
named functions are each TU's exported surface**, and the 7,000-odd unnamed ones are
functions only their own file calls.  That predicts the surviving names should be the
interesting ones, and they are.

**Function names: eleven distinct, not many.**  `orig_names.py --code` collapses to
just 11 once the suffixes go:

| count | name |
| --- | --- |
| 223 | `stub` - every PSP import stub, so the 223 are one name and the NID table is still needed |
| 28 | `updateNodeGraph` |
| 21 | `sortAndCullScene` |
| 21 | `syncSkeleton` |
| 16 | `renderMeshInstances` |
| 15 | `collision` |
| 14 | `drawing` |
| 9 | `renderCommon` |
| 1 each | `elem_register_chunk_tag`, `elem_operator_new`, `elem_throw_bad_alloc` |

So the naming work is **transcription for about a dozen functions and pattern
recognition for the rest**, not pattern recognition throughout.  `sortAndCullScene_1078`
at `0x1B4C94` is a real example: the shape queue has had it sitting labelled for
several iterations.

The three `elem_` names are the exception that proves the suffix rule is not
arbitrary - they carry the source path as a prefix, which is the next item.

**Some strings are the project's own source paths, and they are readable.**  Not
guessed at from the symbol name either - stored at `0x001C8F08` is the literal

```
c:/ad_clean/sims_psp/src/elem/bent/circular.h
```

and at `0x001C95E4`

```
C:/ad/sims_psp/testing/luaDumps
```

**So the build tree's root is `c:/ad/sims_psp/`, with `src/` below it and a
`testing/` directory beside it**; `ad_clean` is a second root, presumably a clean
checkout used for some builds.  `src/elem/` matches the `elem_register_chunk_tag`
constructor and the `elem_` symbols, so `elem` is a real subsystem directory and not a
prefix someone invented.

Getting this wrong twice is worth recording.  The *symbol name* flattens the path -
`str_c_ad_clean_sims_psp_src_elem_bent_circular_h` - and reading that as a path with
`/c/AD/clean/` in it was wrong: `AD` is lowercase, `ad_clean` is one directory, and
the drive letter is `c:` not `/c`.  And the first version of `rodata_names.py
--paths` matched on word count rather than on the separator, and reported **221
"paths"** when most were ordinary error messages.  The separator is the test; a word
count is not.

**182 strings contain a path separator, and most of them are asset paths** - which is
a naming scheme in its own right.  `characters/bodyanims/npctextures/%s/%s.tif`,
`face/afface/afface-s%d`, `hair/afhairbald/ufhairbald-skin-s%d`,
`body/afbodynaked/afbodynaked-nude-s%d`.  **The `af`/`am` prefix is sex** - adult
female, adult male - applied to every gendered asset: `afbodynaked`, `amhairbald`,
`afface`, `amface`.  `ms0:/elem_log.txt` is the only PSP-path string, and it matches
the `elem` subsystem.

That is the project's source layout and asset layout, read out of the binary - worth
more than any number of transcribed functions, and neither could be guessed from the
code.

**And the strings themselves are readable and verifiable.**  3,532 of 3,532 names
agree with the bytes at their address once case and punctuation are squashed - no
exceptions, nothing unreadable.  `rodata_names.py` checks that, so the naming is a
cross-check on the reading rather than the only way to get the text.

Three sources have proved worth reading, now that the first one is confirmed as the
original's own:

**String literals.**  3,754 NUL-terminated ASCII strings survive in `.rodata`
and every one is a relocation target, so each has a known address.  They are
named `str_<text>` and the generated asm now reads `str_BoidBehavior_avoidWalls`
instead of `%hi(sym_001BF8A0)`.  They are largely the game's tuning and
behaviour parameters, so they also document the subsystems.

**51 of them are C++ qualified names, over 46 classes**, and they are the single
best description of the engine's shape that exists anywhere in this project
(`rodata_names.py --classes`):

```
BoidBehavior::avoidWalls             nav::findIntersection
WendToPointBehavior::onUpdate        nav::findIntersectionWithCell
NavigateAndWendToSeat::onUpdate      nav::testLineSegmentWithPlane
StartDecisionMaker::onUpdate         SharedResourceFile::find / request / waitForLoad
StopDecisionMaker::onUpdate          InteractionManager::BehaviorLuaTask::updateTask
Chase::onUpdate                      ResumeLuaTask::onImmediateUnload
CameraBasicFollowSims2::onUpdate     NetworkMgr::updateTask
CameraInteractionSims2::onUpdate     volatileMem::lock
EffectWidget::setProperty %s         HeadshotTextureWidget::setProperty
```

Three things fall out of that list.  **The game is C++, not C** - every one of these
is a `Class::method`.  **The AI is behaviour-based with a scheduler**, because roughly
thirty separate classes expose `onUpdate` and nothing else, which is a template method
pattern driven from one place.  **And the scripting is Lua**: `BehaviorLuaTask`,
`ResumeLuaTask`, `NetworkMgr::updateTask`, plus a whole Lua surface elsewhere -
`lua_yield`, `LuaBreakPoint`, `luadump`, `-ignoreluafiles`, `SCHED_LUA`.  There is a
Lua debugger in the shipped build.

Which brings the [guard](#the-guard-a-nested-re-entrant-setjmp-sandbox-with-a-four-value-error-code)
back into the picture.  `setjmp`/`longjmp` with a small closed set of reason codes and
a nesting stack is precisely how `lua_pcall` is implemented, and the guarded runner's
whole purpose - running something that cannot report failure in band - is what a
protected call is for.  **That is a hypothesis with strong evidence, not a
conclusion**: nothing read so far has shown a Lua state object being passed to the
guard, and the abort sites carry codes 1, 3, 4 and 5 rather than anything that could be
matched to a Lua error value.  It is worth one more look, because if it holds then the
guard is not an engine mechanism at all but Lua's, and the eight abort sites are eight
`error()` calls.

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

1. Keep working down `tools/c_shapes.py --done`.  255 real-shape functions were
   identified and 378 are done.  Each shape that works yields several functions
   at once, and the established rules ("load in asm, store in C", "leave an
   overwritten register uninitialised", "reopen `.set noreorder` at a label")
   keep the per-function cost down.  83 of the remaining 95 are in shapes of one
   or two functions, so this is now mostly one-function-at-a-time work rather
   than shape work; the shapes are exhausted as a source of leverage.
2. The work is hand transcription, deliberately.  A generator *can* emit all
   7,497 function bodies as verbatim asm and does - it was built and measured,
   and it verified 7,497/7,497 - but that is a transcription, not a
   decompilation, and it answers a question this project is not asking.  The
   readable layer is the point, and each sequence needs a person to work out
   what it does.  `tools/gen_copy_asm.py` stays as a helper for the sixteen-word
   copies, where the pattern is long enough to be error-prone but still has to be
   understood.
2. **All three kinds of control flow now work, and all three are byte-exact in one
   function.**  Branches (`func_001A9CF8`, five of them, two of them coprocessor
   branches), unconditional loops (`func_001428E4`) and counted loops with the
   cursor advanced in the branch's delay slot (`func_000CD5B0`, `func_0009C9B4`)
   are all done.  See *Branches work* and *Loops work* above for the machinery.
   `func_001A9CF8` is also where the label/`noreorder` interaction turned up - see
   *A `noreorder` region that spans a label grows the function by four bytes*,
   because the four extra bytes it produced were outside the symbol's own size
   and would have passed any check that only compared words.
   **`func_00143A18` is byte-exact too**, which retires the last function that
   was left undone on the grounds that nobody could say what its source was.  It is
   a hand-rolled `strstr`: two loops, four forward branches and two backward ones.
   See *`func_00143A18` is `strstr`, and the open question about it is closed* above
   for what it turned out to be and why the earlier reading of it was wrong.
   **`func_00143A18` is now byte-exact too**, which retires the last function that
   was left undone on the grounds that nobody could say what its source was.  It is
   a hand-rolled `strstr`: two loops, four forward branches and two backward ones.
   See *`func_00143A18` is `strstr`, and the open question about it is closed* above
   for what it turned out to be and why the earlier reading of it was wrong.
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
5. ~~The packed flag bytes at `0x001E1B98` are worth mapping: the one-bit accessors
   over them will name the individual booleans.~~  **Done, and the framing was
   wrong** - they are not booleans.  `tools/flag_table.py` decodes all 128 entries
   and finds 20 readers; the three that mask ask only for bits 0-3, and
   `flags[i] & 0x07` can only ever be 0, 1, 2 or 4.  So `func_00140A58` returns a
   **four-state property**.  See *The packed flag bytes are four-state properties*.
   What is left: bits 4, 5, 6 and 7 are set on 32, 32, 12 and 1 entries, and the
   `--asked` census shows **bit 7 is read by nobody at all**.  Bits 4 and 6 are read
   only by `func_0010CFC0`, which masks nearly every bit in turn and looks like a
   serialiser or debug dump rather than a property test.  So those bits are set by
   the table and emitted by the dumper, and nothing in the shipped game consults
   them.  **What the index means is also open:** it is not ASCII (`--domain` tests
   both alignments and both fail), and the "one per object class" idea this item
   started from has been withdrawn as a guess with no evidence behind it.
6. **Use the stack census to choose what to read next.**  `tools/delay_slots.py`
   says 5,788 functions have a frame; the 88 distinct sizes are a free bound on
   each one's local count.  Starting from the large frames would find the
   interesting functions far faster than going by address order, which is how the
   first 78 were found.
7. Recover vtables in `.rodata` and give them `ClassName_methods[]` names.  The
   three accessors at `0x001DB014`, `0x001DB050` and `0x001DB0AC` are the
   start of this, and the controllers that register `Start` and
   `ActiveController` should key off them.
8. ~~Read `.rodata.sceNid` for the import list and name the PSP API stubs.~~
   **Done, and the answer is that it cannot be finished from this side.**  All 223
   stubs are empty `jr $ra` placeholders patched at load time; the tool records the
   index/address/library/NID correspondence so far; naming the individual imports
   needs an external NID table this project does not have.  See *The PSP import
   table* above.
9. Name the 185 constructors that only touch the shared runtime, using the
   functions they call.
