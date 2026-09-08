"""Project model shared by every fzgx subcommand.

Reads decomp-toolkit's config (config.yml, symbols.txt, splits.txt), the
generated build/<VERSION>/config.json and objdiff.json, and the disassembly
dtk writes under build/<VERSION>/**/asm/*.s. Nothing here talks to an LLM.

Address conventions follow dtk: DOL symbols use absolute addresses
(0x80xxxxxx); REL symbols use section-relative offsets.
"""

from __future__ import annotations

import json
import os
import re
import time
from dataclasses import dataclass, field
from pathlib import Path
from typing import Dict, Iterable, List, Optional

ROOT = Path(__file__).resolve().parent.parent.parent
DEFAULT_VERSION = "GFZE01"
# Ledger, locks, saved attempts and caches: outside build/ on purpose (a build wipe must not lose state).
STATE_DIR = ROOT / ".fzgx"

# "name = .section:0xADDR; // type:function size:0x10 scope:global align:4 data:4byte"
SYMBOL_RE = re.compile(
    r"^(?P<name>[^\s=]+)\s*=\s*(?P<section>\.[\w.$]+):0x(?P<addr>[0-9A-Fa-f]+);"
    r"\s*//\s*(?P<attrs>.*)$"
)
FN_START_RE = re.compile(r"^\.fn\s+(?P<name>\S+?),\s*(?P<scope>\w+)")
FN_END_RE = re.compile(r"^\.endfn\s+(?P<name>\S+)")
# "/* 000003FC 000004B8  7C 05 21 D6 */\tmullw r0, r5, r4"
ASM_LINE_RE = re.compile(
    r"^/\*\s*(?P<addr>[0-9A-Fa-f]+)\s+(?P<off>[0-9A-Fa-f]+)\s+(?P<bytes>(?:[0-9A-Fa-f]{2}\s?){4})\*/\s*(?P<insn>.*)$"
)
PPC_REGS = {f"r{i}" for i in range(32)} | {f"f{i}" for i in range(32)} | {
    f"cr{i}" for i in range(8)} | {"lr", "ctr", "xer", "sp", "rtoc", "qr0"} | {
    f"qr{i}" for i in range(8)}
OPERAND_SYMBOL_RE = re.compile(r"(?<![\w.$@])([A-Za-z_.$][\w.$]*)(?:@(ha|l|sda21|sda2|h|lo|hi))?")


@dataclass
class Symbol:
    name: str
    module: str  # "main" for the DOL, else the REL module name
    section: str
    addr: int
    kind: str  # function | object | label | ...
    size: int
    scope: str
    attrs: Dict[str, str] = field(default_factory=dict)

    @property
    def end(self) -> int:
        return self.addr + self.size


@dataclass
class Split:
    unit: str  # e.g. "rel/sample/keygen.c"
    section: str
    start: int
    end: int
    align: Optional[int]


@dataclass
class Function:
    symbol: Symbol
    asm: List[str]  # instructions without the byte comment prefix
    refs: List[str]  # referenced symbol names (deduplicated, in order)
    unit: Optional[str]  # dtk unit name the function currently belongs to


class Project:
    def __init__(self, version: str = DEFAULT_VERSION) -> None:
        self.version = version
        self.config_dir = ROOT / "config" / version
        self.build_dir = ROOT / "build" / version
        self.units_path = self.config_dir / "units.json"
        self._modules: Optional[List[str]] = None
        self._symbols: Dict[str, Dict[str, Symbol]] = {}
        self._asm_index: Dict[str, Dict[str, Function]] = {}

    # ------------------------------------------------------------------ modules
    @property
    def modules(self) -> List[str]:
        """Module names: 'main' (DOL) followed by every REL module in config.yml."""
        if self._modules is None:
            names = ["main"]
            text = (self.config_dir / "config.yml").read_text()
            names += re.findall(r"^\s*name:\s*(\S+)\s*$", text, re.M)
            self._modules = names
        return self._modules

    def module_config_dir(self, module: str) -> Path:
        return self.config_dir if module == "main" else self.config_dir / module

    def module_build_dir(self, module: str) -> Path:
        return self.build_dir if module == "main" else self.build_dir / module

    def module_src_prefix(self, module: str) -> str:
        return "dol" if module == "main" else f"rel/{module}"

    # ------------------------------------------------------------------ symbols
    def symbols(self, module: str) -> Dict[str, Symbol]:
        if module not in self._symbols:
            out: Dict[str, Symbol] = {}
            path = self.module_config_dir(module) / "symbols.txt"
            for line in path.read_text().splitlines():
                m = SYMBOL_RE.match(line.strip())
                if not m:
                    continue
                attrs = dict(
                    kv.split(":", 1) if ":" in kv else (kv, "")
                    for kv in m.group("attrs").split()
                )
                size = int(attrs.get("size", "0x0"), 16)
                out[m.group("name")] = Symbol(
                    name=m.group("name"), module=module, section=m.group("section"),
                    addr=int(m.group("addr"), 16), kind=attrs.get("type", ""),
                    size=size, scope=attrs.get("scope", "global"), attrs=attrs,
                )
            self._symbols[module] = out
        return self._symbols[module]

    def find_symbol(self, name: str, module: Optional[str] = None) -> Optional[Symbol]:
        for mod in ([module] if module else self.modules):
            s = self.symbols(mod).get(name)
            if s:
                return s
        return None

    def functions(self, module: str) -> List[Symbol]:
        return sorted(
            (s for s in self.symbols(module).values() if s.kind == "function"),
            key=lambda s: s.addr,
        )

    # ------------------------------------------------------------------- splits
    def splits(self, module: str) -> List[Split]:
        path = self.module_config_dir(module) / "splits.txt"
        out: List[Split] = []
        unit: Optional[str] = None
        for raw in path.read_text().splitlines():
            if not raw.strip():
                continue
            if not raw[0].isspace():
                unit = raw.rstrip(":").split(":")[0].strip() if raw.strip() != "Sections:" else None
                continue
            if unit is None:
                continue
            parts = raw.split()
            attrs = dict(p.split(":", 1) for p in parts[1:] if ":" in p)
            if "start" not in attrs:
                continue
            out.append(Split(
                unit=unit, section=parts[0], start=int(attrs["start"], 16),
                end=int(attrs["end"], 16),
                align=int(attrs["align"]) if "align" in attrs else None,
            ))
        return out

    def unit_of(self, sym: Symbol) -> Optional[str]:
        """Configured (non-auto) unit that owns this symbol's address, if any."""
        for sp in self.splits(sym.module):
            if sp.section == sym.section and sp.start <= sym.addr < sp.end:
                return sp.unit
        return None

    # ---------------------------------------------------------------- asm index
    def _asm_files(self, module: str) -> List[Path]:
        d = self.module_build_dir(module) / "asm"
        return sorted(d.rglob("*.s")) if d.exists() else []

    def function_asm(self, module: str) -> Dict[str, Function]:
        """Parse every .s of a module once per process; cached on disk by mtime."""
        if module in self._asm_index:
            return self._asm_index[module]
        files = self._asm_files(module)
        cache = STATE_DIR / f"asm_index_{self.version}_{module}.json"
        stamp = max((f.stat().st_mtime for f in files), default=0)
        if cache.exists():
            data = json.loads(cache.read_text())
            if data.get("stamp") == stamp:
                self._asm_index[module] = {
                    k: Function(self.symbols(module)[k], v["asm"], v["refs"], v["unit"])
                    for k, v in data["functions"].items() if k in self.symbols(module)
                }
                return self._asm_index[module]
        syms = self.symbols(module)
        result: Dict[str, Function] = {}
        for f in files:
            unit = str(f.relative_to(self.module_build_dir(module) / "asm").with_suffix(""))
            cur: Optional[str] = None
            lines: List[str] = []
            for raw in f.read_text(errors="replace").splitlines():
                m = FN_START_RE.match(raw)
                if m:
                    cur, lines = m.group("name"), []
                    continue
                if cur and FN_END_RE.match(raw):
                    if cur in syms:
                        result[cur] = Function(syms[cur], lines, self._refs(lines, syms), unit)
                    cur = None
                    continue
                if cur is None:
                    continue
                m = ASM_LINE_RE.match(raw)
                if m:
                    lines.append(f"{m.group('addr')}: {m.group('insn').strip()}")
                elif raw.strip().endswith(":"):
                    lines.append(raw.strip())  # local label
        cache.parent.mkdir(parents=True, exist_ok=True)
        cache.write_text(json.dumps({
            "stamp": stamp,
            "functions": {k: {"asm": v.asm, "refs": v.refs, "unit": v.unit} for k, v in result.items()},
        }))
        self._asm_index[module] = result
        return result

    @staticmethod
    def _refs(lines: List[str], syms: Dict[str, Symbol]) -> List[str]:
        seen: List[str] = []
        for line in lines:
            body = line.split(": ", 1)[1] if ": " in line else line
            if " " not in body:
                continue
            operands = body.split(" ", 1)[1]
            for m in OPERAND_SYMBOL_RE.finditer(operands):
                name = m.group(1)
                if name in PPC_REGS or name.startswith("0x") or name[0].isdigit():
                    continue
                if name in syms and name not in seen:
                    seen.append(name)
        return seen

    def function(self, name: str) -> Optional[Function]:
        sym = self.find_symbol(name)
        if not sym or sym.kind != "function":
            return None
        return self.function_asm(sym.module).get(name)

    def callers(self, name: str, limit: int = 8) -> List[str]:
        sym = self.find_symbol(name)
        if not sym:
            return []
        out = [f.symbol.name for f in self.function_asm(sym.module).values() if name in f.refs]
        return out[:limit]

    # -------------------------------------------------------------------- units
    def load_units(self) -> List[dict]:
        if not self.units_path.exists():
            return []
        return json.loads(self.units_path.read_text())

    def save_units(self, units: List[dict]) -> None:
        units.sort(key=lambda u: (u["module"], u["source"]))
        self.units_path.write_text(json.dumps(units, indent=2) + "\n")

    def objdiff_unit_name(self, module: str, source: str) -> str:
        return f"{module}/{source.rsplit('.', 1)[0]}"

    def objdiff_units(self) -> Dict[str, dict]:
        path = ROOT / "objdiff.json"
        if not path.exists():
            return {}
        return {u["name"]: u for u in json.loads(path.read_text())["units"]}

    # ------------------------------------------------------------------- misc
    @staticmethod
    def now() -> int:
        return int(time.time())

    @staticmethod
    def rel(path: Path) -> str:
        return os.path.relpath(path, ROOT)
