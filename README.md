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
| C functions byte-exact and hand written | 68 |

The engine is Maxis' "Elem", the shared engine also used by *The Sims 3* — the
build path baked into the binary is `c:/ad_clean/sims_psp/src/elem/…`.

## What is here

| directory | |
| --- | --- |
| `asm/` | one `.s` per function, as splat produced them.  Assembling and linking this reproduces the original exactly.  Recovered material — see [Legal](#legal). |
| `src/eboot/` | the hand written decompilation: what each function does, in C.  68 of these are verified to compile to the original bytes.  Recovered material — see [Legal](#legal). |
| `include/` | the shared types: `f32`, `Vec3f`, and the object layouts the early functions reveal. |
| `tools/` | everything that produced the above, and the checks that keep it honest.  MIT licensed. |
| `config/` | the recovered symbol map, the relocation map, and hand recovered names. |
| `assets/` | the data sections, byte for byte. |
| `progress.md` | **the report**: what is known, how it was found, what is still open.  Start here. |

## Two layers, and why the distinction matters

**The assembly is byte-exact and complete.**  `asm/` reproduces every byte of
the module image.  That part is finished and has been for some time.

**The C is the readable layer, and it is partial.**  68 of 7,497 functions.  The
original was built with CodeWarrior for PSP (`mwccpsp.exe`), and psp-gcc does
not reproduce its instruction selection.  Reaching byte-exact C for *all* 7,497
needs that proprietary compiler; 68 were reached without it, by pinning the
handful of registers and delay slots where the two compilers disagree.

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
python tools/progress.py        # per-function matching report
python tools/verify_c.py        # does this C compile to the original bytes?
python tools/subsystems.py      # which translation unit belongs to which module
```

`check_image.py` exists because `build.py --diff` compares section *contents*,
which cannot see a layout error: every section was correct while the file bytes
after the last one were shifted by four.  Both checks are in the pipeline.

## Status

`progress.md` is the authority, and it is kept honest — including the parts that
did not work, and why.  The open items are the floating point functions (whose
`lui`/`mtc1` constant idiom is the hardest CodeWarrior habit to reproduce), block
layout in functions with branches, and recovering the controller class names
from the type descriptors at `0x001DB014`.
