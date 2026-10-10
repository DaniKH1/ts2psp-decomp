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

**Current match count: 12 / 20 candidates in `src/`** (60% byte-exact).

The first exact C matches are now in: `func_00049A90` and `func_00049AA0`
both reproduce all 16 retail bytes with the GCC 3.3 lane. Eight additional
trivial leaf functions (`func_000068E8`, `func_000111D8`, `func_00024CFC`,
`func_000255B8`, `func_000273BC`, `func_0004A914`, `func_0004E5CC`,
`func_0004E5D4`) match at 8 bytes each. The renamed `set_word` function
(`func_0004E5DC`) stores a value through a one-word struct and matches at 8
bytes. `subtract_word_at_30` updates a word in a padded object and matches
all 16 bytes at `func_00029CFC`. `func_0000F574` differs by one delay-slot
word; three wrapper functions differ only in frame-prologue words (known GCC
3.3 vs. retail frame-layout difference); the remaining four need further work.

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

The flags every byte-exact C file must be compiled with (the last two are
derived from observed retail code, see *Compiler era*):

```sh
psp-gcc -G0 -mabi=eabi -march=allegrex \
        -fno-pic -fno-common -ffunction-sections -fdata-sections \
        -fno-strict-aliasing -mpreferred-stack-boundary=4 \
        -fno-optimize-sibling-calls -O2 -c
```

### Toolchain: built from source under MSYS2

There are **no Windows binaries** of psp-gcc anywhere: pspdev publishes only
Linux/macOS tarballs (checked every release back to 2020), devkitPro dropped
PSP from its package repos (`dkp-windows` has devkitARM/PPC/A64 but no PSP),
and no copy survives on disk. So the toolchain is being built from source:

* host: the existing MSYS2 at `C:\msys64` (its `prepare.sh` does not know
  MSYS2, so dependencies were installed by hand with `pacman` — `base-devel`,
  `gcc`, `cmake`, `bison`, `flex`, `meson`, `ninja`, `libtool`, `gpgme`,
  `texinfo`, plus the `gmp-devel`/`mpfr-devel`/`mpc-devel`/`isl-devel`
  split-out development packages GCC needs);
* source: `pspdev/psptoolchain-allegrex` cloned to `C:\pspdev-src` — it builds
  binutils `allegrex-v2.44`, GCC `allegrex-v15.2.0` (two stages) and newlib
  `allegrex-v4.5.0` into `PSPDEV=C:\pspdev` (no spaces in the path);
* first failure fixed: the bundled readline (pulled in via gdb) still uses K&R
  declarations (`extern char *tgoto ();`), which GCC 15 rejects under its C23
  default where `()` means `(void)` — the build now exports
  `CFLAGS="-O2 -std=gnu17"`;
* second failure fixed: `libcody` (inside GCC) probes with
  `#if __cplusplus != 201103` and retries with `-std=c++11` *prepended* —
  an exported `CXXFLAGS=-std=gnu++17` lands after it and overrides the retry
  (the last `-std` wins), so no `CXXFLAGS` is exported;
* binutils (`allegrex-v2.44`), GCC `allegrex-v15.2.0` (two stages), newlib
  and pthread-embedded all built and installed — `psp-gcc`, `psp-as`,
  `psp-ld`, `psp-objcopy`, `psp-objdump` are in `C:\pspdev\bin`
  (`C:\pspdev\build.txt` records all five build entries);
* only the psptoolchain dependencies are needed (`check-pspdev.sh`); the
  outer pspdev repo's `check-dependencies.sh` demands pkg-config metadata for
  libarchive/openssl/ncurses that MSYS2 does not ship, but those libraries are
  only used by `psp-pacman`, not by psp-gcc.

**Status: complete.** `C:\pspdev\bin` has the full GCC 15.2.0 toolchain —
`psp-gcc`, `psp-ld`, `psp-objcopy`, plus newlib (`psp/include`, `libc.a`) and
pthread-embedded (`build.txt` carries all five build entries).

#### Second lane: GCC 4.6.4 in `C:\pspdev46`

Built from the same `psptoolchain-allegrex` scripts with
`C:\pspdev46-src\build.sh` (per-step host flags) and
`C:\pspdev46-src\override.sh` pinning the gcc ref:

* the override must set **`PSPTOOLCHAIN_ALLEGREX_GCC_DEFAULT_REPO_REF`** —
  `toolchain.sh` passes `$TAG` (empty for branch clones) as `$1`, so scripts
  read the `_DEFAULT_` variable, and a wrong name silently clones modern GCC;
* host flags per step: binutils 2.44 (2025 code) needs
  `-std=gnu17`/`-std=gnu++17`, the 2012-era steps need
  `-std=gnu89`/`-std=gnu++98 -fpermissive -fcommon`;
* **`liblto_plugin.so` name fix**: the driver demands a readable
  `LTOPLUGINSONAME` (`liblto_plugin.so`) on *every* invocation, but MSYS
  installs `cyglto_plugin-0.dll` — a copy under that name in
  `libexec/gcc/psp/4.6.4/` unblocks everything;
* steps run as `1 2` (binutils + gcc) — **newlib/pthread are skipped**:
  newlib 4.5's `sys/_intsup.h` type machinery fails against gcc 4.6's
  predefined macros, and the harness needs no libc (candidates are
  self-contained and link against `symbols.ld` with
  `--unresolved-symbols=ignore-all`).

**Status: usable** — `C:\pspdev46\bin\psp-gcc.exe` reports
`psp-gcc (GCC) 4.6.4`, and `verify_c.py` now tries every installed lane.

#### Third lane: GCC 3.3.6 in `C:\pspdev33`

Built from the 2005 BRS Allegrex patch under MSYS2. GCC 3.3.6's release
`c-parse.c` must be used with its grammar; a modern Bison-generated parser
accepted the token numbers but rejected ordinary named parameters. The
parser enums themselves match the release source. The build fixups also move
`TI_MAX` after the Allegrex tree indices, preventing the custom tree globals
from overrunning `global_trees` into the standard integer-type table.

The lane compiles the full nine-case C type probe. GCC 3.3 rejects
`-mpreferred-stack-boundary=4`, so the harness drops that option for this
lane. Its driver invokes plain `as`, so the lane borrows the existing PSP
assembler and linker through plain-name aliases. A clean rebuild attempt
currently fails during host `libiberty` configure; the installed compiler
and lane verifier are usable.

### Compiler era: retail is not GCC 15

The first two candidates proved that modern GCC's codegen differs from the
2007 retail build in ways no flag recovers (see the evidence below), so a
second, era-adjacent lane was built: **`gcc-4.6.4-psp`** (the oldest
allegrex-capable branch upstream) into `C:\pspdev46`, alongside GCC 15.
`verify_c.py` tries every installed lane per candidate (`--lane` narrows);
a MATCH under any lane counts. The first two matches are the `0x743A0`
accessor pair, both under GCC 3.3.6.

Evidence, all checkable from `BOOT.BIN`:

| fingerprint | retail (2007) | gcc 15.2 (-O2) |
| --- | --- | --- |
| `func_0002BBA8` predicate | `li`+`bne` branch structure | `xori`+`sltu`, branchless |
| `func_00000B28` base addresses | `addiu a1,a0,96` hoisted | folded into displacements |
| `func_00000B28` store scheduling | scale store above the loads | loads hoisted first |
| whole `.text`: `xori`+`sltu` (branchless setcc) | **0** of 442,442 instrs | emitted for every predicate return |
| whole `.text`: `li`+conditional branch | 2,077 | — |
| whole `.text`: `mfc1`+`sw` float stores | 313 (int-typed fields) | reproducible only via int/union typing |
| whole `.text`: `swc1` | 10,105 (the normal float store) | same |

An explicit `if (…) return 0; return 1;` still compiles branchless under
GCC 15 (`-fno-if-conversion` does not apply — it is expression expansion),
and pointer locals fold flat, so these differences are version traits, not
C-shape problems. Candidates are tried against all three lanes; per-function
matching decides which toolchain is recorded for it.

Two further flags are **derived from retail observations** and are now part
of the harness flag set:

| flag | evidence |
| --- | --- |
| `-mpreferred-stack-boundary=4` | every retail frame is 16-byte aligned (`func_0004AB18`: `sp-32`, `ra` at 16; 8-byte default misaligns the stack) |
| `-fno-optimize-sibling-calls` | a census of all `.text` found **559** call-wrappers keeping `frame+jal+jr` in tail position and **0** true sibling calls (the 33 functions ending in `j` are loop back-edges, delay slots do body work) |

With those two, the wrapper candidates match retail **except for the frame
word** (4 of 7 words identical): retail frames put `ra` at 16 in a 32-byte
frame, i.e. a 16-byte register-argument area below it (`REG_PARM_STACK_SPACE`
for oldabi). Both available GCCs give `ra` at 12 in a 16-byte frame for
`-mabi=eabi`, because gcc ≥ 4.6 classifies EABI as *neither* old nor new ABI
(`TARGET_OLDABI = {ABI_32, ABI_O64}`), so the 16-byte floor never applies —
and `-mabi=32` overshoots (`ra` at 28: `calls.c` adds the floor *and* every
in-register argument's size). Leaf functions (frameless, no outgoing-args
logic) were probed next: `func_000643D0` compiles to 11 instructions in
retail but 9 under gcc 4.6.4 (folds `limit - 1` into a reversed `slt` +
inverted branch, and sinks the early store into the branch delay slot) and 7
under gcc 15 (`movz` instead of the branch). Three independent generation
traits, all version-locked: **the retail compiler predates the oldest branch
pspdev still maintains (4.6.4)**.

#### Era research: the Sony official SDK compiler

The open question "which gcc is retail, and can it be built" now has a
documented path:

* **No version string survives in the image.** `BOOT.BIN` was swept for
  `.comment` sections, `GCC: (GNU)` / `gcc version` / `psp-gcc` strings,
  bare `X.Y.Z` triples, years and month names: the only version-like string
  is zlib's own `1.1.4` (the game links zlib). The compiler identity must
  come from codegen, not from strings.
* **Sony's official SDK compiler versions are documented**: psp-gcc 1.x is
  based on **GCC 3.3.x**, psp-gcc 2.x on **GCC 4.0.x**
  (Retro Reversing's [Official Sony PSP SDK](https://www.retroreversing.com/pspsdk)
  overview). The Sims 2 PSP (Oct 2007, built with the official SDK — the
  `c:/ad_clean/...` build path and `sce*` imports) therefore sits in the
  **3.3.x–4.0.x** window — consistent with every codegen trait above.
* **The original allegrex port still exists.** The
  [BRS-PSP-Research-Initiative](https://github.com/BRS-PSP-Research-Initiative)
  (running the same match-the-compiler game for *Black Rock Shooter: the
  Game*) maintains
  [Psptoolchain-Allegrex-3.3](https://github.com/BRS-PSP-Research-Initiative/Psptoolchain-Allegrex-3.3):
  the 2005 Marcus Brown `psp`/allegrex port of GCC 3.3 as a patch series
  (`patches/gcc/3.3/new`), with binutils 2.25.1 patches, built inside Docker
  on Debian Sarge i386. Their README: *"GCC 3.3 — the version that Sony
  built the PSP toolchain off of"*, and their priority list targets
  "GCC 3.2, 3.3, 3.4 and 4.0 (all versions found in binary strings)".
* **Lane 3 (`gcc33` → `C:\pspdev33`) is usable**: patched GCC 3.3.6 is built
  natively under MSYS2 (`all-gcc` only) and reuses lane 1's assembler and
  linker. The harness compiles all twenty current candidates in all three lanes.
  Twelve candidates now match exactly with GCC 3.3.6; eight still differ.

## Verification harness

Byte-exactness is the definition of "decompiled" here, and this is the machine
that decides it:

1. **`tools/linkerscript.py`** regenerates `config/pgs-si2.symbols.ld`, the
   absolute address of every symbol a candidate can reference — the retail
   `.symtab` is empty, so this file *is* the recovered symbol table:
   * 5,180 function names from `config/functions.txt`;
   * **223 named import stubs** (`sceKernelCreateThread = 0x001B0630;`, …)
     from `config/imports.txt` — see *Import names* below;
   * 104 section anchors (`_sec_*_START`/`_SIZE`), 26 import-library anchors
     (`_stub_sceAudio`, …), and 1,995 `code_*` mid-code anchors;
   * **8,416 data addresses** recovered by scanning the original code for
     `lui`+`addiu`/`ori` and load/store pairs that resolve into an allocated
     section — globals, string literals, jump tables — each annotated with
     which functions reach it.
2. **`tools/verify_c.py`** checks each `src/<name>.c`:
   `psp-gcc` (the flags above) → link against `symbols.ld` so `%hi`/`%lo`
   pairs and `jal` targets resolve exactly as the retail link did →
   `psp-objcopy -O binary --only-section=.text*` → byte-compare against
   `BOOT.BIN`. Every **installed toolchain lane** is tried per candidate
   (`gcc33`, `gcc15`, `gcc46`; `--lane` restricts), and a MATCH names the lane that
   produced it. `--adopt` writes the passing list to
   `config/matched_c.txt`. Renames live in `config/renames.txt`
   (`<inventory_name> <your_name>`), so readable names never lose the
   original address.
3. **`tools/pspcc.py`** locates the toolchain and encodes the exact
   compiler flags in one place; **`tools/pspelf.py`** is the read-only
   module-image reader everything else shares.

The link stage is proven against the real toolchain binaries: a smoke test
assembled with `psp-as` and linked with the flags above resolves
`%hi/%lo(dword_001BF890)` to the exact retail pair (`lui 0x001C` +
`addiu -1904`) and `jal func_00000058` to `0x0C000016` — the addresses in
`symbols.ld` drive the relocations correctly. The whole pipeline
(compile → link → objcopy → compare) now runs end to end with `psp-gcc`
15.2.0; its first two verdicts were `DIFFERS`, which is what surfaced the
era difference documented under *Compiler era*.

### Import names: recovered 223/223

The image stores imports as bare 32-bit NIDs (`.rodata.sceNid`) with no
name table — names only existed in the SDK that linked the game.
`tools/imports.py` rebuilds them from three sources, in order:

1. **pspsdk stub records** — its assembly import macros spell the pairing
   out (`IMPORT_FUNC "InitForKernel",0x1D3256BA,sceKernelRegisterChunk`),
   giving a straight join on (library, NID): **216** of the 223;
2. **the NID scheme itself** — an NID is the first 4 bytes of
   `SHA-1(function name)`, read little-endian. Sanity-checked over the
   pspsdk records: 2,479/3,119 agree (the rest are the salted
   later-firmware kernel NIDs psplibdoc documents; this era's userland
   imports are plain);
3. **`config/nid-extra.txt`** — the 7 records neither source covers
   (5 `sceMpegAvc*` YCbCr/Csc calls, `sceKernelGetModuleId`,
   `sceKernelSetCompilerVersion`), curated from `pspdev/psplibdoc` where
   every one is flagged `matching` (SHA-1 reproduces it).

Result: `config/imports.txt` names **all 223** imports, and
`linkerscript.py` pins each to its stub address — a candidate calling
`sceIoOpen`/`sceKernelCreateThread` now links to the exact stub the
retail link did.

## What is here

| path | |
| --- | --- |
| `tools/` | MIT licensed. Everything that produced the above, and the checks. |
| `config/` | input hashes, function inventory, import names (`imports.txt`, `nid-extra.txt`), generated `pgs-si2.symbols.ld`, renames, matched list. |
| `disks/` | gitignored: the extracted module image. Never commit it. |
| `bin/` | gitignored: downloaded tool binaries (`pspdecrypt`, objdiff). |
| `src/` | the hand-written decompilation — only byte-exact files, one function per `.c`. |
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
python tools/imports.py       # NID -> name join -> config/imports.txt (223/223)
python tools/linkerscript.py  # recovered symbol table -> config/pgs-si2.symbols.ld
python tools/verify_c.py      # byte-compare every src/*.c against BOOT.BIN
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
3. ✅ Get psp-gcc in place: **GCC 15.2.0 built from source** under MSYS2
   into `C:\pspdev` (binutils + gcc×2 + newlib + pthread), and the
   compile-and-compare harness standing (`tools/linkerscript.py` +
   `tools/verify_c.py` + `config/pgs-si2.symbols.ld`, link stage proven
   against the real binaries).
4. ✅ Compiler era: the `gcc-4.6.4-psp` lane **is built** into
   `C:\pspdev46` and proved retail predates it (version-locked traits,
   see *Compiler era*). The era question is now **researched**: retail
   comes from Sony's official SDK compiler window
   (**psp-gcc 1.x = GCC 3.3.x**, psp-gcc 2.x = GCC 4.0.x), and the
   original 2005 Allegrex port of GCC 3.3 survives as the
   BRS-PSP-Research-Initiative patch set — lane 3 (`gcc33`) is usable from
   it. The first byte-exact matches are `func_00049A90` and
   `func_00049AA0` under GCC 3.3.6.
5. ✅ Import names: **223/223 recovered** (`tools/imports.py` →
   `config/imports.txt`, wired into `symbols.ld`) — the pspsdk stub-record
   join, the `SHA-1(name)[:4]` LE scheme, and curated psplibdoc records;
   `sceKernel…`-style calls now resolve to their real stubs.
6. **Byte-exact C functions**: 12 of 20 candidates in `src/` match
   exactly (60%). Includes 2 16-byte accessors, one 8-byte setter, one
   16-byte word update and 8 trivial leaf functions (8 bytes each). Three
   wrapper functions differ only in frame-prologue words; one candidate
   differs by one delay-slot word.
7. Assemble the inventory back to a byte-exact image (the asm layer).
8. Decompile function by function — readable C, human-named structs,
   renamed symbols wherever the code shows what it does.
