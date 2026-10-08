"""Bound-tool receipts, independent of provider chatter and log timestamps."""
from __future__ import annotations
import json
import math
import os
import re
import fcntl
from pathlib import Path

COMPILER_TOOLS = frozenset(('write_unit', 'patch_unit', 'check', 'search'))

def append_receipt(path, row):
    """Serialize reconnecting MCP writers; flush a whole record under the lock."""
    with path.open('a') as stream:
        fcntl.flock(stream.fileno(),fcntl.LOCK_EX)
        try:
            stream.write(json.dumps(row)+'\n')
            stream.flush()
        finally:
            fcntl.flock(stream.fileno(),fcntl.LOCK_UN)


def tool_succeeded(result) -> bool:
    """Recognize backend success, including edit results containing a check."""
    if isinstance(result, str):
        try:
            result = json.loads(result)
        except ValueError:
            # The bound check tool uses api.format_check(), including version
            # tables followed by a selected check. Require its exact verdict line;
            # arbitrary prose and CHECK FAILED are not compiler progress.
            return any(re.fullmatch(r'[A-Za-z_][\w:]*: \d+(?:\.\d+)?%  unit=.*  (?:no match|MATCH(?: \(pool\))?)',
                                    line) for line in result.splitlines())
    if not isinstance(result, dict) or result.get('ok') is not True:
        return False
    check = result.get('check')
    return not isinstance(check, dict) or check.get('ok') is True


class ToolProgress:
    """Track uniquely paired successful operations; fail closed on lost receipts."""
    def __init__(self, path: Path, started: float, timeout_s: float):
        self.path = path
        self.last_progress = started
        self.timeout_s = timeout_s
        self.active: dict[str, str] = {}
        self.offset = 0
        self.file_identity = None
        self.uncertain = False
        self.legacy_sequence = 0

    def refresh(self):
        try:
            with self.path.open('rb') as stream:
                stat = os.fstat(stream.fileno())
                identity = (stat.st_dev, stat.st_ino)
                if (self.file_identity not in (None, identity)
                        or stat.st_size < self.offset):
                    # Rotation/truncation is not evidence that an old tool drained.
                    self.uncertain = True
                    self.active.clear()
                    self.offset = 0
                self.file_identity = identity
                stream.seek(self.offset)
                while True:
                    line = stream.readline()
                    if not line or not line.endswith(b'\n'):
                        break
                    self.offset = stream.tell()
                    try:
                        row = json.loads(line)
                        if not isinstance(row, dict):
                            self.uncertain = True
                            continue
                        tool, phase = row.get('tool'), row.get('phase')
                        timestamp = float(row.get('timestamp', 0))
                        if not math.isfinite(timestamp):
                            self.uncertain = True
                            continue
                    except (ValueError, TypeError, OverflowError):
                        self.uncertain = True
                        continue
                    if not isinstance(tool, str):
                        self.uncertain = True
                        continue
                    operation = row.get('operation_id')
                    if operation is not None and not isinstance(operation, str):
                        self.uncertain = True
                        continue
                    if phase == 'start':
                        if operation is None:
                            self.legacy_sequence += 1
                            operation = f'legacy:{tool}:{self.legacy_sequence}'
                        self.active.setdefault(operation, tool)
                    elif phase == 'end':
                        if operation is None:
                            operation = next((key for key, name in self.active.items()
                                              if name == tool and key.startswith('legacy:')), None)
                        if operation is None or self.active.get(operation) != tool:
                            continue
                        self.active.pop(operation)
                        if tool in COMPILER_TOOLS and row.get('success') is True:
                            self.last_progress = max(self.last_progress, timestamp)
                    else:
                        self.uncertain = True
        except FileNotFoundError:
            if self.file_identity is not None:
                self.uncertain = True

    def expired(self, now: float) -> bool:
        self.refresh()
        return (self.timeout_s > 0 and not self.active and not self.uncertain
                and now - self.last_progress > self.timeout_s)
