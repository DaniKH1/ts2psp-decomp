# The Sims 2 PSP decompilation

This project reconstructs the PSP game module as readable C and checks each
function against the retail executable. A C function is considered complete
only when the selected PSP compiler and linker reproduce its retail bytes
exactly.

The target is the PSP release of *The Sims 2* (`pgs-si2.iso`). The ISO and all
extracted or decrypted game data stay outside Git.

## Progress

**12 of 20 current C candidates match byte for byte (60%).** All 12 currently
match with the GCC 3.3.6 Allegrex lane.

| Result | Functions |
| --- | --- |
| 16-byte accessors | `func_00049A90`, `func_00049AA0` |
| 16-byte field update | `subtract_word_at_30` |
| 8-byte accessors | `set_word`, `func_0004E5CC`, `func_0004E5D4` |
| 8-byte leaf functions | `func_000068E8`, `func_000111D8`, `func_00024CFC`, `func_000255B8`, `func_000273BC`, `func_0004A914` |

The authoritative list is generated in [`config/matched_c.txt`](config/matched_c.txt).
Human names map to the original binary inventory in
[`config/renames.txt`](config/renames.txt). The remaining eight candidates are
still being investigated; an unverified candidate is not treated as completed
C.

## How verification works

1. `config/functions.txt` records function addresses and sizes recovered from
   the stripped ELF.
2. A candidate in `src/` is compiled on its own with each installed toolchain
   lane. `tools/verify_c.py` links it against the retail addresses in
   `config/pgs-si2.symbols.ld`.
3. The harness extracts the linked `.text` and compares every byte with the
   corresponding bytes in `BOOT.BIN`.
4. Only an exact match belongs in the committed source set. Run the verifier
   before committing a decompilation change; `--adopt` refreshes the generated
   match list.

A source can use a readable name and still match an address-based inventory
entry through `config/renames.txt`. Struct padding preserves known offsets while
names and field meanings remain uncertain. Small empty `void` functions are
valid when the retail body is just `jr ra` and its delay-slot `nop`.

## Current toolchains

`tools/pspcc.py` defines the compiler lanes and shared flags. The verifier tries
every installed lane unless restricted with `--lane`.

| Lane | Compiler | Location | State |
| --- | --- | --- | --- |
| `gcc33` | GCC 3.3.6 with the BRS Allegrex port | `C:\pspdev33` | Installed and usable; all current matches use this lane. |
| `gcc15` | GCC 15.2.0 PSP toolchain | `C:\pspdev` | Installed and usable. |
| `gcc46` | GCC 4.6.4 PSP toolchain | `C:\pspdev46` | Installed and usable. |

The GCC 3.3 lane borrows the PSP assembler and linker and drops the later
`-mpreferred-stack-boundary=4` option, which its driver does not recognize.
A clean GCC 3.3 rebuild was attempted but currently fails in the host
`libiberty` configure step; the installed compiler and verifier remain usable.

## Repository map

| Path | Contents |
| --- | --- |
| `src/` | One C source per committed, byte-exact function. |
| `include/` | Shared C types. |
| `config/functions.txt` | Binary-derived function inventory. |
| `config/renames.txt` | Human names mapped to inventory symbols. |
| `config/matched_c.txt` | Generated list of exact C matches. |
| `config/pgs-si2.symbols.ld` | Linker symbols pinned to retail addresses. |
| `tools/` | ISO extraction, ELF inspection, inventory, linker-script, and verification tools. |
| `disks/` | Ignored extracted game data. |
| `bin/`, `build/` | Ignored tools and temporary build output. |

## Reproduce the local workflow

The ISO is expected at `F:\Sims 2 PSP Decomp\pgs-si2.iso`. The extraction
script writes `BOOT.BIN` and `EBOOT.BIN` under the ignored `disks/pgs-si2/`
directory.

```sh
python tools/extract_iso.py
python tools/elfinfo.py
python tools/inventory.py
python tools/imports.py
python tools/linkerscript.py
python tools/verify_c.py --verbose
```

To check one named candidate or update the generated list:

```sh
python tools/verify_c.py --function set_word --verbose
python tools/verify_c.py --adopt
```

Use `python tools/verify_c.py --help` for lane selection and other options.
The verification scripts expect the PSP toolchains described above to be
installed on this machine.

## Recent completed iterations

- `352477f` - Decompiled `func_00029CFC` as `subtract_word_at_30`, with a padded
  struct and the register placement required for the exact GCC 3.3 output.
- `47bf530` - Replaced the incorrect `func_0004E5DC` getter hypothesis with
  the exact struct-based `set_word` implementation.
- `8d4f6ed` - Added eight byte-exact leaf functions.
- `389be32` - Added the first GCC 3.3 byte-exact accessor pair.
- `c132b01` - Researched the retail compiler era and wired the GCC 3.3 lane.

## Next steps

- Investigate the eight remaining candidates and add only verified matches.
- Use call sites, neighboring code, and data references to improve struct and
  function names without claiming semantics the binary does not establish.
- Continue building a readable C reconstruction, then extend the same exactness
  discipline to the remaining module.

The local reference checkout at `F:\Sims 2 PSP Decomp\ref` contains
[mhp2g-decomp](https://github.com/tclamb/mhp2g-decomp), which can inform the
workflow for another PSP decompilation. It is not a source for this game.

## Legal and data policy

The game code and assets are copyrighted by Maxis/EA. This repository does not
include the disc image, extracted executable, decrypted data, or toolchain
binaries. Use a copy of the game you are entitled to use. The repository's
tooling is MIT licensed; see [`LICENSE`](LICENSE).
