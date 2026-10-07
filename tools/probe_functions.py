"""Prototype: run spimdisasm over the EBOOT .text to find function starts."""

import sys

sys.path.insert(0, "tools")
import spimdisasm  # noqa: E402
import spimdisasm.common  # noqa: E402
import spimdisasm.mips  # noqa: E402
import pspelf  # noqa: E402
from spimdisasm.common import Context, GlobalConfig  # noqa: E402
from spimdisasm.mips.sections import SectionText  # noqa: E402

GlobalConfig.ENDIAN = "little"
GlobalConfig.ABI = "n32"

elf = pspelf.load("disks/pgs-si2/EBOOT.dec")
text = elf.section(".text")
print(f".text addr={text.addr:#x} size={text.size:#x}")

ctx = Context()
sec = SectionText(ctx, 0, text.size, 0, ".text", text.data, 0, "eboot")
sec.disassemble()
sec.analyze()
print("functions found:", sec.nFuncs)
funcs = [(s.vromStart, s.vromEnd, s.name)
         for s in ctx.globalSegment.symbols.values()
         if getattr(s, "isFunction", False)]
funcs.sort()
print("function symbols:", len(funcs))
for s in funcs[:20]:
    print(f"  {s[0]:#010x}-{s[1]:#010x} {s[2]}")
print("  ...")
for s in funcs[-5:]:
    print(f"  {s[0]:#010x}-{s[1]:#010x} {s[2]}")
