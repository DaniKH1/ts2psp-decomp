# The Sims 2 PSP — decompilation

A byte-exact reconstruction of the PSP version of *The Sims 2*, recovered from
the disc image with no symbols and no source.

| | |
| --- | --- |
| module image | 2,567,365 bytes (plain ELF, decrypted) |
| code | `.text` is 1,769,768 bytes = 442,442 instructions |
| functions | **5,180 enumerated** (see `config/functions.txt`) |
| jump-table labels | 2,441 recorded separately (not functions) |
| relocation tables | `.rel.text` 451,232 bytes, plus per-TU `.rel.*` |

The engine is Maxis' "Elem", the shared engine also used by *The Sims 3* — the
build path baked into the binary is `c:/ad_clean/sims_psp/src/elem/…`.

## Status: restarted from scratch

This repository was **wiped and restarted** (commit `f32e5214`). The previous
attempt had 753 of 7,497 functions decompiled to byte-exact C against psp-gcc;
all of that was removed on purpose and is being rebuilt here, cleaner, with the
same verification discipline: **nothing is committed as C unless it has been
proven to compile to the original bytes.**

What is proven so far:

* The module image is extracted from the ISO and identified:
  `disks/pgs-si2/BOOT.BIN` — sha1 `a65b73c34233ad1de8574d7c1d82195852d31a52`.
* `EBOOT.BIN` on the disc is a `~PSP` wrapper (tag `C0CB167C`, type 1);
  `pspdecrypt` recovers it, and the result is **byte-identical to `BOOT.BIN`**
  (same sha1). One canonical file, two names.
* The ELF is a 32-bit little-endian MIPS (Allegrex) module, entry `0x0008188C`,
  two `PT_LOAD` segments (R+X at `0x0`, R+W at `0x1EDF98`), linked at 0 and
  relocated by the loader — every address below is module-relative.
* **The symbol table is stripped**: `.symtab` holds only the null entry. Names
  have to be recovered from analysis. What the link did leave:
  * per-translation-unit code sections from `-ffunction-sections`:
    `.text`, `.text.collision`, `.text.sortAndCullScene`,
    `.text.updateNodeGraph`, `.text.syncSkeleton`, `.text.drawing`,
    `.text.renderCommon`, `.text.renderMeshInstances`;
  * complete relocation tables for every section (`.rel.text` alone is
    451,232 bytes = 56,404 relocations), which give the reference graph;
  * 26 `.sceStub.text.*` import stub sections (223 imports, all empty
    `jr $ra` placeholders patched by the loader);
  * `.cplinit` (2,576 bytes) — static constructors, 320 of them by the
    previous attempt's count;
  * 3,754 string literals in `.rodata` (74,300 bytes), 115,848 bytes of
    `.data`, and 963,972 bytes of `.bss`.
* The original compiler is **psp-gcc**, identified from the binary itself:
  `e_flags` `0x10A23001` (MIPS2 + EABI32 + Allegrex + `noreorder`) and the
  `-G0 -fno-pic -ffunction-sections -fno-common` fingerprint (see
  *The build target*).  The previous attempt's README said CodeWarrior;
  its own build driver disagreed, and the header settles it.
* The function inventory is complete and reproducible:
  `tools/inventory.py` finds **5,180 functions** from the binary alone —
  4,616 `jal` targets, 320 static constructors from `.cplinit`, 55 tail
  calls (`j` targets no conditional branch reaches), 160 data-pointer
  entries, and the 8 section heads — and books 2,441 switch jump-table
  labels separately (`config/functions.txt`).

## The build target

**The retail EBOOT was produced by psp-gcc**, not CodeWarrior — the evidence
is in the binary itself and it is checkable:

| evidence | value |
| --- | --- |
| ELF `e_flags` | `0x10A23001` — MIPS2, EABI32, Allegrex, `noreorder`: psp-gcc's default combination |
| small data | `-G0` — no `.sdata`/`.sbss`, not one `R_MIPS_GPREL16`; every global is a `%hi`/`%lo` pair |
| addressing | `-fno-pic` — absolute, no GOT indirection |
| sections | `-ffunction-sections` — the TUs survive as `.text.collision`, `.text.drawing`, … |
| tentative defs | `-fno-common` — landed in `.linkonce.d` |

(The previous attempt's README called the compiler CodeWarrior; its own build
driver contradicted that with the table above. The header wins — it is in the
file.)

The flags every byte-exact C file must be compiled with:

```sh
psp-gcc -G0 -mabi=eabi -march=allegrex \
        -fno-pic -fno-common -ffunction-sections -fdata-sections \
        -fno-strict-aliasing -O2 -c
```

### Toolchain status: not yet on this machine

There are **no Windows binaries** of psp-gcc anywhere: pspdev publishes only
Linux/macOS tarballs (checked every release back to 2020), devkitPro dropped
PSP from its package repos (`dkp-windows` has devkitARM/PPC/A64 but no PSP),
and no copy survives on disk. The plan, in order of preference:

1. build the pspdev toolchain from source under the existing MSYS2
   (documented route; long but automatable);
2. only if modern GCC's codegen turns out not to match the retail bytes,
   pin an older psptoolchain GCC version (the game is from 2007).

## What is here

| path | |
| --- | --- |
| `tools/` | MIT licensed. Everything that produced the above, and the checks. |
| `config/` | hashes of the inputs, and later the symbol map. |
| `disks/` | gitignored: the extracted module image. Never commit it. |
| `bin/` | gitignored: downloaded tool binaries (`pspdecrypt`, objdiff). |
| `src/` | the hand-written decompilation — only byte-exact files, once it starts. |
| `include/` | the shared types (`f32`, `Vec3f`, object layouts). |

## Reproducing

```sh
# one-time: pspdecrypt, verified against the checksum the reference project records
python tools/fetch.py https://github.com/John-K/pspdecrypt/releases/download/1.0/pspdecrypt-1.0-windows.zip bin/pspdecrypt.zip
# (expand bin/pspdecrypt.zip into bin/; sha1 of pspdecrypt.exe must be
#  c218f5aaed99c0995b11300ec081a7e279169285)

python tools/extract_iso.py   # pull BOOT.BIN + EBOOT.BIN out of the ISO
bin/pspdecrypt.exe -o disks/pgs-si2/EBOOT.dec disks/pgs-si2/EBOOT.BIN
python tools/elfinfo.py       # section map + symbol census
python tools/inventory.py     # function inventory -> config/functions.txt
python tools/list_iso.py      # what is on the disc
```

The ISO lives outside the repository (`F:\Sims 2 PSP Decomp\pgs-si2.iso`) and
must never be added to it — it is 1 GB of Maxis/EA data.

## Legal

The tooling is MIT licensed (see `LICENSE`). The recovered game code is a
derivative work of Maxis/EA's copyright and is **unlicensed on purpose** —
no disc image, decrypted executable or toolchain binary is committed here.
You need your own copy of the game. This is not legal advice.

## Function inventory

`tools/inventory.py` enumerates every function **from the binary alone** —
no symbol table is needed because the module links at 0 and every `jal`
carries its target in the instruction word. Sources, and the tier each
entry gets in `config/functions.txt`:

| tier | count | source |
| --- | --- | --- |
| `section` | 8 | start of a `-ffunction-sections` code section |
| `call` | 4,616 | target of a `jal` |
| `jump` | 55 | target of a bare `j` that no conditional branch reaches (tail calls) |
| `ctor` | 320 | entry of the static constructor list in `.cplinit` |
| `data` | 160 | a relocated data word pointing into code (vtable-ish, worth a per-function look) |
| **total** | **5,180** | |

2,441 further relocated data words point *into* function bodies in clusters —
switch jump tables — and are recorded but deliberately not counted as
functions.  The 320 constructors match an independent count from the previous
attempt's notes, which is a good sign for the method.

Everything the previous attempt called 7,497 "functions" is here with the
sources separated: this inventory counts 5,180 and books the jump-table
labels separately instead of inflating the function count with them.

## Roadmap

1. ✅ Extract and identify the module image.
2. ✅ Function inventory: every code section split into functions by
   `jal` graph, constructor list and section boundaries
   (`tools/inventory.py` → `config/functions.txt`).
3. Get psp-gcc in place and stand up the compile-and-compare harness.
4. Assemble the inventory back to a byte-exact image (the asm layer).
5. Decompile function by function — readable C, human-named structs,
   renamed symbols wherever the code shows what it does.
