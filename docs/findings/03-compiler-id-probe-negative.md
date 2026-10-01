# 03: Compiler-ID probe negative; OS leaf seeds found

Date: 2026-08-21. Supersedes the probe premise in tools/probe/README.md
(assumed memset@0x358/__fill_mem@0x388/memcpy@0x440 file offsets) and
corrects session-3's "GC/1.3 memset size-exact = promising" note.

## What was tested

1. ref slices at assumed offsets are NOT function-aligned (memset.bin starts
   mid-instruction `lis r3,0x8000`; memcpy.bin contains a real prologue at
   +0x14). The old addresses were wrong.
2. Compiled probe.c (MSL memset/__fill_mem/memcpy) across ALL 20 GC versions,
   two flag sets (ours; melee-style libc `-inline deferred -str pool,readonly
   -common off -fp_contract on -align powerpc`). Byte-searched whole DOL via
   tools/find_blob.py (exact + branch-slot-tolerant): ZERO code hits for any
   version -> GX's MSL vintage differs from melee's source; probe anchor dead.
3. Melee SDK tiny OS leaves compiled from /tmp/opencode/melee sources ×20
   versions: OSDisableInterrupts/OSEnableInterrupts/OSGetTick are byte-exact
   IDENTICAL across all compilers (asm-style bodies, non-discriminating) but
   DO exist in GX DOL -> real symbol seeds:
   - OSDisableInterrupts @ 0x8000D4F4
   - OSEnableInterrupts  @ 0x8000D508
   - OSGetTick           @ 0x80011424
   - __OSGetSystemTime ~ 0x8001142C (tolerant d12, GC/1.2.5n only)
   - __OSTimeToSystemTime ~ 0x80011490 (tolerant d8)
4. C-compiled melee MTX bodies (C_MTXIdentity/Copy/C_VECAdd/C_VECScale, asserts
   stripped) ×20 versions: ZERO hits any version -> melee-vintage source blob
   transfer is a dead end for compiler ID.

## Conclusions

- Compiler pin still UNKNOWN. Linker family is pinned instead: GC/1.3 links
  retail-byte-exact incl. generated startup tables (M0 green); GC/2.7 does not
  (session 2). Melee pins linker GC/1.3.2, SDK libs GC/1.2.5(n) — consistent.
- New plan: identify compiler by MATCHING real units with GC/1.3 first;
  mismatch patterns drive per-unit version retries. Matching IS the probe.

## Tools added

- tools/find_blob.py <dol> <blob...> [--prefix N] [--max-diff 12] — verified
  working; beware: vaddr=null hits outside sections (DOL-header false hits at
  file_off 0xc3 seen for 8-byte float-constant pools `@N` symbols).

## Cost traps

- Subagent final-message payloads returned empty all session (harness issue);
  artifacts on disk were real. Always verify from disk, never from summaries.
- Tiny/asm-style functions don't discriminate compilers; literal-pool `@N`
  data blobs give false positives in header/unmapped regions.
