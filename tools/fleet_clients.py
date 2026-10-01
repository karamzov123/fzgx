#!/usr/bin/env python3
"""Session-local provider launch policy; never edit personal auth/config."""
from __future__ import annotations
import json
from pathlib import Path
import os
import shlex

ROOT = Path(__file__).resolve().parents[1]
TOOLS = ['write_unit', 'patch_unit', 'check', 'search', 'read_evidence', 'release']

def mcp_config(directory, env):
    directory.mkdir(parents=True, exist_ok=True)
    path = directory / 'mcp.json'
    path.write_text(json.dumps({'mcpServers': {'fzgx': {
        'command': str(ROOT / '.venv/bin/python'),
        'args': [str(ROOT / 'tools/fleet_mcp.py')],
        'env': {key: env[key] for key in ('FZGX_SYMBOL', 'FZGX_AGENT_ID', 'FZGX_HARNESS', 'FZGX_MODEL', 'FZGX_RESULT_FILE')}
    }}}, indent=2) + '\n')
    path.chmod(0o600)
    return path

def claude_command(prompt, model, directory, env):
    config = mcp_config(directory, env)
    return ['claude', '-p', prompt, '--model', model, '--effort', 'high',
        '--restricted', '--tools', '', '--allowedTools', ','.join('mcp__fzgx__' + t for t in TOOLS),
        '--mcp-config', str(config), '--strict-mcp-config', '--setting-sources', '',
        '--permission-mode', 'dontAsk', '--permission-prompts', 'none',
        '--disable-slash-commands', '--no-chrome', '--no-session-persistence',
        '--output-format', 'stream-json', '--verbose', '--max-turns', '40',
        '--system-prompt', (ROOT / 'tools/codex_matcher.md').read_text()]

def command(family, prompt, model, directory, env):
    if family == 'claude':
        return claude_command(prompt, model, directory, env)
    if family == 'cline':
        seed = directory / 'prompt.txt'
        seed.write_text(prompt)
        return ['node', str(ROOT / 'tools/fleet_cline.mjs'), str(seed)]
    if family == 'agy':
        # Project plugins otherwise merge the personal broad fzgx server into
        # the model context. Project a session-local HOME with the SAME account
        # state and only the six-tool server; personal config is never edited.
        import shutil
        original_home = Path.home()
        home = directory / 'home'
        config_dir = home / '.gemini/config'
        state_dir = home / '.gemini/antigravity-cli'
        config_dir.mkdir(parents=True, exist_ok=True);state_dir.mkdir(parents=True, exist_ok=True)
        home.chmod(0o700)
        for source, target in [(original_home/'.gemini/config/config.json', config_dir/'config.json'),
                               (original_home/'.gemini/antigravity-cli/jetski_state.pbtxt',state_dir/'jetski_state.pbtxt'),
                               (original_home/'.gemini/antigravity-cli/installation_id',state_dir/'installation_id')]:
            if source.exists() and not target.exists():
                shutil.copy2(source,target);target.chmod(0o600)
        binaries=original_home/'.gemini/antigravity-cli/bin'
        if binaries.exists() and not (state_dir/'bin').exists():
            (state_dir/'bin').symlink_to(binaries,target_is_directory=True)
        config = json.loads(mcp_config(directory, env).read_text())
        (config_dir/'mcp_config.json').write_text(json.dumps(config))
        server='fzgx'
        # Print mode soft-denies call_mcp_tool unless each bound tool is granted
        # here; the PreToolUse guard below still denies everything else.
        (state_dir/'settings.json').write_text(json.dumps({'agentMode':'accept-edits','model':model,
            'permissions':{'allow':[f'mcp({server}/{tool})' for tool in TOOLS]}}))
        env.update(HOME=str(home), FZGX_MCP_SERVER=server, FZGX_GUARD_LOG=str(directory / 'guard.jsonl'))
        agents=directory/'.agents';agents.mkdir(exist_ok=True)
        hook = {'type':'command', 'command':' '.join(shlex.quote(str(x)) for x in [ROOT / '.venv/bin/python', ROOT / 'tools/fleet_agy_guard.py', server, directory / 'guard.jsonl']), 'timeout':10}
        (agents / 'hooks.json').write_text(json.dumps({'fleet-bound-policy': {'PreToolUse':[{'matcher':'*','hooks':[hook]}]}}))
        instruction = ('You are a function-bound decompilation matcher. The assigned source/assembly/diff is below. '
            'Your ONLY tool server is fzgx and it exposes write_unit(source), patch_unit(old,new), check(versions), '
            'search(), read_evidence(section,cursor), release(reason). Use call_mcp_tool with ServerName=fzgx and those exact '
            'ToolName values. Identity is bound by the host: NEVER pass symbol/agent parameters. '
            'Do not ask for read_unit, context, claim, inventory, resources, shell, files, web, or user input. '
            'All those operations are hard-denied. Source is already in the assignment; continue from it.\n')
        # AGY has no system-prompt flag: the matcher rules travel in the prompt.
        instruction += (ROOT / 'tools/codex_matcher.md').read_text() + '\n'
        return ['agy', '-p', instruction+prompt, '--model', model, '--effort', 'high', '--mode', 'accept-edits',
                '--sandbox', '--disable-slash-commands', '--output-format', 'stream-json', '--print-timeout', '1800s',
                '--log-file', str(directory / 'agy-runtime.log')]
    raise ValueError('Unknown provider transport')
