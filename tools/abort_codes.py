"""The abort codes of the guarded-execution mechanism.

`func_0011296C` is the abort half of a `setjmp`/`longjmp` guard:

    func_001129E0(self, fn, arg)   install a handler, run `fn` under it, restore
    func_0011296C(self, code)       write `code` into the handler, longjmp out
    func_00140AC4(env)              setjmp
    func_00140B28(env, value)       longjmp

`func_0011296C` takes the reason as its second argument and it is *always* an
immediate at every call site - `ori $a1, $zero, N` in the branch's delay slot, which
is where a small constant would be built.  So the module has a small closed set of
abort codes, and which function aborts with which one is the interesting part: two
sites that abort with the same code are failing for the same reason, whatever that
reason turns out to be named.

That is worth a census rather than a list in a report, because it is the kind of thing
that grows as more functions get read.  Run it again after any new transcription.

This does *not* prove the codes are an enum.  It proves they are constants and it
shows how many distinct ones exist, which bounds what an enum would have to contain.
"""
from __future__ import annotations

import argparse
import collections
import pathlib
import re

ASM = pathlib.Path(__file__).resolve().parent.parent / "asm" / "eboot"

# The abort entry point, and the two halves of the guard around it.
ABORT = "func_0011296C"
GUARD = "func_001129E0"

CALL = re.compile(r"^\s*(jal|jalr)\s+(\S+)")
# `ori $a1, $zero, 0x5` and `addiu $a1, $zero, -0x1` are the two spellings.
CONST_A1 = re.compile(
    r"^\s*(?:ori|addiu)\s+\$a1,\s*\$zero,\s*(0x[0-9A-Fa-f]+|-?\d+)"
)
OTHER_A1 = re.compile(r"^\s*\w+\s+\$a1,")


def body(name: str) -> list[str]:
    text = (ASM / f"{name}.s").read_text(encoding="utf-8", errors="replace")
    chunk = text.split(f"glabel {name}", 1)[-1].split("endlabel")[0]
    lines = []
    for line in chunk.splitlines():
        if "/*" not in line:
            continue
        body = line.split("*/", 1)[-1].split(";", 1)[0]
        lines.append(body.strip())
    return lines


def call_sites(target: str) -> dict[str, list[tuple[str, str | None]]]:
    """Every function that calls `target`, with the constant it passes in `$a1`."""
    sites: dict[str, list[tuple[str, str | None]]] = collections.defaultdict(list)
    for path in sorted(ASM.glob("*.s")):
        name = path.stem
        if name == target:
            continue
        text = path.read_text(encoding="utf-8", errors="replace")
        if target not in text:
            continue
        lines = body(name)
        for i, line in enumerate(lines):
            m = CALL.match(line)
            if not m or m.group(2) != target:
                continue
            # The argument is built in the delay slot, i.e. the next line.
            code: str | None = None
            if i + 1 < len(lines):
                nxt = lines[i + 1]
                c = CONST_A1.match(nxt)
                if c:
                    code = c.group(1)
                elif OTHER_A1.match(nxt):
                    code = "variable"
            sites[name].append((line, code))
    return sites


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--guard", action="store_true",
                    help="also list the callers of the guarded runner")
    args = ap.parse_args()

    sites = call_sites(ABORT)
    total = sum(len(v) for v in sites.values())

    by_code: dict[int, list[str]] = collections.defaultdict(list)
    unknown: list[tuple[str, str]] = []
    for caller, entries in sorted(sites.items()):
        for _, code in entries:
            if code is None:
                unknown.append((caller, "?"))
            elif code == "variable":
                unknown.append((caller, "variable"))
            else:
                by_code[int(code, 0)].append(caller)

    print(f"{ABORT} - abort with a reason code, unwinding via setjmp/longjmp")
    print(f"  {len(sites)} callers, {total} abort sites\n")

    print(f"  {'code':<6} {'sites':<6} who aborts with it")
    for code in sorted(by_code):
        who = by_code[code]
        names = ", ".join(n.replace("func_", "") for n in sorted(set(who)))
        print(f"  {code:<6} {len(who):<6} {names}")

    if unknown:
        print(f"\n  {len(unknown)} site(s) do not build the code with a plain")
        print("  immediate in the delay slot:")
        for caller, code in unknown:
            print(f"    {caller.replace('func_', '')}  {code}")

    if args.guard:
        guard = call_sites(GUARD)
        print(f"\n  {GUARD} - the guarded runner, {len(guard)} callers:")
        for caller, entries in sorted(guard.items()):
            print(f"    {caller.replace('func_', '')}  {len(entries)} call(s)")

    print(f"\n  {len(by_code)} distinct codes, so an enum would have "
          f"{len(by_code) + 1} values if 0 means 'ran to completion'.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())