#!/usr/bin/env python3
"""Encrypt/decrypt F-Zero GX's enemy_line/line__.bin (main.rel.lz).

The retail loader (fze.sample.rel) XORs every big-endian 32-bit word of the
file with the successive states of a 32-bit linear congruential generator:

    s_0     = key2
    s_{n+1} = s_n * key1 + key0        (mod 2**32)
    word_n ^= s_{n+1}

XOR makes encryption and decryption the same operation. The trailing partial
word (file size is not a multiple of 4) is XORed against a zero-extended word
and truncated back to the original length, matching the loader's behaviour of
operating on a padded buffer.

Key constants per region come from CosmoCortney/Line__Crypt and are
cross-checked against the loader's disassembly (see docs/batches/m0.md).
The original tool also computes 64-bit multiply-high values and a 'salt' term
that never feed back into the keystream; they are dead code and omitted here.
"""

from __future__ import annotations

import argparse
import struct
import sys
from pathlib import Path

MASK32 = 0xFFFFFFFF

# region -> (key0 additive, key1 multiplier, key2 initial state)
KEYS = {
    "INT": (0x000CD8F3, 0x9B36BB94, 0xAF8910BE),  # USA (GFZE01) and Europe (GFZP01)
    "JPN": (0x0004F107, 0xB5FB6483, 0xDEADDEAD),  # Japan (GFZJ01)
}


def crypt(data: bytes, region: str) -> bytes:
    key0, key1, state = KEYS[region.upper()]
    n_words = (len(data) + 3) // 4
    padded = data + b"\0" * (n_words * 4 - len(data))
    words = list(struct.unpack(f">{n_words}I", padded))
    out = []
    for w in words:
        state = (state * key1 + key0) & MASK32
        out.append(w ^ state)
    return struct.pack(f">{n_words}I", *out)[: len(data)]


def main(argv: list[str] | None = None) -> int:
    p = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    p.add_argument("input", type=Path)
    p.add_argument("output", type=Path)
    p.add_argument("--region", choices=sorted(KEYS), default="INT")
    args = p.parse_args(argv)
    args.output.write_bytes(crypt(args.input.read_bytes(), args.region))
    return 0


if __name__ == "__main__":
    sys.exit(main())
