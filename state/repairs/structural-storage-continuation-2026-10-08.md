# Structural, ownership and storage continuation

## Failure families and a measured cohort

The main_rel audit groups retail instructions by indexing stride as a routing heuristic, not proof that every equal-size object has the same semantics. The 0xC family has 85 unresolved members; the 0x440 family has 14. Three saved 0x440 callers independently witness a compatible partial physical view: s32 at 0x324, a 32-bit field at 0x334, u8 at 0x38C, a 32-bit field at 0x390, and s16 at 0x3B8. Unknown bytes are retained without invented semantics or initialization.

`include/rel/main_rel/selection_row.h` records that partial view. A real GC/1.3.2 compile of address/sizeof getters proves all five offsets and the 0x440 stride; its instruction words are in the portable dossier. The three-body private cohort measured:

| Function | Baseline differing rows | Typed-view differing rows | Exact closure |
| --- | ---: | ---: | --- |
| fn_1_7D6B8 | 5 | 5 | no |
| fn_1_12C110 | 15 | 15 | no |
| fn_1_38CF4 | 51 | 51 | no |

This is reusable structural evidence, not new function credit. The typed substitutions were stopped after their zero-closure result. The matcher context now supplies the scoped proof and that failed hypothesis only for these three targets, only while the header's SHA matches the proved version, and within its context budget. Malformed/stale proof and unrelated targets do not receive the note. No attempt counters, limits, model pins or cooldowns were reset.

The existing five-row selection-loop and two-row pointer-test frontiers remain preserved. No repeated broad pragma/version search was launched. The remaining difficulty is source-realizable register/lifetime/control-flow lowering, not an unproved array stride.

## Ownership reconciliation

Two old `matched` rows had no C source, no accepted commit and no reproducible exact saved body. They were suspended with the supported block API, retaining their original attempt counts (2 and 1) and prior records:

- fn_80036AC4: 352-byte DOL function; registered source missing.
- fn_12_D0B8: 884-byte movie-module function; source/commit missing. Its old blanket asm-only diagnosis is unproven: one HID2 read offers a stock-intrinsic investigation path, not proof of impossibility.

These functions are unresolved, not completed. Authored-C work remains 1,591 functions; unsupported credit is now zero. Historical score/link_state fields are not acceptance and do not replace source plus full hash proof. See `state/blocked.md`.

The 12,556-byte DTK/ledger gap is exactly accounted for by four generated, untyped main/DOL regions. Matching is by virtual address, since older report names can differ from current config aliases:

| Region | Address | Bytes | Observed contents |
| --- | --- | ---: | --- |
| pad_01_8006D044_text | 0x8006D044 | 4540 | Arithmetic/runtime instructions; not zero padding |
| pad_01_8006E2C0_text | 0x8006E2C0 | 24 | Paired-single/return sequence |
| pad_01_8008CF44_text | 0x8008CF44 | 4 | Return instruction; entry/padding role needs separate proof |
| pad_00_800035E4_init | 0x800035E4 | 7988 | Resident-kernel marker, zero fill and mixed initialization region |

Reference objects, byte SHA-256s and raw first words are in the dossier. These are explicit coverage obligations; no fake functions were created from pad labels and no covered byte count was silently redefined.

## Evidence-preserving inode/storage retention

A complete low-priority scan found 14,854,895 files and 178,570 subdirectories in `.fzgx/fixup/sessions`, occupying 126,274,007,040 allocated bytes. It contains 7,817,265 generated C files, 6,844,918 objects and 170,818 compiler logs.

The old collector assumed object directory names were full 64-character candidate IDs. Actual compilation uses 24-character chunk directories with several object siblings and diagnostics. That assumption is removed.

The replacement `tools/fzgx/fixup_gc.py`:

- Retains every compiler-scored C source, all metadata, and every metadata-referenced source/current object chunk, including parent/frontier objects, compile.log and .sym.o companions.
- Archives other generated sources and objects losslessly with per-member size/hash/cookie manifests; re-reads and verifies the entire compressed archive, fsyncs it, and only then removes matching original files. It never broad-deletes a session tree or rewrites its reports/cache.
- Skips missing/malformed metadata, symlinks and live Engine sessions. Stable external flock leases are shared with the Engine and inherited safely by live compiler children.
- Defaults to dry-run. Applies bounded file/session limits and preserves incomplete transaction artifacts/originals on failure. Archive/restore checks reject unsafe members and refuse to overwrite changed evidence.
- Guards generation at startup and every 128 newly created sources: stop below 8 GiB available or 100,000 free inodes. Cold retention has a separate bounded temporary-space check and can operate below that generation floor, leaving 256 MiB reserve. Restore cannot exhaust the generation reserve.
- Writes atomic monitoring state to `~/.cache/fzgx-agents/storage-v1.json`. Pressure is surfaced below 30 GiB available, below one million free inodes, or at 75% inode usage. Inspect the receipt timestamp and service journal; this CLI session has no automatic notification delivery channel.

The initial two manual batches and first service run placed 100,000 raw files into four fully verified cold bundles. Their original allocated size was 576,380,928 bytes; bundles occupy 49,440,571 bytes, a net 526,940,357 bytes of allocated-byte recovery for that measured set. One actual archived object was restored byte-exact without overwriting anything. Later timer runs are separately accounted in their receipts; whole-filesystem free space also fluctuates because other projects run large ISO tests, and is not attributed wholesale to this collector.

Cold archives are deliberately outside Git, at `.fzgx/fixup-archives/`. Important source and verdict evidence remains discoverable in hot reports/cache. Complete removed-file evidence is recoverable from the immutable bundles. Do not delete these archives as disposable cache.

## Operation

Versioned unit templates are `tools/systemd/fzgx-storage.service` and `.timer`. Installed copies run every ten minutes, compacting at most two sessions/20,000 files each, considering at most eight candidate sessions, with one-hour minimum age. Memory is bounded at 768 MiB, CPU at half a core, and I/O/Nice priority is low. User-requested fleet controls, including disabled GPT, are unchanged.

Inspect:

    df -h /home/armandofm/projects/fzgx
    df -i /home/armandofm/projects/fzgx
    systemctl --user status fzgx-storage.timer fzgx-storage.service
    journalctl --user -u fzgx-storage.service

Dry-run/restore (use the actual recorded archive/session/member values):

    .venv/bin/python tools/fzgx/fixup_gc.py --dry-run --max-files 20000 --max-sessions 2
    .venv/bin/python tools/fzgx/fixup_gc.py --restore .fzgx/fixup-archives/ARCHIVE_SHA.tar.gz --session .fzgx/fixup/sessions/SYMBOL --member objects/GROUP/CHUNK/OBJECT.o

The installed scheduler remains separate from matching. Disable only its timer if investigating retention; do not delete the archives or change matcher claims.

## Reproduction and acceptance

`state/repairs/structural-storage-audit-2026-10-08.json.gz` includes all three cohort sources/baselines, exact compiler verdicts, layout getter words, family routing, four coverage regions, prior unsupported-credit rows and initial retention receipts. Candidate source SHA-256s were read back and checked. Existing portable near-match archives also replayed successfully after the Engine lease change.

Regression harnesses remain outside this repository at `~/.local/share/fzgx-audit/tests/` per AGENTS.md. They exercise real file preservation/full restore, live leases, malformed metadata, corrupt archives, changed-evidence refusal, Engine lifetime and the distinct generation/retention pressure policies. Production acceptance additionally requires the real configure/Ninja/16-target DTK/lint gate, scoped commit and authorized publisher's clean pinned-SHA build plus exact remote readback.
