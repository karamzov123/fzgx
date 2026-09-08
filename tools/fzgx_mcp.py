#!/usr/bin/env python3
"""MCP server exposing the fzgx matcher surface as tools.

Matcher agents get these tools and nothing else (no shell): they can only
claim a function, read its context, replace their own unit's source, check it
against retail, and submit or release. Every mutation is validated against
the ledger's claim, so an agent cannot touch a unit it does not own.

Registered in .mcp.json (Claude Code) and ~/.codex/config.toml (Codex).
Run from the repository root: `uv run tools/fzgx_mcp.py`. The script's own
directory (tools/) is on sys.path, so the fzgx package imports directly.
"""

from __future__ import annotations

from typing import List, Optional

import anyio
from mcp.server.fastmcp import FastMCP

from fzgx import api
from fzgx.project import Project

mcp = FastMCP("fzgx", instructions="F-Zero GX matching-decompilation oracle. One function per agent: "
              "claim -> context -> write_unit -> check (<= 8) -> submit | release.")
VERSION = "GFZE01"


def _p() -> Project:
    return Project(VERSION)


@mcp.tool()
async def claim(symbol: str, agent: str) -> dict:
    """Claim SYMBOL for AGENT and carve it into its own unit (src/<unit>.c). Fails if claimed, matched, blocked, or at the attempt cap."""
    return await anyio.to_thread.run_sync(lambda: api.claim(_p(), symbol, agent))


@mcp.tool()
async def context(symbol: str, budget_tokens: int = 6000) -> str:
    """Context bundle: retail assembly, referenced symbols with declarations, callers, nearby matched C, current unit source, compiler flags, idioms, rules."""
    return await anyio.to_thread.run_sync(lambda: api.context(_p(), symbol, budget_tokens))


@mcp.tool()
async def read_unit(symbol: str) -> dict:
    """Current source of SYMBOL's unit."""
    return await anyio.to_thread.run_sync(lambda: api.read_unit(_p(), symbol))


@mcp.tool()
async def write_unit(symbol: str, agent: str, source: str) -> dict:
    """Replace the whole source of the unit AGENT has claimed for SYMBOL. Returns lint findings (A1/A2: hardcoded addresses; S1/S2: goto/volatile without justification). Inline asm is rejected."""
    return await anyio.to_thread.run_sync(lambda: api.write_unit(_p(), symbol, agent, source))


@mcp.tool()
async def check(symbol: str, versions: Optional[str] = None, max_diff_lines: int = 80) -> str:
    """Compile SYMBOL's unit and diff against retail. Prints match % and a `target | ours` instruction diff. With versions='all' (or a comma list like 'GC/1.2.5n,GC/1.3.2') compiles under each CodeWarrior version and reports % per version instead."""
    res = await anyio.to_thread.run_sync(lambda: api.check(_p(), symbol, max_diff_lines, versions))
    return api.format_check(res)


@mcp.tool()
async def submit(symbol: str, agent: str, message: str, harness: str = "", model: str = "",
                 mw_version: Optional[str] = None, names: Optional[List[dict]] = None) -> dict:
    """Accept a 100% match: lint, relink every target, verify all 16 hashes, commit. `names` is an optional list of {kind,target,name,rationale} proposals for the librarian. Pass mw_version (e.g. 'GC/1.3') only if check(versions=...) showed that version matches."""
    return await anyio.to_thread.run_sync(
        lambda: api.submit(_p(), symbol, agent, message, harness or None, model or None, mw_version, None, names))


@mcp.tool()
async def release(symbol: str, agent: str, reason: str, harness: str = "", model: str = "") -> dict:
    """Give up on SYMBOL. The best attempt is saved for the next agent; say precisely what still differs."""
    return await anyio.to_thread.run_sync(
        lambda: api.release(_p(), symbol, reason, harness or None, model or None, agent=agent))


@mcp.tool()
async def inventory(module: Optional[str] = None, status: Optional[str] = None,
                    max_size: Optional[int] = None, limit: Optional[int] = 50) -> list:
    """List functions (orchestrator use): filter by module (main, main_rel, ...), status (unmatched|claimed|matched|blocked), max_size bytes."""
    return await anyio.to_thread.run_sync(lambda: api.inventory(_p(), module, status, limit, max_size))


@mcp.tool()
async def report() -> dict:
    """Ledger status totals, costs, and objdiff progress."""
    return await anyio.to_thread.run_sync(lambda: api.report(_p()))


if __name__ == "__main__":
    mcp.run()
