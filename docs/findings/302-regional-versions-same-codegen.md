# Finding 302: regional versions (EU/JP) do not help with register-allocation tie-breaks

Measured 2026-10-10 by extracting and comparing DOL/REL code across US (GFZE01),
EU (GFZP01), and JP (GFZJ01) retail builds.

## The experiment

Extracted the DOL and all RELs from the EU and JP ISOs, parsed the GameCube
filesystem (FST), and compared the executable code sections byte-by-byte.

## Results

- **movie_module.rel .text is byte-identical across all three regions** (md5
  d2be6886a3357e1680067ede47e0a9d7, 257004 bytes). Same for car_colchg.rel.
  The register-allocation tie-breaks in those modules are the same compiler
  output everywhere -- there is no region where MWCC coloured them differently.
- **DOL .text differs across regions but only in data/string sections.** The
  diffs are massive contiguous blocks (US vs EU: 58KB at 0x1d2, 31KB at 0xe5a7,
  13KB at 0x161bd; US vs JP: 189KB at 0x465e8, 149KB at 0x6c88, 69KB at
  0x35680). No scattered single-instruction register changes. The actual
  instruction sequences are identical.
- **fn_8005DCEC** (a DOL function): US vs JP differ in 149 bytes out of 7920,
  but every diff is a 1-2 byte immediate value change (e.g. 0x2d68 -> 0x25c8,
  0xc0 -> 0xe0) -- regional data constants, not register allocation.

## Conclusion

The register-allocation tie-breaks are a property of the MWCC compiler + the
source, not the region. The EU and JP builds use the same compiler with the same
flags on the same source, so they produce the same register allocation. There is
no region-crossing match to be found for these functions.

## What this means

The finding 301 levers remain the only path:
1. Source-shape work (proven exhausted for single levers -- 12 spellings tried)
2. A combinatorial induction-variable spelling search (the one lever search()
   does not synthesize)
3. The regional versions do NOT provide a shortcut
