# The Sims 2 PSP decompilation

This repository reconstructs the PSP game module as readable C, with the long-
term goal of rebuilding the game from source. Decompiled functions count as
complete only when the configured PSP compiler and linker reproduce the retail
machine code byte for byte. Readability and exact output are both required;
inline assembly is not a substitute for understanding a function's C behavior.

The retail ISO and extracted game data are kept outside Git.

## Current progress

The binary inventory contains **5,180 functions**. The repository tracks **178
C implementations** that reproduce their retail bytes exactly. Five more
local candidates are still being investigated and do not match yet; they stay
out of the tracked source set until verified. Current work is concentrated in
small accessors, leaf functions, and a few struct-based functions. This is not
yet enough to recompile the game module.

Run `python tools/verify_c.py --verbose` for the live per-function results.
The generated `config/matched_c.txt` records the candidate names that matched
on the last `--adopt` run; it is not a measure of full-game coverage.

### Open byte-matching work

| Function | Current mismatch |
| --- | --- |
| `func_00000B28` | C output is 32 bytes; retail is 48. The float loads, operations, and stores need to preserve the retail instruction sequence. |
| `func_0002BBA8` | C output is 12 bytes; retail is 32. The compiler shortens the comparison and branch structure. |
| `func_0004AAFC`, `func_0004AB18`, `func_000F8084` | The wrappers call the expected imports, but the compiler produces different stack-frame and return-address offsets than retail. |

Do not count a candidate as decompiled until the verifier reports an exact
match. The verifier compares linked code at the original retail addresses.

## Source and naming conventions

- Prefer descriptive C function names and structs when the observed behavior
  supports them. Keep uncertain fields as padding or neutral names instead of
  guessing their meaning.
- Include the retail address in generated names to keep otherwise similar
  accessors distinct, for example `get_word_at_0x18_00080584`.
- `config/renames.txt` maps a binary inventory symbol to a readable C symbol.
- Keep one source definition per retail function. Aliases for the same address
  must not create duplicate definitions in a future full-module link.
- Empty `void` leaves are valid when the retail body is exactly `jr ra` plus
  its delay-slot `nop`; a zero-byte C body still compiles to those instructions
  under the target compiler.
- Use compiler extensions only when they express the original C-level
  behavior and are needed for exact output. For example,
  `return_0x10_at_0000F574` binds a GNU C register variable to Allegrex `$zero`
  so the C expression emits the retail `ori`. A plain constant return emits a
  different instruction. This is target-specific C, not portable ISO C.

## Verification workflow

The inventory is derived from the retail ELF. Each candidate is compiled
separately, linked at its retail address, and compared against `BOOT.BIN`.
An exact byte match on any configured compiler lane is accepted.

```sh
python tools/verify_c.py --function return_0x10_at_0000F574 --verbose
python tools/verify_c.py --verbose
python tools/verify_c.py --adopt
```

Run the focused check while working on one function, then the full verifier
before committing. `--adopt` regenerates `config/matched_c.txt` from the
current results. `python tools/verify_c.py --help` lists lane and other
options.

## Toolchains

`tools/pspcc.py` describes the configured compiler lanes:

| Lane | Compiler | Local installation |
| --- | --- | --- |
| `gcc33` | GCC 3.3.6 BRS Allegrex port | `C:\pspdev33` |
| `gcc15` | GCC 15.2.0 PSP toolchain | `C:\pspdev` |
| `gcc46` | GCC 4.6.4 PSP toolchain | `C:\pspdev46` |

The verifier reports the lane used for each match. Keep compiler configuration
changes separate from function-source changes so exactness results stay
reproducible and reviewable.

## Repository layout

| Path | Purpose |
| --- | --- |
| `src/` | C function candidates. |
| `include/` | Shared C types and declarations. |
| `config/functions.txt` | Function addresses and sizes from the retail module. |
| `config/renames.txt` | Mapping from inventory symbols to readable source names. |
| `config/matched_c.txt` | Generated list of candidates with exact bytes. |
| `config/pgs-si2.symbols.ld` | Linker symbols pinned to retail addresses. |
| `tools/` | Extraction, ELF, inventory, linking, and verification scripts. |
| `disks/`, `bin/`, `build/` | Local game data, tools, and build products; ignored by Git. |

## Local setup

The extraction script expects the ISO at
`F:\Sims 2 PSP Decomp\pgs-si2.iso` and writes extracted files under the
ignored `disks/pgs-si2/` directory. The ISO and extracted data are not part of
this repository.

```sh
python tools/extract_iso.py
python tools/elfinfo.py
python tools/inventory.py
python tools/imports.py
python tools/linkerscript.py
python tools/verify_c.py --verbose
```

The local checkout at `F:\Sims 2 PSP Decomp\ref` contains
[mhp2g-decomp](https://github.com/tclamb/mhp2g-decomp) as a reference for PSP
decompilation workflows; it is not a source for this game.

## Recent work

- Added exact word and byte accessors, setters, address getters, constant
  returns, and empty leaf functions with names tied to observed offsets or
  retail addresses.
- Added struct-based implementations for `subtract_word_at_30`,
  `store_word_pair_at_00052604`, and `advance_counter_at_0x50_000643D0`.
- Removed the duplicate `func_00029CFC.c` implementation. The readable
  `subtract_word_at_30.c` is the single source for retail address `0x00029CFC`.
- Added exact return-zero leaves at `0x000F698C` and `0x0019C028`, and an
  identity-pointer leaf at `0x001AA5D8`.
- Added the exact float-field setter `set_float_at_0x18C_00170BD4` with a
  padded struct that keeps the field at offset `0x18C`.
- Added three 12-byte accessors/updates: incremented word read at `+0x0C`,
  low-16-bit read at `+0x04`, and a word write plus adjacent clear at `+0x48`.
- Added exact global access at `0x001D03828` and a global byte store at
  `0x001D03624`; their C structs preserve the observed base and offsets.
- Added five global word getters across the `0x0005xxxx` to `0x001E1F8C`
  address ranges, preserving the retail high-register base and signed offsets.
- Added eleven functions returning static addresses; their target-specific C
  expressions preserve the retail high-half/low-half construction.
- Added exact 12-byte struct operations for paired float storage, a word/byte
  field update, two-byte clearing, and a clear-through-pointer helper. A second
  global byte store at `0x001D04930` also matches.
- Added four exact float-pair setters for fields at `+0x24`, `+0x34`, `+0x3C`,
  and `+0x44`.
- Added eleven exact pointer-based helpers: indirect word/byte access, field
  copying, paired word stores, indexed byte clearing, and setters that return
  zero.
- Added an exact global float getter, a global byte clear, and three global
  word setters in the `0x0006xxxx` and `0x001Dxxxx` regions.
- Added exact float constant returns for `480.0f`, `272.0f`, and positive
  infinity. Their unions preserve the retail bit patterns, with a no-instruction
  register constraint for the retail `$a0` placement.
- Corrected `return_0x10_at_0000F574` to use only a GNU C hard-register
  variable bound to `$zero`; the unnecessary empty inline-assembly statement
  was removed. GCC 3.3.6 emits the exact retail 8 bytes.
- Reworked this README to report the current verifier results and document the
  byte-matching workflow and source conventions.

## Copyright and game data

The game code and assets are copyrighted by Maxis/EA. This repository does not
include the ISO, extracted executable, decrypted game data, or toolchain
binaries. Use a copy of the game you are entitled to use. The repository's
tooling is MIT licensed; see [`LICENSE`](LICENSE).
