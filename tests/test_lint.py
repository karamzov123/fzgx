from pathlib import Path

from fzgx.lint import lint_file


def _lint(tmp_path: Path, text: str):
    p = tmp_path / "unit.c"
    p.write_text(text)
    return [(r, l) for r, l, _ in lint_file(p)]


def test_address_literal_flagged(tmp_path):
    assert _lint(tmp_path, "u32 *p = (u32 *)0x80123456;\n") == [("A1", 1), ("A2", 1)]


def test_allow_comment(tmp_path):
    assert _lint(tmp_path, "u32 x = 0x80123456; // fzgx-allow: A1 known constant\n") == []


def test_small_constants_ok(tmp_path):
    assert _lint(tmp_path, "u32 x = 0x1234; u32 y = 0xFFFFFFFF; u32 z = 0x7FFFFFFF;\n") == []


def test_goto_and_volatile_need_comment(tmp_path):
    src = "void f(void) {\n    goto done;\n    volatile u32 *r;\ndone:\n    return;\n}\n"
    assert _lint(tmp_path, src) == [("S1", 2), ("S2", 3)]
    src2 = "void f(void) {\n    // irreducible loop in retail\n    goto done;\ndone:\n    return;\n}\n"
    assert _lint(tmp_path, src2) == []
