#!/usr/bin/env python3
"""Six function-bound MCP tools for non-Codex fleet clients.

The supervisor supplies identity, work copy and initial evidence. Models cannot
claim, submit, select other symbols, access files, or invoke arbitrary commands.
"""
from __future__ import annotations
import json
import asyncio
import os
from pathlib import Path
import time
import uuid
from fleet_activity import tool_succeeded, append_receipt
from mcp.server.fastmcp import FastMCP
import fzgx_mcp as backend

SYMBOL = os.environ.get('FZGX_SYMBOL', '')
AGENT = os.environ.get('FZGX_AGENT_ID', '')
RESULT = os.environ.get('FZGX_RESULT_FILE', '')
EVENTS = Path(RESULT).with_name(f'{SYMBOL}.tools.jsonl') if RESULT else None
mcp = FastMCP('fzgx', instructions='Continue the assigned function. Only bound edit/check/search/evidence/release tools exist; matching and limits terminate automatically.')

def binding():
    if not SYMBOL or not AGENT or not RESULT:
        raise RuntimeError('A supervisor-owned function assignment is required')
    path = Path(RESULT).resolve()
    path.relative_to((backend.ROOT / '.fzgx/runs').resolve())
    if path.name != SYMBOL + '.terminal.json':
        raise RuntimeError('Terminal result does not belong to the assigned function')
    return SYMBOL, AGENT

async def invoke(name, operation):
    binding()
    operation_id=uuid.uuid4().hex
    success=False
    def event(phase):
        if EVENTS:
            append_receipt(EVENTS,dict(timestamp=time.time(), tool=name, phase=phase,
                    symbol=SYMBOL, agent=AGENT, operation_id=operation_id,
                    success=success if phase=='end' else None))
    event('start')
    job = asyncio.create_task(operation())
    try:
        result=await asyncio.shield(job)
        success=tool_succeeded(result)
        return result
    except asyncio.CancelledError:
        result=await job
        success=tool_succeeded(result)
        raise
    finally:
        event('end')

@mcp.tool()
async def write_unit(source: str) -> dict:
    """Replace your complete assigned C source, compile, and return the retail diff."""
    return await invoke('write_unit', lambda: backend.write_unit(SYMBOL, AGENT, source))

@mcp.tool()
async def patch_unit(old: str, new: str) -> dict:
    """Replace one unique occurrence in your assigned C source; compile and diff."""
    return await invoke('patch_unit', lambda: backend.patch_unit(SYMBOL, AGENT, old, new))

@mcp.tool()
async def check(versions: str = '') -> str:
    """Check your assigned source; optionally probe and retain the best compiler."""
    return await invoke('check', lambda: backend.check(SYMBOL, versions or None))

@mcp.tool()
async def read_evidence(section: str = 'diff', cursor: int = 0) -> str:
    """Page through the cached diff or proven retail data without compiling."""
    return await invoke('read_evidence', lambda: backend.read_evidence(SYMBOL, section, cursor))

@mcp.tool()
async def search() -> dict:
    """Let the tooling permute your current source mechanically (declaration order, type and sign flips, optimizer pragmas, literal-pool priming; thousands of compiles in seconds). Use it once your body is at 80% or better and the remaining rows are register, pool or `L` rows. A match is accepted; a better body becomes your work copy; three uses per attempt."""
    return await invoke('search', lambda: backend.search(SYMBOL, AGENT))


@mcp.tool()
async def release(reason: str) -> dict:
    """Save the best candidate and stop for a concrete technical obstacle."""
    return await invoke('release', lambda: backend.release(SYMBOL, AGENT, reason))

if __name__ == '__main__':
    binding()
    mcp.run()
