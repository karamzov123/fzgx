// Fail closed before native tool execution; MCP identity stays host-bound.
import { appendFileSync } from 'node:fs';
const allowed = new Set(['write_unit', 'patch_unit', 'check', 'search', 'read_evidence', 'release'].map(x => `fzgx_${x}`));
export default async function FleetGuard() {
  const receipt = process.env.FZGX_OPENCODE_GUARD_LOG;
  if (!receipt) throw new Error('Missing bound guard receipt path');
  const record = row => appendFileSync(receipt, JSON.stringify(row) + '\n', { mode: 0o600 });
  record({ event: 'ready', pid: process.pid, nonce: process.env.FZGX_OPENCODE_GUARD_NONCE });
  return {
    'tool.execute.before': async input => {
      const allow = allowed.has(input.tool);
      record({ event: 'tool', tool: input.tool, allow });
      if (!allow) throw new Error('FZGX_BOUND_DENIED: native tool execution prohibited');
    },
  };
}
