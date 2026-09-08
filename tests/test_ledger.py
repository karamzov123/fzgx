import pytest

from fzgx.ledger import Ledger


@pytest.fixture
def ledger(tmp_path):
    l = Ledger(tmp_path / "ledger.db")
    l.sync_functions([
        {"symbol": "fn_a", "module": "main_rel", "unit": None, "addr": 0x100, "size": 32},
        {"symbol": "fn_b", "module": "main_rel", "unit": None, "addr": 0x120, "size": 64},
    ])
    return l


def test_claim_is_exclusive(ledger):
    ledger.claim("fn_a", "agent1", ttl=1000, max_attempts=3)
    with pytest.raises(PermissionError):
        ledger.claim("fn_a", "agent2", ttl=1000, max_attempts=3)


def test_release_then_reclaim_counts_attempts(ledger):
    ledger.claim("fn_a", "a1", 1000, 3)
    ledger.bump_checks("fn_a", 40.0)
    ledger.finish("fn_a", "released", "unmatched", notes="gave up")
    row = ledger.get("fn_a")
    assert row["status"] == "unmatched" and row["attempts"] == 1 and row["best_percent"] == 40.0
    ledger.claim("fn_a", "a2", 1000, 3)
    ledger.finish("fn_a", "released", "unmatched")
    ledger.claim("fn_a", "a3", 1000, 3)
    ledger.finish("fn_a", "released", "unmatched")
    with pytest.raises(PermissionError, match="attempt cap"):
        ledger.claim("fn_a", "a4", 1000, 3)


def test_expired_claim_is_reclaimable(ledger):
    ledger.claim("fn_b", "slow", ttl=0, max_attempts=3)
    ledger.db.execute("UPDATE functions SET claimed_at = claimed_at - 10 WHERE symbol='fn_b'")
    assert ledger.expire_claims() == 1
    ledger.claim("fn_b", "fast", ttl=1000, max_attempts=3)


def test_matched_and_snapshot_roundtrip(ledger, tmp_path):
    ledger.claim("fn_a", "a1", 1000, 3)
    ledger.finish("fn_a", "matched", "matched", commit="abc123", cost_usd=0.02, tokens_in=100)
    snap = ledger.snapshot(tmp_path / "ledger.json")
    other = Ledger(tmp_path / "other.db")
    other.restore(snap)
    assert other.get("fn_a")["status"] == "matched"
    assert other.costs()["cost_usd"] == pytest.approx(0.02)
    assert other.summary()["main_rel"]["matched"] == 1


def test_sync_never_downgrades_status(ledger):
    ledger.claim("fn_a", "a1", 1000, 3)
    ledger.finish("fn_a", "matched", "matched")
    ledger.sync_functions([{"symbol": "fn_a", "module": "main_rel", "unit": "x.c", "addr": 0x100, "size": 32}])
    assert ledger.get("fn_a")["status"] == "matched"
    assert ledger.get("fn_a")["unit"] == "x.c"
