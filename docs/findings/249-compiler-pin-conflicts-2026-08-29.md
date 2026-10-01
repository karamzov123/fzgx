# 249 — Compiler-pin conflicts: MetroTRK and OSReset

Date: 2026-08-29
Evidence source: authoritative `tools/natc_compile.py --discriminate` runs
Canonical head during probes: `76acc97b557298ed3963e16e6c77a50b0035b0e5`

## Verified results

### `main/dolphin/metrotrk/main`

Candidate:
`~/.cache/natc/scratch/natc5/main_TRK_ReadFile_Game_cand2.c`

| compiler | diff score |
|---|---:|
| GC/1.2.5 | 54.711 |
| GC/1.2.5n (authoritative) | 55.267 |
| GC/1.3 | **73.756** |
| GC/1.3.2 | 73.756 |
| GC/1.3.2r | 73.756 |

`natc_compile.py` emitted `Mispin suspect`: the authoritative unit edge is
GC/1.2.5n, while this candidate ranks GC/1.3 best. Do not change the pin in
place; this requires an integrator-controlled per-unit experiment.

### `main/dolphin/os/OSReset`

Candidate:
`~/.cache/natc/scratch/hard/OSReset_OSRegisterResetFunction.c`

| compiler | diff score |
|---|---:|
| GC/1.2.5 | **98.03** |
| GC/1.2.5n (authoritative) | 98.03 |
| GC/1.3 | 95.424 |
| GC/1.3.2 | 95.424 |
| GC/1.3.2r | 95.424 |

The tool reports GC/1.2.5 as the winner, but the candidate has already been
compiled once at the same source SHA and is therefore a duplicate candidate.
The 1.2.5 vs 1.2.5n distinction is a compiler/toolchain evidence issue, not a
reason to retry unchanged source.

## Action

- MetroTRK needs a unit-wide pin review before further source-shaping attempts.
- OSReset needs a fresh-context candidate (new source SHA) if the residual 2%
is to be closed; do not repeat the recorded candidate.
- Any pin change must be tested as a gated per-unit change and the full DOL SHA
must remain exact.
