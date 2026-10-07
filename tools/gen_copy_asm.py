"""Emit the asm for a long load/store interleaving.

Some of the engine's accessors are "copy N words, loading and storing them in a
rotating pattern of three registers": `func_00102BF8` is 16 such words and the
other function of its shape has 13.  Writing that out by hand is tedious and easy
to get wrong, and the register pattern is completely mechanical - the loads
cycle `$a1`, `$a2`, `$a3`, and each store uses whichever register the load two
steps earlier filled.

That regularity is worth a generator rather than a transcription.  The output is
the interior of an `__asm__` block, with the operands already written out, so it
can be pasted straight into a source file; each source records its word count in
a comment so it can be regenerated.

    python tools/gen_copy_asm.py --words 16 --dest-hi 0xE --dest-off 0x69A8
"""

from __future__ import annotations

import argparse

LOAD_REGS = ("a1", "a2", "a3")


def block(words: int, dest_hi: int, dest_off: int) -> list[str]:
    """The instruction lines for `words` interleaved loads and stores.

    The shape is: the first three words are loaded up front, then the
    destination base is materialised, then each subsequent load is followed by
    the store of the word before it.  Three registers are enough because a
    store always trails its load by one step, so the oldest of `$a1`..`$a3` is
    free by the time the next load needs it.
    """
    lines: list[str] = []

    # The opening burst: three loads, then the base.
    for i in range(min(3, words)):
        lines.append(f"lw    ${LOAD_REGS[i]}, {i * 4}(%[a0])")
    lines.append(f"lui   %[t0], {dest_hi:#x}")
    lines.append(f"sw    $a1, {dest_off:#x}(%[t0])")
    lines.append(f"addiu %[t1], %[t0], {dest_off:#x}")

    # After the base, the sequence alternates load-then-store-two-behind.  Word 3 is
    # loaded, then word 1 (held in $a2) is stored, then word 4 is loaded, then
    # word 2 (held in $a3) is stored, and so on.  Each store trails the load that
    # filled its register by two, which is what lets three registers cover an
    # unbounded run.
    #
    # Word 0 is already stored (into the global via $t0), so the queue of
    # unstored words starts at word 1.
    pending = [("a2", 4), ("a3", 8)]
    cycle = ["a1", "a2", "a3"]
    last = words - 1
    for i in range(3, words):
        # The final word loads into $a0: the object pointer has been read for the
        # last time and is free, and the original reuses it rather than
        # disturbing a third register for one word.  Its store is the one that
        # goes in the return's delay slot, so it is the last instruction.
        reg = "a0" if i == last else cycle[(i - 3) % 3]
        victim, victim_off = pending.pop(0)
        lines.append(f"lw    ${reg}, {i * 4}(%[a0])")
        lines.append(f"sw    ${victim}, {victim_off:#x}(%[t1])")
        pending.append((reg, i * 4))

    for reg, off in pending:
        lines.append(f"sw    ${reg}, {off:#x}(%[t1])")
    return lines


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--words", type=int, required=True)
    ap.add_argument("--dest-hi", type=lambda s: int(s, 0), required=True)
    ap.add_argument("--dest-off", type=lambda s: int(s, 0), required=True)
    ns = ap.parse_args()
    for line in block(ns.words, ns.dest_hi, ns.dest_off):
        print(f'        "{line}\\n\\t"')
    return 0


if __name__ == "__main__":
    raise SystemExit(main())