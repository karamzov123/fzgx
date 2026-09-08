"""SQLite work ledger: who owns which function, what it cost, what is blocked.

Lives in build/fzgx/ledger.db (gitignored). `snapshot()` writes the committed
state/ledger.json so history survives machines and sessions.
"""

from __future__ import annotations

import json
import sqlite3
import time
from pathlib import Path
from typing import Dict, Iterable, List, Optional

from .project import ROOT

SCHEMA = """
CREATE TABLE IF NOT EXISTS functions (
  symbol TEXT PRIMARY KEY, module TEXT, unit TEXT, addr INTEGER, size INTEGER,
  status TEXT CHECK(status IN ('unmatched','claimed','matched','blocked','seeded')) DEFAULT 'unmatched',
  attempts INTEGER DEFAULT 0, best_percent REAL DEFAULT 0,
  claimed_by TEXT, claimed_at INTEGER, claim_ttl INTEGER,
  matched_commit TEXT, blocked_issue INTEGER, blocked_reason TEXT
);
CREATE TABLE IF NOT EXISTS attempts (
  id INTEGER PRIMARY KEY, symbol TEXT, agent TEXT, harness TEXT, model TEXT,
  started INTEGER, ended INTEGER, checks INTEGER DEFAULT 0, final_percent REAL,
  tokens_in INTEGER DEFAULT 0, tokens_out INTEGER DEFAULT 0, cost_usd REAL DEFAULT 0,
  outcome TEXT, notes TEXT, best_body_path TEXT
);
CREATE TABLE IF NOT EXISTS names (
  id INTEGER PRIMARY KEY, kind TEXT, target TEXT, proposed TEXT, rationale TEXT,
  proposed_by TEXT, status TEXT DEFAULT 'pending', applied_commit TEXT
);
CREATE TABLE IF NOT EXISTS batches (
  id TEXT PRIMARY KEY, started INTEGER, ended INTEGER, budget_usd REAL,
  spent_usd REAL DEFAULT 0, matched INTEGER DEFAULT 0, released INTEGER DEFAULT 0,
  blocked INTEGER DEFAULT 0
);
CREATE INDEX IF NOT EXISTS idx_functions_status ON functions(status);
CREATE INDEX IF NOT EXISTS idx_attempts_symbol ON attempts(symbol);
"""


class Ledger:
    def __init__(self, path: Optional[Path] = None) -> None:
        self.path = path or ROOT / "build" / "fzgx" / "ledger.db"
        self.path.parent.mkdir(parents=True, exist_ok=True)
        self.db = sqlite3.connect(self.path, timeout=30, isolation_level=None)
        self.db.row_factory = sqlite3.Row
        self.db.execute("PRAGMA journal_mode=WAL")
        self.db.execute("PRAGMA busy_timeout=30000")
        self.db.executescript(SCHEMA)

    # ------------------------------------------------------------- inventory
    def sync_functions(self, rows: Iterable[dict]) -> int:
        """Insert unknown functions (status unmatched); never overwrite status."""
        n = 0
        with self.db:
            self.db.execute("BEGIN")
            for r in rows:
                cur = self.db.execute(
                    "INSERT OR IGNORE INTO functions(symbol,module,unit,addr,size) VALUES(?,?,?,?,?)",
                    (r["symbol"], r["module"], r.get("unit"), r["addr"], r["size"]),
                )
                n += cur.rowcount
                self.db.execute("UPDATE functions SET unit=?, size=? WHERE symbol=?",
                                (r.get("unit"), r["size"], r["symbol"]))
        return n

    def get(self, symbol: str) -> Optional[sqlite3.Row]:
        return self.db.execute("SELECT * FROM functions WHERE symbol=?", (symbol,)).fetchone()

    def list(self, module: Optional[str] = None, status: Optional[str] = None,
             limit: Optional[int] = None) -> List[sqlite3.Row]:
        q, args = "SELECT * FROM functions WHERE 1=1", []
        if module:
            q += " AND module=?"; args.append(module)
        if status:
            q += " AND status=?"; args.append(status)
        q += " ORDER BY module, addr"
        if limit:
            q += f" LIMIT {int(limit)}"
        return self.db.execute(q, args).fetchall()

    # ---------------------------------------------------------------- claims
    def expire_claims(self) -> int:
        now = int(time.time())
        with self.db:
            cur = self.db.execute(
                "UPDATE functions SET status='unmatched', claimed_by=NULL, claimed_at=NULL, claim_ttl=NULL "
                "WHERE status='claimed' AND claimed_at + claim_ttl < ?", (now,))
        return cur.rowcount

    def claim(self, symbol: str, agent: str, ttl: int, max_attempts: int) -> sqlite3.Row:
        self.expire_claims()
        with self.db:
            self.db.execute("BEGIN IMMEDIATE")
            row = self.get(symbol)
            if row is None:
                raise LookupError(f"{symbol}: not in ledger (run `fzgx sync`)")
            if row["status"] != "unmatched":
                raise PermissionError(f"{symbol}: status is {row['status']}"
                                      + (f" (by {row['claimed_by']})" if row["claimed_by"] else ""))
            if row["attempts"] >= max_attempts:
                raise PermissionError(f"{symbol}: attempt cap {max_attempts} reached; needs triage")
            self.db.execute(
                "UPDATE functions SET status='claimed', claimed_by=?, claimed_at=?, claim_ttl=? WHERE symbol=?",
                (agent, int(time.time()), ttl, symbol))
            self.db.execute(
                "INSERT INTO attempts(symbol, agent, started) VALUES(?,?,?)",
                (symbol, agent, int(time.time())))
        return self.get(symbol)

    def current_attempt(self, symbol: str) -> Optional[sqlite3.Row]:
        return self.db.execute(
            "SELECT * FROM attempts WHERE symbol=? AND ended IS NULL ORDER BY id DESC LIMIT 1",
            (symbol,)).fetchone()

    def bump_checks(self, symbol: str, percent: float) -> None:
        with self.db:
            self.db.execute(
                "UPDATE attempts SET checks=checks+1, final_percent=? WHERE symbol=? AND ended IS NULL",
                (percent, symbol))
            self.db.execute(
                "UPDATE functions SET best_percent=MAX(best_percent, ?) WHERE symbol=?", (percent, symbol))

    def finish(self, symbol: str, outcome: str, status: str, notes: str = "",
               commit: Optional[str] = None, body_path: Optional[str] = None,
               model: Optional[str] = None, harness: Optional[str] = None,
               tokens_in: int = 0, tokens_out: int = 0, cost_usd: float = 0.0) -> None:
        with self.db:
            self.db.execute(
                "UPDATE attempts SET ended=?, outcome=?, notes=?, best_body_path=?, model=COALESCE(?,model), "
                "harness=COALESCE(?,harness), tokens_in=tokens_in+?, tokens_out=tokens_out+?, cost_usd=cost_usd+? "
                "WHERE symbol=? AND ended IS NULL",
                (int(time.time()), outcome, notes, body_path, model, harness, tokens_in, tokens_out, cost_usd, symbol))
            self.db.execute(
                "UPDATE functions SET status=?, attempts=attempts+1, claimed_by=NULL, claimed_at=NULL, "
                "claim_ttl=NULL, matched_commit=COALESCE(?, matched_commit) WHERE symbol=?",
                (status, commit, symbol))

    def block(self, symbol: str, reason: str, issue: Optional[int]) -> None:
        with self.db:
            self.db.execute(
                "UPDATE functions SET status='blocked', blocked_reason=?, blocked_issue=? WHERE symbol=?",
                (reason, issue, symbol))

    def unblock(self, symbol: str) -> None:
        with self.db:
            self.db.execute(
                "UPDATE functions SET status='unmatched', blocked_reason=NULL, attempts=0 WHERE symbol=?",
                (symbol,))

    # ----------------------------------------------------------------- names
    def propose_names(self, symbol: str, agent: str, proposals: List[dict]) -> None:
        with self.db:
            for p in proposals:
                self.db.execute(
                    "INSERT INTO names(kind,target,proposed,rationale,proposed_by) VALUES(?,?,?,?,?)",
                    (p.get("kind", "function"), p.get("target", symbol), p["name"],
                     p.get("rationale", ""), agent))

    def pending_names(self) -> List[sqlite3.Row]:
        return self.db.execute("SELECT * FROM names WHERE status='pending' ORDER BY id").fetchall()

    # ------------------------------------------------------------- reporting
    def summary(self) -> Dict[str, Dict[str, int]]:
        out: Dict[str, Dict[str, int]] = {}
        for r in self.db.execute(
                "SELECT module, status, COUNT(*) n, SUM(size) bytes FROM functions GROUP BY module, status"):
            out.setdefault(r["module"], {})[r["status"]] = r["n"]
            out[r["module"]][f"{r['status']}_bytes"] = r["bytes"] or 0
        return out

    def costs(self) -> sqlite3.Row:
        return self.db.execute(
            "SELECT COUNT(*) attempts, SUM(checks) checks, SUM(tokens_in) tokens_in, "
            "SUM(tokens_out) tokens_out, SUM(cost_usd) cost_usd FROM attempts").fetchone()

    def snapshot(self, path: Optional[Path] = None) -> Path:
        path = path or ROOT / "state" / "ledger.json"
        path.parent.mkdir(parents=True, exist_ok=True)
        data = {
            "generated": int(time.time()),
            "functions": [dict(r) for r in self.db.execute("SELECT * FROM functions ORDER BY module, addr")],
            "attempts": [dict(r) for r in self.db.execute("SELECT * FROM attempts ORDER BY id")],
            "names": [dict(r) for r in self.db.execute("SELECT * FROM names ORDER BY id")],
            "batches": [dict(r) for r in self.db.execute("SELECT * FROM batches ORDER BY started")],
        }
        path.write_text(json.dumps(data, indent=1) + "\n")
        return path

    def restore(self, path: Optional[Path] = None) -> int:
        """Load a committed snapshot into an empty/new ledger (fresh clone or machine)."""
        path = path or ROOT / "state" / "ledger.json"
        if not path.exists():
            return 0
        data = json.loads(path.read_text())
        n = 0
        with self.db:
            self.db.execute("BEGIN")
            for table in ("functions", "attempts", "names", "batches"):
                for row in data.get(table, []):
                    cols = ",".join(row.keys())
                    qs = ",".join("?" * len(row))
                    n += self.db.execute(
                        f"INSERT OR REPLACE INTO {table}({cols}) VALUES({qs})", list(row.values())).rowcount
        return n
