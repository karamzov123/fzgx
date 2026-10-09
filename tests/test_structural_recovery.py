"""Regression tests for non-destructive diagnostics and empty seed scores."""
import ast
import json
from collections import defaultdict
import os
from pathlib import Path
import subprocess
import sys
import tempfile
from types import SimpleNamespace
import unittest
from unittest.mock import patch

PROJECT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(PROJECT / 'tools'))
from fzgx import oracle, tufile
from seeds import recovered


class StructuralRecoveryTests(unittest.TestCase):
    def setUp(self):
        scratch = Path(os.environ.get('TMPDIR', str(Path.home() / '.hermes/cache/scratch')))
        scratch.mkdir(parents=True, exist_ok=True)
        self.tmp = tempfile.TemporaryDirectory(dir=scratch)
        self.root = Path(self.tmp.name)

    def tearDown(self):
        self.tmp.cleanup()

    def test_diagnosis_preserves_pool_tu_and_reports_real_link_error(self):
        root = self.root
        tu = root / 'src/sound.c'
        tu.parent.mkdir()
        tu.write_text('accepted pool reconstruction\n')
        gen = root / 'gen.c'
        gen.write_text('generated code\n')
        rec = dict(source='sound/fn.c', tu='sound.c', status='nonmatching', symbols=['fn'])

        class Project:
            def resolve(self, symbol): return SimpleNamespace(module='main_rel')
            def key(self, symbol): return 'fn'
            def unit_of(self, symbol): return 'sound/fn.c'
            def unit_record(self, source): return rec.copy()
            def load_units(self): return [rec.copy()]
            def save_units(self, rows): rec.update(rows[0])

        cp = subprocess.CompletedProcess([], 1,
            "### Linker Warning:\n# warning noise\n### Linker Error:\n# multiply-defined 'lbl_bss'\n", '')
        with patch.object(oracle, 'ROOT', root), \
             patch.object(oracle, 'unit_source_path', return_value=gen), \
             patch.object(oracle, 'configure'), \
             patch.object(oracle, 'byte_diff', return_value={}), \
             patch.object(oracle, 'relink', return_value=cp), \
             patch.object(tufile, 'remove', side_effect=lambda *args: tu.write_text('')):
            out = oracle.why_link(Project(), 'fn')
        self.assertEqual(tu.read_text(), 'accepted pool reconstruction\n')
        self.assertEqual(rec['status'], 'nonmatching')
        self.assertIn("# multiply-defined 'lbl_bss'", out['diag']['errors'])
        self.assertFalse(any('warning' in line.lower() for line in out['diag']['errors']))

        with patch.object(oracle, 'ROOT', root), \
             patch.object(oracle, 'unit_source_path', return_value=gen), \
             patch.object(oracle, 'configure'), \
             patch.object(oracle, 'relink', side_effect=[RuntimeError('probe failed'), cp]), \
             patch.object(tufile, 'remove', side_effect=lambda *args: tu.write_text('')):
            with self.assertRaisesRegex(RuntimeError, 'probe failed'):
                oracle.why_link(Project(), 'fn')
        self.assertEqual(tu.read_text(), 'accepted pool reconstruction\n')
        self.assertEqual(rec['status'], 'nonmatching')

    def test_null_score_is_not_a_recovered_seed(self):
        obj = recovered.SavedCandidates.__new__(recovered.SavedCandidates)
        obj.rows = {'fn': {'best_percent': None}}
        obj.threshold = 95
        obj.ledger = SimpleNamespace(db=SimpleNamespace(execute=lambda *args: []))
        obj.evidence = defaultdict(list)
        obj.candidates = defaultdict(list)
        obj.unreadable = []
        obj.inputs = defaultdict(dict)
        with patch.object(recovered, 'ROOT', self.root), \
             patch.object(recovered, 'STATE_DIR', self.root / 'state'):
            obj._collect()
        self.assertEqual(dict(obj.evidence), {})
        self.assertEqual(dict(obj.candidates), {})
    def test_prepare_accepts_historical_evidence_with_null_ledger_score(self):
        candidate = dict(percent=96, settings_recorded=True, origin='archive',
                         sha256='fixture', body='void fn(void) {}', mw='GC/1.2.5n', flags='')
        saved = SimpleNamespace(rows={'fn': {'best_percent': None, 'size': 4}},
                    evidence={'fn': ['archive']}, candidates={'fn': [candidate]}, missing=[], unreadable=[],
                    collect=lambda: None, project=SimpleNamespace(resolve=lambda s: SimpleNamespace(size=4, module='main')))
        checked = SimpleNamespace(ok=True, percent=96, percent_adjusted=96, mw_version='GC/1.2.5n', extra_cflags='', error=None)
        with patch.object(recovered, 'SavedCandidates', return_value=saved), \
             patch.object(recovered.oracle, 'check_many', return_value={'fn': checked}), \
             patch.object(recovered, 'refine'):
            recovered.prepare(self.root / 'prepared', 95)
        selection = json.loads((self.root / 'prepared/selection.json').read_text())
        self.assertEqual(selection['ledger_above_threshold'], 0)
        self.assertEqual(selection['additional_to_ledger'], ['fn'])

    def test_recarve_cleanup_runs_and_preserves_all_failures(self):
        from fzgx import api, ledger, uncarve
        attempts = self.root / 'attempts'
        attempts.mkdir()
        (attempts / 'fn.linkfail.fixture.c').write_text('preserved C')
        source = self.root / 'source.c'
        source.write_text('original C')
        class Project:
            unit = None
            def resolve(self, symbol): return SimpleNamespace(module='main')
            def key(self, symbol): return 'fn'
            def unit_of(self, symbol): return self.unit
            def unit_record(self, unit): return dict(source=unit, status='nonmatching')
            def load_units(self): return [dict(source=self.unit, status='nonmatching')]
            def save_units(self, units): pass
        project = Project()
        fake_ledger = SimpleNamespace(db=SimpleNamespace(execute=lambda *args: None))
        with patch.object(oracle, 'STATE_DIR', self.root), \
             patch.object(api, 'carve', side_effect=lambda *args: setattr(project, 'unit', 'fn.c')), \
             patch.object(ledger, 'Ledger', return_value=fake_ledger), \
             patch.object(oracle, 'unit_source_path', return_value=source), \
             patch.object(oracle, 'configure'), \
             patch.object(oracle, 'relink', side_effect=[RuntimeError('diagnostic failed'), RuntimeError('restore failed')]), \
             patch.object(uncarve, 'uncarve', side_effect=RuntimeError('uncarve failed')) as cleanup:
            try:
                oracle.why_link(project, 'fn')
            except BaseException as exc:
                errors = []
                def flatten(error):
                    if isinstance(error, BaseExceptionGroup):
                        for child in error.exceptions: flatten(child)
                    else: errors.append(str(error))
                flatten(exc)
            else:
                self.fail('injected failures should propagate')
            cleanup.assert_called_once_with(project, ['fn.c'])
        self.assertCountEqual(errors, ['diagnostic failed', 'restore failed', 'uncarve failed'])
        self.assertEqual(source.read_text(), 'original C')


if __name__ == '__main__':
    # Exercise the same tests against the tracked pre-fix methods without
    # changing files or touching the live project state.
    if '--original' in sys.argv:
        sys.argv.remove('--original')
        for file, namespace, cls, method in [
            ('tools/fzgx/oracle.py', oracle.__dict__, None, 'why_link'),
            ('tools/seeds/recovered.py', recovered.__dict__, recovered.SavedCandidates, '_collect'),
            ('tools/seeds/recovered.py', recovered.__dict__, None, 'prepare')]:
            text = subprocess.check_output(['git', 'show', 'HEAD:' + file], cwd=PROJECT, text=True)
            tree = ast.parse(text)
            nodes = tree.body if cls is None else next(n.body for n in tree.body if isinstance(n, ast.ClassDef) and n.name == cls.__name__)
            node = next(n for n in nodes if isinstance(n, ast.FunctionDef) and n.name == method)
            exec(compile(ast.Module(body=[node], type_ignores=[]), '<pre-fix>', 'exec'), namespace)
            if cls is not None:
                setattr(cls, method, namespace[method])
    unittest.main()
