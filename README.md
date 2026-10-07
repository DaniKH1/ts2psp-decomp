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
| `asm/` | one `.s` per function, as splat produced them.  Assembling and linking this reproduces the original exactly. |
| `src/eboot/` | the hand written decompilation: what each function does, in C.  68 of these are verified to compile to the original bytes. |
| `src/generated/` | not present; see *Two layers* below. |
| `include/` | the shared types: `f32`, `Vec3f`, and the object layouts the early functions reveal. |
| `tools/` | everything that produced the above, and the checks that keep it honest. |
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

## Reproducing

Needs the psp-gcc toolchain, `splat`, and `spimdisasm`; `tools/setup.py` fetches
the toolchain and `progress.md` records where each dependency comes from.

```sh
python tools/setup.py          # toolchain, splat, spimdisasm
python tools/split.py --build  # regenerate everything and compare
```

The disc image is **not** in this repository and must not be added to it — it is
1 GB of Maxis/EA data.  `tools/setup.py` takes a path to your own copy.

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
