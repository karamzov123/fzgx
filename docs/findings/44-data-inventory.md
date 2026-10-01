# F-ZERO GX main.dol data-section survey (sprint priority 2)

Binary: orig/GFZE01/sys/main.dol. Sections: .text 800055E0+8A920, .rodata 8008FF40+5F60,
.data 80095EA0+C5A80, .sdata 801A63C0+2E0, .sbss 801A6E40+AC0.

## 1. 80095EA0-800B3CB8 (.data head, sparse)

- `80095EA0-80095EB0` — string `"fze.sample.rel"` + pad. REL module name for the
  game's relocatable module; the following 0x7A40 bytes are its reserved load area.
- `80095EB0-8009D8F0` (0x7A40) — ALL ZERO: REL loading buffer.
- `8009D8F0-800A5688` — repeating structured records, NOT floats/pointers:
  records are 0x58-0xB0 with a recurring signature word pair `FFFF0000 00000000`
  and data words like `BDF70000 55555500` (-0.1206, 1.466e13 as f32 = bit
  pattern coincidence). Pattern is per-entity/per-track parameter blocks with
  s16-packed fields (`FFFF` markers = -1 sentinel / end-of-list). ~30 blocks;
  several near-identical copies at e.g. 800A0120/800A0B20/800A1520/800A1F20/
  800A2920 (0x4E0 each) suggest per-difficulty or per-machine variants.
- `800A3280-800A3900` — small fixed records containing repeated
  `FFFF0841 0000AA55` pairs; flag/mask tables.
- `800A3C88-800A50E8` — three large dense blocks (0x660/0x620/0x5A0), mixed
  s16 fields, same FFFF-marker style.
- `800A5688-800AA138` (0x4AB0) — ALL ZERO.
- `800AA138-AA698` — more FFFF-style parameter records (~4 small tables).
- `800AAB10-800B3CB8` — six ~equal-size groups of ~0x570-byte dense blocks
  separated by ~0x450 zero gaps (e.g. AB4F8/BEE0/C8E0/D2C8/DCC8/AE6E0/F0E8/
  FAC8/B04C8/B0EC8/B18C8/B22C8...): strongly periodic — reads as per-course
  or per-machine stat tables (12+ entries). Tiny 8-byte islands at
  800AD850, 800AF670, 800B1490, 800B32B0 (`0821E73C 00804040`,
  `E71C0000 D5951535`) are alignment leftovers / constants between blocks.
- No real pointer tables found in this range (ptr-fraction ≈ 0 everywhere);
  no genuine ASCII strings besides `fze.sample.rel`.

## 2. 80121EC0-8012AA60 (.data: SDK banners/version strings)

All identifiable banner/assert strings (exact VAs):

```
80121EC0 "The Disc Cover is open."          80121ED8 "If you want to continue the game,"
80121EFC "please close the Disc Cover."
80121F38 "the F-ZERO GX Game Disc."         80121F54 "The Game Disc could not be read."
80121F78 "Please read"                      80121F84 "the Nintendo GameCube Instruction Booklet"
80121FB0 "for more information."
80121FF4 "Turn the power off and refer to"  80122014 "for further instructions."
801220CC "open from DVD..."                 801220F0 "reading from DVD..."
80122118 "avfile.c"                         80122140 "no initialize entrynum_buffer"
801221C0..80122510  OSCheckHeap assertion strings x14 (heap.c)
80122590 "Not allocate heap block."
8012261C "Mar 17 2003"   80122628 "04:20:41"   <- SDK build stamp
80122634 "Console Type : "
80122C8C "Non-recoverable Exception %d"      80122CAC "Unhandled Exception %d"
80123BC4-80123C58 controller type names ("N64 controller", "GameBoy Advance",
             "Standard controller", "WaveBird controller", "Keyboard", "Steering", ...)
80123CC8 "SISetSamplingRate: unknown TV format. Use default."
80123D48-80123DFC card/device names ("Memory Card 59".."Memory Card 2043",
             "USB Adapter","Net Card","Broadband Adapter","IS-DOL-VIEWER",...)
80123F28/80123F5C DVDReadAsync()/DVDRead() out-of-range messages
80123FE4 "DVDChangeDisk(): FST in the new disc is too big."
8012A8B0 "Sep  5 2002"   8012A8BC "05:35:13"   <- second SDK build stamp (DVD lib)
```

Rest of range is SDK code-support data (function-pointer dispatch tables near
80124000+, OSAlloc/argv/exception vectors).

## 3. 8012AA60-8015A860 (.data float-dense game tables)

Boundaries & grouping (all float counts verified by f32 decode):

| Range | Size | Content |
|---|---|---|
| 8012AA60-8012ABA4 | 0x144 | small table; first half packed u8/u32 flags (0x21,0x13061203,...), not floats |
| 8012ABC0-8012D938 | 0xD54 | integer ramp tables: byte sequences 1,2,4,8,16,32... (power-of-2 ladders) |
| **8012B938-8012D938** | **0x2000** | **1024-entry sin/cos pair table** [cos,sin], quarter-wave symmetric, offset so idx0=(0.7071,-0.7071). Referenced by `fn_80053BFC` (adxt_80053A30.c already imports it as `lbl_8012B938[8320]`) |
| 8012D9B8-8012E140 | 0x6C4 | small-int config records (values 1,2,5, 0.6667=2/3 as final f32) |
| 8012E140-8013059C | 0x45C | 17 identical 0x5C-byte records spaced 0x180 apart (per-slot defaults); referenced via fn_8002D6B0/fn_8002D77C indexing `8012ABC0` base |
| 801309C4-801315E0 | 0x39C+0x868 | power-of-two float ramps (1e-45,4e-45,...) — quantization step tables |
| 80131630-80132558 | 0xBB0 | float ramp table, same family |
| **80132558-80142824** | **0x102CC (~66KB)** | **giant float table, values in [-1,1]** (samples 0.6949@+0x8000, 0.99985@+0xFFFF) — normalized curve/weight table, prime AI/handling candidate. No direct lis/addi ref found → accessed via computed base or r13-relative pointer copy |
| **80142844-80152844** | **0x10000** | **65536-word float table; entry k = k·2⁻²³·2^? — starts 9.587e-05 stepping ×(k/1024)**; value at +0x8000 = 1.0001918, ends at inf. Reads as **inverse-sine/arcsine normalization or 1/x-scaled lookup** (float[0x4000]); no direct lis/addi ref |
| **80152860-8015A860** | **0x8000** | **1024-entry pair table (10430.3779…→10430.36…, t²)** — 10430.38 = **65536/2π**, i.e. radian→binary-angle (rev→GXA/GC angle-unit) conversion pairs (value, correction). Sole direct reference: `lis r5,-0x7fed; addi r5,r5,-0x2648` at **fn_8006D2AC** (getCupModeConst region, 80050670 helper indexes sibling 8012D9B8 table by cup mode ×0x3000) |

## Zero-region confirmation

`800B3CB8-80121EC0` (0x6E208, 561.5 KB): read in full — **byte-for-byte all zeros**.

## Xref method note

Refs located by scanning main.elf `.text` for `lis/addi` ha/lo pairs (tools/
mine_assert_xrefs.py needs pyelftools; install into a venv to run it directly).
