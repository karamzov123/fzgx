#!/usr/bin/env python3
"""Amusement Vision LZSS (.lz) used by F-Zero GX and Super Monkey Ball.

This is the FF7-style LZSS (4 KiB ring buffer, 3..18 byte matches) with an
8-byte header of two little-endian u32s: compressed payload size (excluding
the header) and uncompressed size. The decoder mirrors the retail routine in
fze.sample.rel (ring buffer starts at index 0xFEE, first 0xFEE bytes zeroed,
flag byte bit set = literal, clear = 2-byte back-reference whose 12-bit
offset is an absolute ring index).

Decompression is byte-exact by construction. Compression is a greedy
longest-match encoder; whether it reproduces Sega's original bytes is checked
by tools/prepare_orig.py and recorded, since only `ninja disc` needs it.
"""

from __future__ import annotations

import argparse
import struct
import sys
from pathlib import Path

RING = 0x1000
RING_START = 0xFEE
MIN_MATCH = 3
MAX_MATCH = 18


def decompress(data: bytes) -> bytes:
    comp_size, uncomp_size = struct.unpack_from("<II", data, 0)
    end = comp_size + 8
    if len(data) < end:
        raise ValueError(f"truncated: header says {end} bytes, have {len(data)}")
    ring = bytearray(RING)
    pos = RING_START
    out = bytearray()
    src = 8
    flags = 0
    while True:
        flags >>= 1
        if not flags & 0x100:
            if src >= end:
                break
            flags = data[src] | 0xFF00
            src += 1
        if flags & 1:
            if src >= end:
                break
            b = data[src]
            src += 1
            out.append(b)
            ring[pos] = b
            pos = (pos + 1) & 0xFFF
        else:
            if src + 1 >= end:
                break
            b0, b1 = data[src], data[src + 1]
            src += 2
            off = b0 | ((b1 << 4) & 0xF00)
            n = (b1 & 0xF) + MIN_MATCH
            for k in range(n):
                b = ring[(off + k) & 0xFFF]
                out.append(b)
                ring[pos] = b
                pos = (pos + 1) & 0xFFF
    if len(out) != uncomp_size:
        raise ValueError(f"size mismatch: header {uncomp_size}, decoded {len(out)}")
    return bytes(out)


def compress(data: bytes) -> bytes:
    """Greedy longest-match LZSS. Ring index of a match = (RING_START + src_pos) & 0xFFF."""
    n = len(data)
    # Virtual window: ring initially zero in [0, 0xFEE), so a match may start
    # in a zero region before the data. We model the stream as zeros + data.
    prefix = RING_START
    buf = bytes(prefix) + data
    total = len(buf)
    head: dict[int, list[int]] = {}
    out = bytearray(8)
    flag_pos = len(out)
    out.append(0)
    flags = 0
    nflag = 0
    i = prefix

    def key(p: int) -> int:
        return buf[p] | (buf[p + 1] << 8) | (buf[p + 2] << 16)

    while i < total:
        best_len, best_pos = 0, 0
        if i + MIN_MATCH <= total:
            cands = head.get(key(i), ())
            limit = min(MAX_MATCH, total - i)
            for p in reversed(cands):
                if i - p >= RING:
                    break
                if i - p == RING:  # cannot encode exactly-window-back
                    continue
                l = 0
                while l < limit and buf[p + l] == buf[i + l]:
                    l += 1
                if l > best_len:
                    best_len, best_pos = l, p
                    if l == limit:
                        break
        if best_len >= MIN_MATCH:
            ring_idx = (best_pos - prefix + RING_START) & 0xFFF
            out.append(ring_idx & 0xFF)
            out.append(((ring_idx >> 4) & 0xF0) | (best_len - MIN_MATCH))
            step = best_len
        else:
            flags |= 1 << nflag
            out.append(buf[i])
            step = 1
        for p in range(i, i + step):
            if p + MIN_MATCH <= total:
                head.setdefault(key(p), []).append(p)
        i += step
        nflag += 1
        if nflag == 8:
            out[flag_pos] = flags
            flags, nflag = 0, 0
            flag_pos = len(out)
            out.append(0)
    if nflag:
        out[flag_pos] = flags
    else:
        del out[flag_pos]
    struct.pack_into("<II", out, 0, len(out) - 8, n)
    return bytes(out)


def main(argv: list[str] | None = None) -> int:
    p = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    p.add_argument("mode", choices=["d", "c"], help="d=decompress, c=compress")
    p.add_argument("input", type=Path)
    p.add_argument("output", type=Path)
    a = p.parse_args(argv)
    data = a.input.read_bytes()
    a.output.write_bytes(decompress(data) if a.mode == "d" else compress(data))
    return 0


if __name__ == "__main__":
    sys.exit(main())
