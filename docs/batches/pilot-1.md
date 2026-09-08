# Pilot wave 1 — no-shell matchers, Haiku 4.5 (2026-09-08)

8 functions from main.rel, 4–88 bytes, one `matcher` subagent each
(tools: Read + fzgx MCP; no shell). Orchestrator: Fable 5.1 in Claude Code.

| Function | Size | Result | Checks | Notes |
|---|---|---|---|---|
| fn_1_46EA8 | 12 | matched | 1 | store u32 arg to global |
| fn_1_7899C | 16 | matched | 1 | zero first byte of global |
| fn_1_B2770 | 12 | matched | 2 | pointer had to be 2nd parameter (r4) |
| fn_1_105744 | 36 | matched | 3 | s16 -1 store; declaration order fixed regs |
| fn_1_1310FC | 88 | matched | 3 | conditional init, call, field updates |
| fn_1_D3F88 | 84 | matched | 2 | countdown loop over 0xAC-byte items; s32 count |
| fn_1_8CA20 | 20 | released @100% | 1 | correct C; submit lost to the split/link race (fixed: single build lock) |
| fn_1_411A4 | 48 | released @98.8% | 6 | callee signature `(idx, table[idx])`; agent misread r4 as a quirk → idiom added |

Match rate 6/8 (7/8 correct C). Mean checks per match: 2.0. Wall clock per
agent ≈ 1–2 min. Token/cost accounting per agent is not yet captured by the
harness; estimate from context size (≈ 4k tokens bundle + ≈ 1.5k per check).

Earlier shell-enabled wave (same day): 3/3 matched on tiny functions before an
agent ran `rm -rf build`; that wave was aborted and the surface locked down.

Findings folded into tooling:
- one lock for every ninja invocation (`check` had its own, letting `dtk split`
  rewrite objects during another agent's relink);
- MCP server now runs the CLI per call so library fixes apply without restart;
- idiom: the register a value lands in before a `bl` is the argument position.
