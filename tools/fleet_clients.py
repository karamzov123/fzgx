#!/usr/bin/env python3
"""Session-local provider launch policy; never edit personal auth/config."""
from __future__ import annotations
import json
from pathlib import Path
import os
import shlex
import subprocess

ROOT = Path(__file__).resolve().parents[1]
TOOLS = ['write_unit', 'patch_unit', 'check', 'search', 'read_evidence', 'release']
# opencode merges every opencode.json up from the working directory, and loads
# AGENTS.md/CLAUDE.md from the same tree. The client's own config root is
# therefore the only safe place to pin a tool-restricted policy, and the client
# must not live under $HOME: ~/AGENTS.md belongs to an unrelated project and
# would otherwise be injected into a matcher prompt.
OPENCODE_ROOT = Path(os.environ.get('XDG_RUNTIME_DIR', f'/run/user/{os.getuid()}')) / 'fzgx-opencode-clients'
# One shared config root for every session. opencode bootstraps ~62MB of
# node_modules into whatever XDG_CONFIG_HOME points at, and /run/user is tmpfs,
# so a per-session config root would spend 62MB of RAM per matcher session. The
# config needs nothing per-session: fleet_mcp.py reads its binding from the
# inherited environment, which the runner already sets on the client process.
OPENCODE_CONFIG = Path(os.environ.get('XDG_RUNTIME_DIR', f'/run/user/{os.getuid()}')) / 'fzgx-opencode-config'
OPENCODE_VARIANT = 'xhigh'
# Resolution order matters and PATH is not enough. The free models answer 403
# "FreeTierError: OpenCode's free tier can only be used from within OpenCode" on
# any build that is not the official client, and the fleet daemon runs with a
# pinned PATH that resolves /usr/bin/opencode (v2), which cannot run them at
# all. Prefer the official client, then anything that still supports --pure.
OPENCODE_CANDIDATES = (Path.home() / '.opencode/bin/opencode', Path('/usr/bin/opencode'))
_resolved = None

def opencode_binary():
    """First candidate that exists and still supports --pure. Cached per process."""
    global _resolved
    if _resolved is not None:
        return _resolved
    tried = []
    for candidate in OPENCODE_CANDIDATES:
        if not (candidate.exists() and os.access(candidate, os.X_OK)):
            tried.append(f'{candidate}: absent')
            continue
        try:
            # Help lands on stderr for some builds and stdout for others.
            probe = subprocess.run([str(candidate), 'run', '--help'], capture_output=True,
                                   text=True, timeout=60)
            version = subprocess.run([str(candidate), '--version'], capture_output=True,
                                     text=True, timeout=60).stdout.strip()
            supports_pure = '--pure' in (probe.stdout + probe.stderr)
        except (OSError, subprocess.SubprocessError) as error:
            tried.append(f'{candidate}: {error}')
            continue
        if not supports_pure:
            tried.append(f'{candidate} ({version}): no --pure, cannot pin the toolset')
            continue
        _resolved = str(candidate)
        return _resolved
    raise RuntimeError('No usable opencode client for the free tier: ' + '; '.join(tried))
# Removed from the toolset outright; `permission: deny` alone still advertises
# them, and the model then wastes turns proposing calls it cannot make.
OPENCODE_OFF = ['bash', 'edit', 'write', 'patch', 'read', 'glob', 'grep', 'list',
                'webfetch', 'websearch', 'task', 'todowrite', 'question', 'skill',
                'lsp', 'external_directory', 'invalid', 'multiedit']

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

def client_root(family, agent_id):
    """Working directory for the client process.

    opencode is launched outside $HOME: it loads AGENTS.md/CLAUDE.md from the
    working directory upwards, and ~/AGENTS.md is an unrelated project's
    contract. Every other provider lives under the shared client cache.
    """
    if family == 'opencode':
        return OPENCODE_ROOT / agent_id
    return Path.home() / '.cache/fzgx-agents/clients' / agent_id

def opencode_config(env, model):
    """Shared, session-independent config: one MCP server, one restricted agent.

    Mirrors the AGY sandbox. The personal opencode config grants bash/edit/
    webfetch, and opencode merges config from the whole directory tree, so the
    only way to give a matcher the same six bound tools as every other provider
    is a config root that contains nothing else. XDG_CONFIG_HOME must point at
    it: writing the file alone leaves opencode reading the personal config, and
    the session then runs with a full shell. The client's working directory is
    kept outside $HOME for the same reason (see client_root).

    No per-session values live here. fleet_mcp.py binds from the environment the
    runner puts on the client process, so all four instances can share this one
    root and pay opencode's node_modules bootstrap once.
    """
    import stat
    guarded = model == 'opencode/exo-free'
    config_home = OPENCODE_CONFIG.with_name(OPENCODE_CONFIG.name + '-exo') if guarded else OPENCODE_CONFIG
    config_dir = config_home / 'opencode'
    config_dir.mkdir(parents=True, exist_ok=True)
    contract = (ROOT / 'tools/codex_matcher.md').read_text()
    config = {
        '$schema': 'https://opencode.ai/config.json',
        'mcp': {'fzgx': {
            'type': 'local',
            'command': [str(ROOT / '.venv/bin/python'), str(ROOT / 'tools/fleet_mcp.py')],
            'enabled': True,
            'timeout': 120000,
        }},
        'agent': {'matcher': {
            'mode': 'primary',
            'variant': OPENCODE_VARIANT,
            'steps': 40,
            'tools': {name: False for name in OPENCODE_OFF},
            'permission': {name: 'deny' for name in
                           ('edit', 'bash', 'webfetch', 'websearch', 'task', 'read',
                            'glob', 'grep', 'list', 'external_directory', 'todowrite',
                            'question', 'skill', 'lsp')},
            'prompt': contract,
        }},
    }
    if guarded:
        config['plugin'] = [(ROOT / 'tools/fleet_opencode_guard.mjs').as_uri()]
        # Zen requires advertised bash; the pre-execution hook hard-denies it.
        config['agent']['matcher']['tools'] = {name: True for name in OPENCODE_OFF}
        config['agent']['matcher']['permission'] = {'*': 'allow'}
        config['agent']['matcher']['prompt'] = ('Native tools are advertised only for provider compatibility. '
            'Every native tool, including bash, is hard-blocked before execution and ends your session. '
            'Use ONLY fzgx_write_unit, fzgx_patch_unit, fzgx_check, fzgx_search, '
            'fzgx_read_evidence, fzgx_release. Never call bash, even echo or a no-op.\n' + contract)
        env['FZGX_OPENCODE_GUARD_LOG'] = env['FZGX_RESULT_FILE'] + '.guard.jsonl'
        env['OPENCODE_DISABLE_DEFAULT_PLUGINS'] = 'true'
        env.pop('OPENCODE_PERMISSION', None)
    path = config_dir / 'opencode.json'
    body = json.dumps(config, indent=2) + '\n'
    if not path.exists() or path.read_text() != body:
        tmp = path.with_suffix('.json.tmp')
        tmp.write_text(body)
        tmp.chmod(0o600)
        tmp.replace(path)
    config_dir.chmod(stat.S_IRWXU)
    config_home.chmod(stat.S_IRWXU)
    # HOME is left alone so the free models keep their credentials and store;
    # only config resolution is redirected.
    env['XDG_CONFIG_HOME'] = str(config_home)
    return config_home

def opencode_command(prompt, model, directory, env):
    opencode_config(env, model)
    # Model-independent shared config; each process owns its explicit model pin.
    # Otherwise two different free-model lanes overwrite each other's agent model.
    cmd = [opencode_binary(), 'run', '--model', model, '--agent', 'matcher',
           *([] if model == 'opencode/exo-free' else ['--pure']), '--format', 'json', prompt]
    if model == 'opencode/exo-free':
        return [str(ROOT / '.venv/bin/python'), str(ROOT / 'tools/fleet_opencode_launch.py'), *cmd]
    return cmd

def command(family, prompt, model, directory, env):
    if family == 'claude':
        return claude_command(prompt, model, directory, env)
    if family == 'opencode':
        return opencode_command(prompt, model, directory, env)
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
