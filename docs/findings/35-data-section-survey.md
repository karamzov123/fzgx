# Finding 35 — Data-section survey (sprint day 1, session 22)

Method: parsed build/GFZE01/asm/coarse/data_*.s object blocks; classified by content.

## Range: 0x80095EA0–0x80121EC0 ("sparse", 0x8C020 bytes)
- ONE string: "fze.sample.rel" @0x80095EA0 (REL loader name).
- 143,363 4-byte words; only 7,834 nonzero; of those just 32 pointer-like
  (mostly 0x80800000/0x80400000-range = AR/ARAM addresses, not code ptrs).
- Content is sparse binary blobs (pattern words like 0x55555558, 0xffff0020,
  bit-packed data). NOT a table region amenable to semantic naming.
- VERDICT: low rename yield. Leave as coarse blob; do not spend squad time.

## Range: 0x80121EC0–0x8012AA60 (SDK banners/strings)
- Already well-covered by earlier waves: GXResetFuncInfo, jumptable_* labels,
  ARC assert strings named. Remaining unnamed are small SDK-internal strings.
- VERDICT: mostly done; no dedicated wave needed.

## Range: 0x8012AA60–0x8015A860 (float-dense "game tables")
- 73 obj blocks in first unit: 64 word-blocks, 8 strings, 1 float block at head
  (small int/float mix: 0,10,616,205.33333 — looks like UI/layout metrics).
- Dense nonzero command-like words (0x2ff0021, 0x13061203...) — consistent with
  GX display-list / material command streams, NOT scalar physics tables.
- VERDICT: the "race physics constants" hypothesis is WRONG for this range;
  it is display-list/binary asset data. Naming individual lbl_ entries has low
  value; the float-dense race physics likely lives in .sdata2 or overlay RELs.

## Recommendation to PM sprint plan
- Data-section squads on these ranges will not produce meaningful verified-unit
  or symbol gains. The real remaining levers in-main.dol are:
  1) M3-style semantic fn renames with positive evidence (findings/23/29/31/32
     style studies on remaining ~1680 fn_ units), and
  2) near-miss unit matching (objdiff fuzzy>0%) which moves byte counts.
- Suggest redirecting data-squad effort to per-family fn naming studies
  (adxt/criadx/axmix/gamehead families already have candidate maps).
