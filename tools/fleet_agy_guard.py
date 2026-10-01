#!/usr/bin/env python3
"""AGY PreToolUse hard-deny policy: only this worker's bound MCP tools."""
from __future__ import annotations
import json
import os
from pathlib import Path
import sys
import time
TOOLS = {'write_unit', 'patch_unit', 'check', 'read_evidence', 'release'}

def decide(payload, server):
    call = payload.get('toolCall') or {}
    name = call.get('name', '')
    args = call.get('args') or {}
    if not server or not isinstance(args, dict):
        return False
    # MCP tools may be directly namespaced or carried by the MCP step wrapper.
    selected = {server, 'fleet-bound_' + server}
    if name in {f'mcp_{s}_{t}' for s in selected for t in TOOLS} | {f'mcp__{s}__{t}' for s in selected for t in TOOLS}:
        return True
    if name in ('call_mcp_tool', 'mcp_tool', 'mcp_tool_call', 'use_mcp_tool'):
        selected_server = args.get('serverName', args.get('ServerName', args.get('server_name')))
        selected_tool = args.get('toolName', args.get('ToolName', args.get('tool_name')))
        return selected_server in selected and selected_tool in TOOLS
    return False

def main():
    try:
        payload = json.load(sys.stdin)
        server = sys.argv[1] if len(sys.argv)>1 else os.environ.get('FZGX_MCP_SERVER', '')
        allowed = decide(payload, server)
        call = payload.get('toolCall') or {}
        log = sys.argv[2] if len(sys.argv)>2 else os.environ.get('FZGX_GUARD_LOG')
        if log:
            with Path(log).open('a') as output:
                output.write(json.dumps(dict(timestamp=time.time(), tool=call.get('name'),
                    arg_keys=list((call.get('args') or {}).keys()), args={k:v for k,v in (call.get('args') or {}).items()
                    if 'server' in k.lower() or 'tool' in k.lower()}, allowed=allowed)) + '\n')
    except Exception:
        allowed = False
    print(json.dumps(dict(decision='allow' if allowed else 'deny',
        reason='Only the five supervisor-bound decompilation tools are permitted; no shell, file, web, subagent or interactive tools.')))

if __name__ == '__main__':
    main()
