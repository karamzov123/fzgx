from fzgx.project import Project, SYMBOL_RE, ASM_LINE_RE


def test_symbol_regex():
    m = SYMBOL_RE.match("fn_18_3FC = .text:0x000003FC; // type:function size:0x10 scope:global align:4")
    assert m and m.group("name") == "fn_18_3FC" and m.group("section") == ".text"
    m = SYMBOL_RE.match("lbl_1_data_3D580 = .data:0x0003D580; // type:object size:0x30")
    assert m and int(m.group("addr"), 16) == 0x3D580


def test_asm_line_regex():
    m = ASM_LINE_RE.match("/* 000003FC 000004B8  7C 05 21 D6 */\tmullw r0, r5, r4")
    assert m and m.group("insn") == "mullw r0, r5, r4" and m.group("addr") == "000003FC"


def test_refs_extraction():
    syms = {"lbl_18_section4_0": None, "fn_18_3FC": None, "memset": None}
    lines = [
        "0014: lis r6, lbl_18_section4_0@ha",
        "0024: addi r26, r6, lbl_18_section4_0@l",
        "0034: bl fn_18_3FC",
        "0088: bl memset",
        "0090: xori r0, r0, 0x2d2e",
    ]
    assert Project._refs(lines, syms) == ["lbl_18_section4_0", "fn_18_3FC", "memset"]


def test_key_and_resolve_for_ambiguous_names(tmp_path, monkeypatch):
    from fzgx.project import Symbol
    p = Project.__new__(Project)
    p._modules = ["main", "a", "b"]
    p._symbols = {
        "main": {"OSInit": Symbol("OSInit", "main", ".text", 0x80003100, "function", 4, "global")},
        "a": {"_prolog": Symbol("_prolog", "a", ".text", 0, "function", 4, "global")},
        "b": {"_prolog": Symbol("_prolog", "b", ".text", 0, "function", 4, "global")},
    }
    assert p.resolve("OSInit").module == "main"
    assert p.resolve("_prolog") is None
    assert p.resolve("b:_prolog").module == "b"
    assert p.key(p.resolve("b:_prolog")) == "b:_prolog"
    assert p.key(p.resolve("OSInit")) == "OSInit"
