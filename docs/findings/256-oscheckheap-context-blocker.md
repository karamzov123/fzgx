# OSCheckHeap context blocker

- Unit: `main/dolphin/os/OSAlloc`
- Symbol: `OSCheckHeap`
- Worker: `tool`
- Lease: verified active via `tools/natc_rank.py --status --worker tool`
- Durable submission queue: verified `0 ready`, `0 claimed`

## Verified evidence

`python3 tools/natc_loop.py --unit main/dolphin/os/OSAlloc --symbol OSCheckHeap --context-only` returned:

- Retail function size: 720 bytes (`0x2d0`), `.text`
- Caller: `OSDumpHeap`
- Callees: none
- Relocations: 6 `R_PPC_EMB_SDA21`, 2 `R_PPC_ADDR16_HA`, 2 `R_PPC_ADDR16_LO`
- Reference-backed projects: dolsdk2001, melee, melee-src-tmpcopy, sms
- Prior attempts: 11/12
- Best score: 45.289%
- Readiness verdict: `ready`

The live source at `src/dolphin/os/OSAlloc.c` defines `OSCheckHeap` as an `asm int` function. The available SDK reference is a natural-C body from `dolsdk2001:src/os/OSAlloc.c:499`, but it depends on SDK headers, `HeapDesc`/`Cell` definitions, heap globals, and assertion macros that are not present in the live asm-only translation unit. No existing complete `OSCheckHeap` candidate was found in the repository.

## Disposition

No candidate was generated or compiled. Passing the SDK body as a snippet, wrapper, or mutation would violate the lease protocol and exact-artifact requirement. A valid next step requires a fresh, authoritative translation-unit context that makes the natural-C body a complete compilable unit, or integrator disposition for adding the required declarations/definitions. Do not retry the unchanged source/context/compiler identity.
