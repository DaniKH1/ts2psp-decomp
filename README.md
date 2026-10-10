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

### Compiler era: retail is not GCC 15

The first two candidates proved that modern GCC's codegen differs from the
2007 retail build in ways no flag recovers (see the evidence below), so a
second, era-adjacent lane is building: **`gcc-4.6.4-psp`** (the oldest
allegrex-capable branch upstream) into `C:\pspdev46`, alongside GCC 15.

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
C-shape problems. Candidates are tried against both lanes; per-function
matching decides which toolchain is recorded for it.

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
     (`_stub_sceAudio`, …);
   * **6,421 data addresses** recovered by scanning the original code for
     `lui`+`addiu`/`ori` and load/store pairs that resolve into an allocated
     section — globals, string literals, jump tables — each annotated with
     which functions reach it.
2. **`tools/verify_c.py`** checks each `src/<name>.c`:
   `psp-gcc` (the flags above) → link against `symbols.ld` so `%hi`/`%lo`
   pairs and `jal` targets resolve exactly as the retail link did →
   `psp-objcopy -O binary --only-section=.text*` → byte-compare against
   `BOOT.BIN`. `--adopt` writes the passing list to
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
4. 🔄 Compiler era: the first two candidates proved GCC 15's codegen is
   not the retail one (see *Compiler era*) — build the `gcc-4.6.4-psp`
   lane into `C:\pspdev46` (in progress) and get the first byte-exact
   match recorded, per candidate under whichever lane reproduces it.
5. ✅ Import names: **223/223 recovered** (`tools/imports.py` →
   `config/imports.txt`, wired into `symbols.ld`) — the pspsdk stub-record
   join, the `SHA-1(name)[:4]` LE scheme, and curated psplibdoc records;
   `sceKernel…`-style calls now resolve to their real stubs.
6. Assemble the inventory back to a byte-exact image (the asm layer).
7. Decompile function by function — readable C, human-named structs,
   renamed symbols wherever the code shows what it does.
