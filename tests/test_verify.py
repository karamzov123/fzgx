from fzgx import verify as v


def test_bisect_isolates_bad_units(monkeypatch):
    bad = {"c", "f"}
    state = {}

    def fake_set_status(p, sources, status):
        for s in sources:
            state[s] = status

    def fake_relink(p):
        return not any(state.get(k) == "matching" for k in bad)

    monkeypatch.setattr(v, "_set_status", fake_set_status)
    monkeypatch.setattr(v, "_relink", fake_relink)
    keys = list("abcdefg")
    good, rejected = v._bisect(None, keys, {k: k for k in keys})
    assert set(rejected) == bad
    assert set(good) == set(keys) - bad
    assert all(state[k] == "matching" for k in good)


def test_bisect_all_good_is_one_relink(monkeypatch):
    calls = []
    monkeypatch.setattr(v, "_set_status", lambda p, s, st: None)
    monkeypatch.setattr(v, "_relink", lambda p: calls.append(1) or True)
    good, bad = v._bisect(None, ["a", "b", "c"], {"a": "a", "b": "b", "c": "c"})
    assert (good, bad, len(calls)) == (["a", "b", "c"], [], 1)
