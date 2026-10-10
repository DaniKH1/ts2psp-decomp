# The Sims 2 PSP decompilation

This project reconstructs the PSP game module as readable C and checks each
function against the retail executable. A C function is considered complete
only when the selected PSP compiler and linker reproduce its retail bytes
exactly.

The target is the PSP release of *The Sims 2* (`pgs-si2.iso`). The ISO and all
extracted or decrypted game data stay outside Git.

## Progress

**59 of 65 current C candidates match byte for byte (90.8%).** Fifty-eight
match with GCC 3.3.6; `store_word_pair_at_00052604` matches with GCC 15.2.0.

| Result | Functions |
| --- | --- |
| 16-byte code-word accessors | `read_code_word_at_0x743A4_00049A90`, `write_code_word_at_0x743A4_00049AA0` |
| 16-byte field update | `subtract_word_at_30` |
| 16-byte pair writer | `store_word_pair_at_00052604` writes two words to a caller-provided struct and returns its address. |
| 44-byte counter update | `advance_counter_at_0x50_000643D0` increments the field at `+0x50` and wraps it at the limit in `+0x54`. |
| 8-byte word accessors | `set_word`, `get_word_at_0x0_0004E5CC`, `get_word_at_0x0_0004E5D4` |
| 8-byte leaves | Four empty no-ops, plus `get_address_at_0x30_000111D8` and `get_address_at_0x5C_000273BC`. |
| 21 one-word getters | Generated names identify the field offset and function address; see `config/matched_c.txt`. |
| 9 one-word setters and clearers | Names record the write or clear operation, field offset, and function address. |
| 14 byte and address accessors | Exact byte loads/stores and byte-offset address calculations. |
| 1 constant return | `return_0x8000_at_000E5A10` reproduces the retail `ori`. |

The authoritative list is generated in [`config/matched_c.txt`](config/matched_c.txt).
Names use one consistent convention: describe the observed operation or byte
offset, then include the original address as a stable suffix. The four empty
functions use `noop_at_<address>` because their purpose is not established.
Human names map to the original binary inventory in
[`config/renames.txt`](config/renames.txt). The remaining six candidates are
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
installed lanes in order until a candidate matches or all lanes fail; `--lane`
restricts the check to selected lanes.

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

- Added 21 exact word getters at offsets from `0x00` through `0x3C`; their
  names describe the observed load and offset without guessing field meaning.
- Added 9 exact one-word setters and clearers.
- Added 14 exact byte accessors, byte setters, and address getters.
- Added an exact constant return for `0x8000`.
- Added `store_word_pair_at_00052604`, which writes a two-word result struct;
  GCC 15.2 reproduces its 16 retail bytes exactly.
- Added `advance_counter_at_0x50_000643D0`, using a padded struct for its
  counter and limit fields; all 44 retail bytes match with GCC 3.3.6.
- Reached 59/65 exact C candidates (90.8%); 58 match with GCC 3.3.6 and the
  pair writer matches with GCC 15.2.0.
- `352477f` - Decompiled `func_00029CFC` as `subtract_word_at_30`, with a padded
  struct and the register placement required for the exact GCC 3.3 output.
- `47bf530` - Replaced the incorrect `func_0004E5DC` getter hypothesis with
  the exact struct-based `set_word` implementation.
- `8d4f6ed` - Added eight byte-exact leaf functions.
- `389be32` - Added the first GCC 3.3 byte-exact accessor pair.
- `c132b01` - Researched the retail compiler era and wired the GCC 3.3 lane.

## Next steps

- Investigate the six remaining candidates and add only verified matches.
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
