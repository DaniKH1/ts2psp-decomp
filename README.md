# The Sims 2 PSP — decompilation

A byte-exact reconstruction of the PSP version of *The Sims 2*, recovered from
`EBOOT.BIN` with no symbols and no source.

The module is 32-bit MIPS for the Allegrex (the PSP's CPU), linked at address 0
and relocated by the loader.  Every address in this repository is module
relative.

| | |
| --- | --- |
| module image | **2,030,864 / 2,030,864 bytes identical** |
| code | **507,673 / 507,673 instructions identical** |
| functions recovered | 7,497 |
| relocations recovered | 66,503 / 66,503 |
| C functions byte-exact and hand written | 607 |

The engine is Maxis' "Elem", the shared engine also used by *The Sims 3* — the
build path baked into the binary is `c:/ad_clean/sims_psp/src/elem/…`.

## What is here

| directory | |
| --- | --- |
| `asm/` | one `.s` per function, as splat produced them.  Assembling and linking this reproduces the original exactly.  Recovered material — see [Legal](#legal). |
| `src/eboot/` | the hand written decompilation: what each function does, in C.  Every file here is verified by `tools/verify_c.py` to compile to the original bytes - there is no unverified attempt left in the tree.  The exact count is `wc -l config/matched_c.txt`, which is generated, and this sentence deliberately carries no number so it cannot go stale.  Recovered material - see [Legal](#legal). |
| `include/` | the shared types: `f32`, `Vec3f`, and the object layouts the early functions reveal. |
| `tools/` | everything that produced the above, and the checks that keep it honest.  MIT licensed. |
| `config/` | the recovered symbol map, the relocation map, and hand recovered names. |
| `assets/` | the data sections, byte for byte. |
| `progress.md` | **the report**: what is known, how it was found, what is still open.  Start here. |

## Two layers, and why the distinction matters

**The assembly is byte-exact and complete.**  `asm/` reproduces every byte of
the module image.  That part is finished and has been for some time.

**The C is the readable layer, and it is partial.**  607 of 7,497 functions.  The
original was built with CodeWarrior for PSP (`mwccpsp.exe`), and psp-gcc does
not reproduce its instruction selection.  Reaching byte-exact C for *all* 7,497
needs that proprietary compiler; 607 were reached without it, by pinning the
handful of registers and delay slots where the two compilers disagree.

What those 607 have in common is that the disagreement is small enough to name.
Both compilers emit the same *instructions* and differ over which register is the
destination, or when a value is loaded, so pinning the registers makes them agree.
The pinning is deliberately narrow: an inline `asm` block over the two or three
instructions that differ, with the rest left to C, so the source still says what
the function does.

Three results from that work are worth knowing before reading the C:

* **psp-gcc is not a weaker compiler, it is a differently opinionated one.**  It
  rewrites expressions into fewer instructions than CodeWarrior does: `(link + 1)
  & ~1` becomes a single Allegrex `ins`, and `~(limit - 1)` becomes a single
  `negu`.  Byte-exactness is as much about stopping its optimisations as about
  fixing its register choices.
* **Control flow is solved, all three kinds.**  A conditional (`func_000E8EA8`),
  an unconditional loop (`func_001428E4`) and counted loops whose cursor advances
  in the branch's delay slot (`func_000CD5B0`, `func_0009C9B4`) are all byte-exact,
  and so is `func_001A9CF8`, which has five branches in one block — two of them
  coprocessor branches, and two paths that merge onto the same `and` with different
  masks already in the same register.  A conditional with two long sides, where the
  compiler has a real block-order choice, is still untested.
* **CodeWarrior peels a loop's first test out and leaves it above the loop's own
  setup.**  `func_0014402C` — a `strlen` — branches to its return *before* the
  instruction that sets the register the return subtracts, so an empty string would
  return nonsense.  The identical shape is in a second function elsewhere in the
  binary, so it is a compiler habit rather than a one-off, and the reading that fits
  is that no caller passes an empty string.  Worth knowing before reading any loop
  here: a guard sitting above the loop's prologue is not guarding it.

* **The engine has a `setjmp`-based guard for calls that cannot report failure in
  band.**  `func_00140AC4` and `func_00140B28` are `setjmp` and `longjmp` — they save
  and restore exactly the registers o32 says a callee must preserve, and nothing else.
  Both are byte-exact, and the `longjmp` half is where the register rules bite: its
  first attempt listed `$s0`–`$s7` and `$f20`–`$f31` as clobbered, and psp-gcc emitted
  a prologue *saving* them — the mirror image of the `$sp` rule, where listing `$sp`
  makes it emit a frame.  Both push a prologue in front of a block meant to have none.
  `func_001129E0` installs a handler record on an object and runs a **function
  pointer** under it; if anything calls `func_0011296C` the record's reason code is
  written into that frame's own return slot and control jumps back out, so the caller
  gets an error code where a call could not have returned one.  The codes are a closed
  set of four — 1, 3, 4 and 5, with 5 the common one (`tools/abort_codes.py`) — and
  handlers nest: an abort with nothing installed is forwarded to the enclosing one.
  **Read the callers as error checks, not as ordinary calls.**
* **The vector coprocessor is used by 32 functions and nothing outside rendering and
  skeletons.**  `tools/vector_unit.py` is the census: 542 vector instructions in all,
  the largest users being `renderMeshInstances_122C`, `syncSkeleton_0FDC`,
  `drawing_0C04` and four functions in `renderCommon`.  Eight uses of `vrsq.s` are
  reciprocal square roots — distance attenuation in a lighting calculation, four
  vertices at a time — and `svl.q`/`svr.q` plus six `vmmul.q` say matrices are being
  transposed and multiplied.  These functions have **no C spelling at all**:
  psp-gcc has no `float4` and no operator that lowers to `lv.q`, so
  `src/eboot/syncSkeleton_27D0.c` and `_2808.c` are the machine code with the
  decompilation in the comment, and that departure is recorded rather than disguised.
* **Nine of the module's functions are C++ boolean accessors, and only one bit kept
  all three of them.**  `tools/flag_accessors.py` is the census: a flags word at
  offset 0x18, a `float` at 0x1C whose -1.0f is a sentinel, and accessors for bits
  17, 18 and 20 of that word.  Bit 20 has a getter, a setter and a clearer
  twenty bytes apart; bit 17 kept only its getter and bit 18 only its setter — which
  is about which call sites inlined the accessor, not about the class.  One of the
  nine, `func_001A9CBC`, **reads `$f12` without writing it**: on this ABI that is the
  first floating-point argument, so the function assigns a parameter to the field and
  its `lui`/`mtc1` sentinel pair is simply missing.  The query beside them,
  `func_001A9CF8`, is a three-way test over bit 13, the sentinel float and bit 12 —
  because each of the four combinations of two booleans has its own accessor.
* **The module writes its own display geometry into memory at startup.**
  `func_0014EBA4` fills four identical 0xFC-byte records at 0x6C770, and each has
  **480 by 272** in it — the PSP framebuffer, in two fields a word apart.  What the
  engine calls those records is not established; the 4 and the 0xFFFF beside them
  look like sentinels saying "not set yet".  It is also the only static constructor
  here where the *record stride* is the interesting part: two cursors step in
  parallel, one 0xF8 behind the other, so the second one's store lands on the last
  word of the same record rather than the first word of the next.
* **One packer has a channel that is dead for every input.**  `func_000706A8` masks
  to eight bits, shifts right 19 — which empties the word — and shifts the zero back
  by ten.  The result is used; it is always zero.  Every other piece of dead code
  found so far computes something that *is* used, so this one is the clearest
  instance yet of byte-exact and correct being different questions.
* **The module uses branch-likely to make a delay slot conditional, and 1,173
  functions do it.**  `tools/branch_load.py` counts 38,336 `beq`/`bne`/`beql`/`bnel`,
  of which 3,214 are the likely forms — 8.4 % — and 1,173 functions put *any register
  write* in a likely branch's delay slot, 311 of them writing a literal into the
  destination first.  That is "conditionally materialise a value, defaulting to a
  constant": `func_0000E04C` guards `addiu $s1, $a0, 0x8`, so `s1 = node ? node + 8 :
  NULL`, and `func_0000EBDC` does the whole thing three times into three registers in
  one function.  **The likely form is the whole trick** — the slot runs only when the
  branch is taken, so this costs one branch and no label — and it being *uncommon* is
  what makes it read as deliberate rather than as a scheduling accident.  What the
  sentinel means is still not established.
* **A tool's rule said "always `lui`, because these are all bits above 15", and had no
  test at bit 15.**  `func_001A9ABC` masks `0x8000` with `andi` — the one bit an
  `andi` immediate can still reach — and sixteen bytes away `func_001A9ACC` masks
  `0x20000` with `lui`, on the same word, with nothing else differing.  Bit 15 is what
  separates "the compiler prefers `lui`" from "the immediate stops at 16 bits", so the
  missing boundary case was the only thing standing between a rule and two rules.  It
  turned up because the function was transcribed by hand and the tool built to find it
  did not.  Two bugs were behind it: the immediate-mask fallback tested opcode `0x0D`
  (`ori`) instead of `0x0C` (`andi`), and the length filter demanded five instructions
  when an `andi`-masked getter has no `lui` and is four.
* **A comment in this repo argued from a missing name to a real fact, and the census
  caught it.**  `updateNodeGraph_0E48` is a fifth member of the four-member
  three-float copy family; the first draft claimed the other four shared a translation
  unit with each other and not with it, because their names are `func_`-prefixed
  placeholders.  But a missing symbol name is not evidence about a header, and
  `asm/eboot/` holds one `.s` per function so the build cannot show the boundary
  either.  What survives: five functions share the shape, and only one of the five
  came through the link with a name.
* **236 functions move floats through their stack frame, and the three transcribed
  here are at the mild end of that.**  `tools/spill_frame.py` is the census;
  `func_000C3470` — a 4x4 matrix scale that is 632 bytes for sixteen multiplies,
  with three passes over the data and a final `lw`/`sw` reload of values last written
  by `swc1` — is transcribed as machine code, as the two lerps are.  But it sits at
  48 frame stores and 40 loads, while the worst function in the module is at 4 and 28
  across 2,028 bytes.  **The report had called the lerps extreme and the count says
  they are middling** — they came out of the work queue, not out of being the worst of
  anything.  The ratio is not the discriminator either: what makes these need machine
  code is that the compiler spilled where it had registers free.
* **The cheapest pair of related functions in the module.**  `syncSkeleton_22B8` and
  `func_0012A9D0` are fifty-two bytes each and differ in exactly one instruction — the
  one in `jr $ra`'s delay slot, `nop` against `move $v0, $a0`.  **The return costs
  nothing because the slot was going to be a `nop` anyway**, which makes it the closest
  thing to two related functions this module offers.
* **The vector unit is used the long way, and used well.**  `func_0012D9F8` loads
  sixteen floats with sixteen `lv.s` where four `lv.q` would do, then stores them back
  with four `sv.q` to the same addresses — **a round trip whose effect on memory is
  nil**, with no arithmetic between the last load and the first store.  It is the
  vector-unit counterpart of `func_000C3470`'s spill storm and the opposite case: that
  one moves floats through memory because psp-gcc would not keep them in registers,
  this one keeps them in registers and does nothing with them.  `renderMeshInstances_1060`
  does use eight `lv.q`, so **both spellings are present and the module does not always
  pick the shorter one.**  And a tool was written to settle the staggered `svr.q` /
  `svl.q` store and could not: five instances, all at the identical offsets, so the
  module cannot say which half each store writes — a bounded negative the tool records
  as one.
* **One family, one member that returns, and the instruction it spends instead.**  The
  six-member three-float copy family has five void copies that begin with an `addiu`
  forming a base pointer and one that returns the destination pointer instead.  **Both
  forms are eight instructions** — this function did not drop the base pointer to make
  room for the return, it spent the instruction the base pointer would have occupied on
  the return.  That is the only place in the module where the choice between the two copy
  spellings is visible *inside a single family*, and `func_00055974` shows the same
  choice made twice in one body: three words through a formed base, four floats through
  immediate offsets, twelve instructions apart.  **So the 687 is not a preference the
  compiler has and sometimes loses — it is a choice it makes in one function and not the
  other, and what decides it is not established.**
* **A flag setter the census has no shape for, and a getter it reports as an orphan.**
  `func_00012F14` clears bit 31 and ORs a boolean into it — two operations, and
  `flag_accessors.py` models `get`, `set` and `clear` as one each.  Forty bytes away
  `func_00012F3C` reads the same bit of the same word and *is* found, so the pairing the
  census reports as half-missing was not half-missing at all.  `boolean_shapes.py` gains
  a third shape and counts **10 functions**, bounded to eight instructions because
  "some `or` later" in a 372-byte body pairs the wrong things.  That shape took two wrong
  versions to get right and both matched **zero** — first by requiring the `or` to write
  the `and`'s register, then by requiring the `addiu -1` to be adjacent to the `and`.
* **A fourth code-written address, and a two-line change that found a quarter of the
  census.**  `func_00102E6C` writes four floats to 0x0EC7E8, **128 bytes past 0x0EC768**
  which `func_00102C84` writes words to, and **both land in the same nominal function** —
  `func_000EC5E8` at +0x180 and +0x200.  Two independent initialisers in different
  translation units writing into one nominal body 128 bytes apart is the strongest
  evidence yet that these are a data region rather than a patch target, though splat's
  "distance to the next label" sizes still leave it unproven.  Finding it needed
  `code_writers.py` to stop ignoring the float opcodes: with `lwc1` and `swc1` added, the
  census goes from 1,024 functions at 455 bases to **1,458 at 675**.
* **850 functions form a base pointer once and then run consecutive accesses through
  it, so a function's length here is mostly codegen.**  `tools/base_pointer.py` counts
  687 that run stores through the formed base, 275 that run loads, 112 that do both.
  The five functions that prompted the tool are five of the 850, and `func_00102D34`
  holds the longest store run found at fifteen.  **The tool's first version counted 4,
  and none of the five were in it** — it required every store to follow the `addiu`
  with nothing in between, and `func_00194ADC` puts a load there, as four of the five
  do; it also insisted the run start at offset zero, which `func_00102D34` cannot
  satisfy.  **Both were the test being more specific than the shape.**  The count also
  caught a wrong claim in a file comment: `func_0018A650.c` cited the store count as
  evidence for itself, and that function forms its base for three `lwc1`s.
* **Two functions, one a strict superset of the other.**  `func_00123588` and
  `func_00123560` are 0x28 apart and the first is inside the second, differing only in
  that three instructions of a second list-unlink are threaded through the middle.
  Two of the four visible differences are consequences rather than choices — the second
  load reads the field the first unlink just repaired, and a reload becomes unnecessary
  because the register is still live.  The three-word copies around them are members of
  the 687, and read whole the pair is a splice across two containers.
* **A dead mask in 168 functions, and a flag setter shape the census could not see.**
  `tools/boolean_shapes.py` counts two shapes that hand-transcribing turned up.
  `sltiu rt, rs, 1` followed by `andi rd, rs, 0xFF` — where the `sltiu` already yields
  0 or 1, so the mask is dead — is the module's standard spelling of "is this zero" and
  it appears in **168 functions** with the mask left in every one.  The second is the
  mirror of the flag-mask boundary above: `ori rt, rs, MASK` with `rt == rs` is a
  read-modify-write in one instruction where `lui` plus `or` needs a scratch register,
  so any mask at or below bit 15 makes the accessor four instructions instead of five.
  **18 functions use it**, against the eight rows `flag_accessors.py` finds for the wide
  form — so more than half the immediate-width setters were invisible to the census
  built to find them.  Both counts were wrong first, and wrong plausibly: testing the
  REGIMM sub-opcode against the standard MIPS value 9 matches nothing, because in this
  encoding `sltiu $a0, $a0, 1` is `0x2c840001` with *both* candidate fields equal to
  4; and requiring the register to be `$a0` matched 192 functions that were the
  float-constant idiom, `lui 0x3F7D` then `ori 0x70A4`.  **The two functions that
  prompted this are 1.2 % and 5.6 % of their totals** — transcribing finds instances,
  counting says how many.
* **A tool matched only one of the two spellings of an address, and the gap hid a
  cluster.**  `stride_table.base_of` required the `addiu` to write the register the
  `lui` wrote; `func_00102D34` uses the other form, `lui $t0, 0xE` then
  `addiu $a1, $t0, 0x2168`, which keeps the base and copies the address elsewhere.  An
  `any_dest` flag — off by default, because in a linked image nothing marks which
  `addiu` completes a symbol — takes `code_writers.py` from 1,024 functions at 455 bases
  to 1,242 at 515.  The same hunt produced **three wrong turns worth recording**: a
  hand-computed `lui`/`addiu` pair gave `0x0C768` where the tool said `0x0EC768`; a
  check for "inside the live body" had its comparison inverted; and a test for
  relocations at the target passed nothing even for the three already-verified
  clusters, which is how the docstring's phrase turned out to mean relocations *inside
  the containing function*.  **What those addresses are is still not established** —
  they are real symbols with six and nine relocations, but splat's function sizes are
  "distance to the next label" and there is no label there.
* **The module keeps writable data inside its code section, and three clusters inside
  live code are written by two dozen live functions.**  `tools/code_writers.py` is
  the census: 12 functions name 0x0E9728, 9 name 0x0E97A8, 3 name 0x0EB850, and each
  of those addresses falls inside a function's own body — checked by disassembling
  the containing function to its `jr $ra`.  Every one of the 24 is referenced by an
  `R_MIPS_26` relocation, so none is dead code.  The unfiltered count is 1,024 and
  that is an upper bound, not a finding: splat derives function sizes from "distance
  to the next label", so a data block after a function is attributed to it.  What the
  writes are *for* is not established.
* **Nine functions in the module write into its own code section.**  `tools/stride_table.py`
  is the census: nine functions build the address 0x0EB850 and index it with a
  28-byte stride, and 0x0EB850 is sixteen bytes into `func_000EB840`'s prologue,
  with two `jal` relocations inside it.  One of the nine has a real caller, passing
  an unmasked byte out of its object as the index.  What that means is recorded and
  **not** resolved — three readings fit the bytes and none can be settled from inside
  the functions — but it is a fact about the shipped image, and the tool also
  reports which of its 1,203 shared constants are just immediates: the largest are the
  chunk tags, `surf` 137 times and `gshd` 108, which is `tools/tags.py` confirmed from
  a direction it does not otherwise use.
* **The one genuinely ugly piece** is `__attribute__((noreturn))` on a function
  that does return.  It is a lie about control flow that only affects codegen, and
  it does two jobs: it stops GCC appending a return after a hand-written block, and
  it lets the block's own `nop` be counted in the symbol size — which matters more
  than it looks, and is written up in `progress.md`.  A third job turned up recently
  and is less obvious: a `.set noreorder` region that spans a label boundary makes
  the assembler append a word *past the end of the symbol*, so the bytes all match
  and only the length is wrong.

A generator can emit all 7,497 function bodies as verbatim assembly inside C
files and does verify 7,497/7,497 — but that is a transcription, not a
decompilation, and it produces no more understanding of the code than the `.s`
files above.  The work here is hand transcription, function by function, and the
point is to work out what the code *does*.

The image stays byte exact regardless of how far the C gets: a function is linked
from `src/` only once it has been proven to compile to the original bytes.

## Legal

**The tooling is MIT licensed. The recovered game code is not, and cannot be.**

| | |
| --- | --- |
| `tools/`, `include/`, the pipeline | MIT — see [`LICENSE`](LICENSE) |
| `src/eboot/` (hand written C) | unlicensed, on purpose — see [`src/eboot/LICENSE.txt`](src/eboot/LICENSE.txt) |
| `asm/` (generated assembly) | unlicensed, on purpose — see [`asm/LICENSE.txt`](asm/LICENSE.txt) |

The files in `src/` and `asm/` are reverse engineered from `EBOOT.BIN`, a
commercial game by Maxis / EA.  They are a derivative work of EA's copyright, so
no one but EA can license them.  What you get is the right to read them, and the
right to use, modify and redistribute the tooling without restriction.

**You need your own copy of the game to do anything else.**  No disc image, no
decrypted executable and no toolchain binary is committed here;
`tools/setup.py` says where it looked.  That is the ordinary condition on
reverse engineering for study and interoperability, and it is the condition
this work was done under.

The reference project this takes its structure from,
[tclamb/mhp2g-decomp](https://github.com/tclamb/mhp2g-decomp), uses CC0 at its
root.  CC0 is a public domain *dedication* — it asserts the licensor may
relinquish copyright entirely.  That assertion cannot honestly be made about
reverse engineered game code, so the tooling here uses MIT, which is a grant
rather than a relinquishment, and the recovered code is left unlicensed
deliberately.

This is not legal advice.  If the licensing matters to you beyond making the
repository honest about what it is, talk to someone who can advise on it.

## Reproducing

Two things are needed and neither is in this repository: the decrypted module
image and the psp-gcc toolchain.  Tell the tools where you keep them, once:

```sh
export TS2PSP_ELF=/path/to/EBOOT.dec        # POSIX
export TS2PSP_PSPDEV=/path/to/pspdev/bin
export TS2PSP_MSYS=/path/to/msys64/usr/bin  # only if not at C:/msys64
```

```bat
set TS2PSP_ELF=F:\path\to\EBOOT.dec          :: Windows
set TS2PSP_PSPDEV=F:\path\to\pspdev\bin
set TS2PSP_MSYS=C:\msys64\usr\bin
```

`TS2PSP_ELF` and `TS2PSP_PSPDEV` default to the in-tree location
(`disks/pgs-si2/EBOOT.dec` and `bin/pspdev/bin`), so a working copy that has them
in place needs no environment at all.  `python tools/setup.py` reports which of
the two you have and, if one is missing, how to get it.

Then:

```sh
python tools/split.py --build  # regenerate everything and compare
```

`splat` and `spimdisasm` are only needed to regenerate the assembly, which is
already committed; `pip install -r requirements.txt` if you want to.
`progress.md` records where each dependency comes from.

The disc image is **not** in this repository and must not be added to it — it is
1 GB of Maxis/EA data.

## The checks

```sh
python tools/check_image.py     # byte-for-byte, the module image itself
python tools/check_symbols.py   # does every function report the size it should?
python tools/progress.py        # per-function matching report
python tools/verify_c.py        # does this C compile to the original bytes?
python tools/subsystems.py      # which translation unit belongs to which module
```

`check_image.py` exists because `build.py --diff` compares section *contents*,
which cannot see a layout error: every section was correct while the file bytes
after the last one were shifted by four.  Both checks are in the pipeline, and
`check_symbols.py` runs first — it catches the other way an image goes wrong, a
function whose symbol size is short, and names the function instead of reporting
half a million differing bytes.

## Status

`progress.md` is the authority, and it is kept honest — including the parts that
did not work, and why.  It also records every rule worked out for pinning the two
compilers together, so the `src/eboot/*.c` files can be read against the reasons
rather than as magic.

What is open now:

* **A conditional where both sides are long enough that the compiler has to choose
  a block order.**  The easy kinds are all solved; this is the one layout question
  left.  `tools/find_loops.py` lists candidates by size.
* **The `lui`/`mtc1` float-constant idiom**, the last piece of the CodeWarrior
  floating-point habits psp-gcc does not share.  Thirteen other float functions
  are done.
* **Recovering the controller class names** from the type descriptors at
  `0x001DB014`, and the packed property bytes at `0x001E1B98`.
  The property bytes are decoded and turn out not to be booleans: `flags[i] & 0x07`
  can only ever be 0, 1, 2 or 4, so `func_00140A58` returns a **four-state field**.
  What the index means is open — it is not ASCII, which `tools/flag_table.py --domain`
  tests in both alignments and rejects.  Of the eight bits, **bit 7 is read by nobody
  at all**, and bits 4 and 6 only by `func_0010CFC0`, which masks nearly every bit in
  turn and looks like a serialiser rather than a property test.
* **Naming the rest of the module.**  **The symbol table is not stripped** — 3,882 of
  the 15,977 symbols carry a name from the original CodeWarrior link, and
  `tools/orig_names.py` is the census.  What that bought:
  * **The build tree.**  The literal `c:/ad_clean/sims_psp/src/elem/bent/circular.h`
    is stored at `0x1C8F08` and `C:/ad/sims_psp/testing/luaDumps` at `0x1C95E4`, so the
    root is `c:/ad/sims_psp/` with `src/` and `testing/` below it.
  * **The language and the shape of the engine.**  51 of the surviving string literals
    are C++ `Class::method` names over 46 classes — roughly thirty of them expose
    `onUpdate` and nothing else, which is a scheduler-driven behaviour system.  There
    is a **Lua** scripting layer (`BehaviorLuaTask`, `lua_yield`, `SCHED_LUA`) and a
    Lua debugger (`LuaBreakPoint`, `luadump`) in the shipped build.
  * **About a dozen function names**, which is all that survives in code: `stub` 223
    times, then `updateNodeGraph`, `sortAndCullScene`, `syncSkeleton`,
    `renderMeshInstances`, `collision`, `drawing`, `renderCommon`, and three `elem_`
    ones.  The rest have to be recovered by analysis.
  * 3,754 string literals and 320 static constructors are also mapped; roughly 185
    constructors still touch nothing but the shared runtime and have no name.
* **The 223 PSP imports have no names and cannot get them from here.**  Every stub
  in the 26 `.sceStub.text.*` sections is an empty `jr $ra` placeholder that the
  loader patches at load time, so the code carries nothing and `.rodata.sceNid`
  cannot be interpreted with confidence from the binary alone.
  `tools/nid_table.py` records the index/address/library/NID correspondence for
  whoever wants to match it against a NID table.  The original link agrees: all 223
  are named `stub`.

Two tools are worth pointing at.  `tools/setup.py` reports which of the two inputs
this repository deliberately omits a given clone is missing.  `tools/delay_slots.py`
groups all 7,503 functions by the instruction in the return's delay slot, which
doubles as a census of how much stack 5,788 of them need — without reading a single
instruction of their bodies.  `tools/find_loops.py` finds the 1,740 functions with
a backward branch, smallest first.  `tools/flag_table.py` decodes the 128 packed
property bytes at `0x001E1B98` and lists the twenty functions that read them.

One caveat about reading the generated assembly, learned the hard way: `asm/eboot/*.s`
writes branch targets as `.Leboot_XXXXXXXX` labels, and the label's *position in the
file* is not always the branch's target.  It also writes data references as
`%hi(sym_001E1B98)` rather than as the literal address, so a grep for the address
finds nothing while a grep for the symbol finds twenty uses.  `tools/disasm_range.py`
disassembles on the fly and prints the real displacement; trust it over the `.s` when
a target matters.
