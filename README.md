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
| C functions byte-exact and hand written | 106 |

The engine is Maxis' "Elem", the shared engine also used by *The Sims 3* — the
build path baked into the binary is `c:/ad_clean/sims_psp/src/elem/…`.

## What is here

| directory | |
| --- | --- |
| `asm/` | one `.s` per function, as splat produced them.  Assembling and linking this reproduces the original exactly.  Recovered material — see [Legal](#legal). |
| `src/eboot/` | the hand written decompilation: what each function does, in C.  106 of these are verified to compile to the original bytes.  Recovered material — see [Legal](#legal). |
| `include/` | the shared types: `f32`, `Vec3f`, and the object layouts the early functions reveal. |
| `tools/` | everything that produced the above, and the checks that keep it honest.  MIT licensed. |
| `config/` | the recovered symbol map, the relocation map, and hand recovered names. |
| `assets/` | the data sections, byte for byte. |
| `progress.md` | **the report**: what is known, how it was found, what is still open.  Start here. |

## Two layers, and why the distinction matters

**The assembly is byte-exact and complete.**  `asm/` reproduces every byte of
the module image.  That part is finished and has been for some time.

**The C is the readable layer, and it is partial.**  106 of 7,497 functions.  The
original was built with CodeWarrior for PSP (`mwccpsp.exe`), and psp-gcc does
not reproduce its instruction selection.  Reaching byte-exact C for *all* 7,497
needs that proprietary compiler; 106 were reached without it, by pinning the
handful of registers and delay slots where the two compilers disagree.

What those 106 have in common is that the disagreement is small enough to name.
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
  in the branch's delay slot (`func_000CD5B0`, `func_0009C9B4`) are all byte-exact.
  A conditional with two long sides, where the compiler has a real block-order
  choice, is still untested.
* **CodeWarrior peels a loop's first test out and leaves it above the loop's own
  setup.**  `func_0014402C` — a `strlen` — branches to its return *before* the
  instruction that sets the register the return subtracts, so an empty string would
  return nonsense.  The identical shape is in a second function elsewhere in the
  binary, so it is a compiler habit rather than a one-off, and the reading that fits
  is that no caller passes an empty string.  Worth knowing before reading any loop
  here: a guard sitting above the loop's prologue is not guarding it.

* **The one genuinely ugly piece** is `__attribute__((noreturn))` on a function
  that does return.  It is a lie about control flow that only affects codegen, and
  it does two jobs: it stops GCC appending a return after a hand-written block, and
  it lets the block's own `nop` be counted in the symbol size — which matters more
  than it looks, and is written up in `progress.md`.

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
  `0x001DB014`, and the packed flag bytes at `0x001E1B98`.
  The flag bytes are decoded and turn out not to be booleans: `flags[i] & 0x07` can
  only ever be 0, 1, 2 or 4, so `func_00140A58` returns a **four-state property**.
  Bits 4 to 7 are set on 32, 32, 12 and 1 classes and nothing masks for them yet —
  finding those readers is open.
* **Naming the rest of the module.**  3,754 string literals and 320 static
  constructors are mapped; roughly 185 constructors still touch nothing but the
  shared runtime and have no name.
* **The 223 PSP imports have no names and cannot get them from here.**  Every stub
  in the 26 `.sceStub.text.*` sections is an empty `jr $ra` placeholder that the
  loader patches at load time, so the code carries nothing and `.rodata.sceNid`
  cannot be interpreted with confidence from the binary alone.
  `tools/nid_table.py` records the index/address/library/NID correspondence for
  whoever wants to match it against a NID table.

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
