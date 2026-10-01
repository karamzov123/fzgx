# CLI self-test sweep — 2026-08-27 (fleet/tool, supervisor tool-health mandate)

Run by tooling worker (`tool`) as a proactive CLI health sweep across every
`tools/*.py` exposing a `--self-test` flag. This is live runtime evidence, not
a re-read of prior board lines.

## Command
```
cd ~/fzgx-wt/tool
for t in find_xrefs similar emit_m2c_asm natc_loop natc_feedback \
         find_slice_unit carve_guard; do
  uv run --with capstone --no-project python3 tools/$t.py --self-test
done
```
(Integration-domain self-tests `natc_compile` / `natc_gate` / `natc_preflight`
are NOT run here: they require a full `build/` + the gate, which the contract
bars the tool worker from invoking — integ-only domain, per contract L13.)

## Result — 7/7 runnable self-tests PASS
...[truncated]