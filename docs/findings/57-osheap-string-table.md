# Finding 57 — OS heap-check diagnostic string table

Symbolic-only data rename on `contrib-20260824-osheap-strings`.

- `lbl_801221C0` -> `os_check_heap_assert_str_table`
- Size: `0x3F0` bytes.
- Contents: a contiguous table of `OSCheckHeap` failure/assertion strings,
  including heap-array, bounds, allocation-link, alignment, and dump-report
  diagnostics.
- Xref: `src/dolphin/os/OSAllocHead.c` uses the object as a table base when
  formatting heap-check diagnostics; no code bytes or data bytes changed.

Verification: fresh configure+ninja completed with exit 0; resulting
`build/GFZE01/main.dol` SHA-1 is
`421c88106697d3275a3fc26fb7a01bf6d816b271`.
