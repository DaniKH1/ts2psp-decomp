"""Shared MIPS/R4000ALLEGREX disassembly helpers for the Sims 2 PSP decomp."""

from __future__ import annotations

import struct

from rabbitizer import Abi, InstrCategory, Instruction

# PSP code is N32 ABI, so keep the standard register naming ($s0..$s7, $fp, $ra).
CATEGORY = InstrCategory.R4000ALLEGREX


def make_instruction(word: int, vaddr: int) -> Instruction:
    return Instruction(word, category=CATEGORY, vram=vaddr)


def disassemble_words(words, base_vaddr: int) -> list[str]:
    out = []
    for i, word in enumerate(words):
        vaddr = base_vaddr + i * 4
        out.append(f"{vaddr:08X}: {word:08X}  {make_instruction(word, vaddr)}")
    return out


def read_words(data: bytes, offset: int, count: int) -> list[int]:
    return [struct.unpack_from("<I", data, offset + i * 4)[0] for i in range(count)]
