from pathlib import Path

from fzgx.carve import add_force_active


class FakeProject:
    def __init__(self, tmp: Path):
        self.config_dir = tmp


CFG = """object: sys/main.dol
symbols: config/X/symbols.txt
modules:
- object: files/a.rel
  name: alpha
  symbols: config/X/alpha/symbols.txt
- object: files/b.rel
  hash: abc
  name: beta
  force_active:
  - fn_old
  symbols: config/X/beta/symbols.txt
"""


def test_force_active_insert_and_append(tmp_path):
    (tmp_path / "config.yml").write_text(CFG)
    p = FakeProject(tmp_path)
    assert add_force_active(p, "alpha", "fn_a") is True
    assert add_force_active(p, "beta", "fn_b") is True
    assert add_force_active(p, "beta", "fn_b") is False  # idempotent
    assert add_force_active(p, "main", "OSInit") is True
    text = (tmp_path / "config.yml").read_text()
    assert "  name: alpha\n  force_active:\n  - fn_a\n" in text
    assert "  force_active:\n  - fn_b\n  - fn_old\n" in text
    assert text.startswith("object: sys/main.dol\nsymbols: config/X/symbols.txt\nforce_active:\n- OSInit\nmodules:\n")
