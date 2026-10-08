"""Pure deterministic-search evidence keys and fail-safe local cache helpers."""
from __future__ import annotations

import hashlib
import json
import os
import tempfile
from pathlib import Path
from typing import Any, Mapping


def _canonical(value: Any) -> bytes:
    try:
        return json.dumps(value, sort_keys=True, separators=(",", ":"), ensure_ascii=False, allow_nan=False).encode("utf-8")
    except (TypeError, ValueError) as exc:
        raise ValueError("value must be canonical JSON data") from exc


def key_for(*, symbol: str, source_sha: str, header_closure_sha: str,
            compiler_identity: str, flags: Any, context_sha: str,
            hypothesis_signature: Any) -> str:
    """Return a content/context key; filenames are deliberately not inputs."""
    required = {"symbol": symbol, "source_sha": source_sha,
                "header_closure_sha": header_closure_sha,
                "compiler_identity": compiler_identity, "context_sha": context_sha}
    if any(not isinstance(v, str) or not v.strip() for v in required.values()):
        raise ValueError("all identity fields must be non-empty strings")
    payload = dict(required, flags=flags, hypothesis_signature=hypothesis_signature)
    encoded = _canonical(payload)
    return hashlib.sha256(encoded).hexdigest()


def eligible(record: Any, key: str, required_budget: int) -> bool:
    """Whether exact-key evidence safely proves an exhaustive zero-closure search."""
    if not isinstance(record, Mapping) or not isinstance(key, str) or not key:
        return False
    budget = record.get("budget")
    closures = record.get("verified_closures")
    return (record.get("key") == key and isinstance(budget, int) and not isinstance(budget, bool)
            and isinstance(required_budget, int) and not isinstance(required_budget, bool)
            and budget >= required_budget and required_budget > 0
            and isinstance(closures, int) and not isinstance(closures, bool) and closures == 0
            and isinstance(record.get("searched"), bool) and record["searched"] is True)


def _record_path(directory: os.PathLike[str] | str, key: str) -> Path:
    if not isinstance(key, str) or len(key) != 64 or any(c not in "0123456789abcdef" for c in key):
        raise ValueError("invalid evidence key")
    return Path(directory) / (key + ".json")


def read_record(directory: os.PathLike[str] | str, key: str) -> dict[str, Any] | None:
    """Read valid matching record; absent, malformed, or inaccessible means no evidence."""
    try:
        path = _record_path(directory, key)
        data = json.loads(path.read_text(encoding="utf-8"))
        if not isinstance(data, dict) or data.get("key") != key:
            return None
        return data
    except (OSError, ValueError, TypeError, UnicodeError):
        return None


def write_record(directory: os.PathLike[str] | str, key: str, record: Mapping[str, Any]) -> bool:
    """Atomically persist a JSON record. Return False on any invalid input or I/O failure."""
    temp_name = None
    try:
        path = _record_path(directory, key)
        if not isinstance(record, Mapping) or record.get("key") != key:
            return False
        data = _canonical(dict(record))
        path.parent.mkdir(parents=True, exist_ok=True)
        fd, temp_name = tempfile.mkstemp(prefix="." + key + ".", suffix=".tmp", dir=str(path.parent))
        with os.fdopen(fd, "wb") as stream:
            stream.write(data)
            stream.flush()
            os.fsync(stream.fileno())
        os.replace(temp_name, path)
        temp_name = None
        return True
    except (OSError, ValueError, TypeError, UnicodeError):
        return False
    finally:
        if temp_name is not None:
            try:
                os.unlink(temp_name)
            except OSError:
                pass
