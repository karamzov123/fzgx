# health-state.json decode — 2026-08-27 (fleet/tool, supervisor tool-health mandate)

Correction + evidence to a claim made in the stranded-harvest-probe cycle:
I had written that `health-state.json` is "written by a monitor OUTSIDE this
repo (grep across tools/ returns 0)". This cycle I traced it properly.

## What the file actually is
```
{
  "metric": 294,
  "last_alert_key": "stranded-work",
  "last_alert_ts": 1787857346.0784764,   # 2026-08-27 15:02:26 EDT
  "stranded": 244,
  "queue_total": 0,
  "stranded_recoverable": 80,
  "supervisor_missing": [],
  "last_alert_sig": "delivery-queue|stranded-work#68|60|773|90|72|73|73|23|50|68|773|38|4|3|3|115|117|117|6|65|46|115",
  "last_alert_sev": {"stranded-work": 240}
}
```
- `last_alert_sig` is a **history of alert severities**, NOT a function count
  (decoded: `stranded-work#68` then a numeric severity sequence 60,773,90...).
  The bare number 244 does NOT appear in the sig — it is the current
  `stranded` gauge.
- `last_alert_ts` = 15:02:26; file mtime = 15:32:48. So: computed ~15:02,
  flushed ~15:32. It is a **periodic gauge**, not event-driven.
- `queue_total: 0` => no live delivery backlog. `stranded: 244` = a backlog
  gauge of conversions that scored 100% but never landed (matches the
  `natc_harvest` semantics). `stranded_recoverable: 80` of those have
  recoverable source.

## Who writes it — evidence (not inference this time)
- grep `health_state|stranded_recoverable|last_alert_key` across `tools/` = 0
  hits. So **no tool in this repo writes it** (not natc_gate, natc_harvest,
  natc_compile — all checked).
- `grep -rln` across `~/.hermes/skills|scripts|profiles` = only
  `agent-fleet-operations/.../audit-reconciliation-and-fuzzy-matching.md`, and
  that reference only *mentions* "stranded-work alarms" as a fleet concept, not
  a writer.
- `~/.hermes/scripts/` is EMPTY (no cron script owns it).
- The writer is therefore most likely the **supervisor's own fleet sweep / a
  Hermes-internal monitor** (consistent with `supervisor_missing` and the
  gauge-style semantics). This is the correct correction to my earlier
  "outside monitor" one-liner: precisely, it is NOT a repo tool and NOT a
  user cron; it is the fleet supervisor's own health writer.

## Implication for the tool worker
- The `stranded-work` alert is a **fleet backlog metric**, not a tool crash or
  a broken tool. Nothing in `tools/` is responsible for clearing it.
- The only actionable drain is the integration-domain landing of the
  already-staged, provenance-compliant `GXGeometry` batch (3 packageable
  functions) via `natc_gate.py` — parked at
  `~/.cache/natc/scratch/tool/harvest-run-0827/GXGeometry/` and awaiting integ.
- The remaining 244-3 stranded require worker re-conversion (80 fail preflight)
  or are unrecoverable (71 source-lost) / false-positive (94 still-asm) — all
  outside tool-worker scope (contract L13/L143).

## Conclusion
Tooling worker's contribution to the stranded signal is complete: it identified
the recoverable set via `natc_harvest`, emitted a gate-ready batch, and verified
its provenance compliance. The durable gauge will only move when integ lands
that batch. No further tool-worker action exists; re-dispatch yields nothing.
