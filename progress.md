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
| functions written in C | 404 (see below) |
| **C functions that byte-match** | **383** (linked from `src/`) |
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
