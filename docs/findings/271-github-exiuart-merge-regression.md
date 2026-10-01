# GitHub EXI split merge regression: verified blocker

Observed public main: 9af7e15af23b0dcfbce890532f984f5eb555edfd (verified by git ls-remote github refs/heads/main).
Workshop HEAD: a05c4235d9883f072f69709dc09fc9104645e16d.

## Evidence

GitHub Actions run https://github.com/karamzov123/fzero-gx-decomp/actions/runs/34164914021 failed on the public main above. `gh run view 34164914021 --repo karamzov123/fzero-gx-decomp --log-failed` shows MWCC aborting EXIBios.c line 1092: `stw r30, lbl_801A6840`, illegal use of label. Ninja exits 2. EXIUart.o compiles separately in that same build.

Reading both committed source files through git show confirms four duplicate external definitions between src/dolphin/os/EXIBios.c and src/dolphin/os/EXIUart.c: __EXIGetID, __OSEnableBarnacle, InitializeUART, WriteUARTN. Compared with workshop HEAD, public EXIBios.c has an extra 447-line UART tail. Public configure.py and config/GFZE01/splits.txt agree with workshop HEAD, which already splits the objects. Therefore the old tail is inconsistent with the split configuration. The observed failure is a compiler error; a duplicate-symbol linker failure has NOT been separately exercised.

The preceding contributor merge 675f69f289b338df4a7098fedcf89a73de23d113 passed Actions run 34164369074. Contributor commit 6c290184a47ce856cbfa3331a4a3d819176bf238 changes fn_80085814 from asm to C and changes the old InitializeUART definition signature in EXIBios.c. The later split/merge leaves that old definition alongside the new EXIUart owner.

## Import and fleet boundaries

`python3 tools/github_import.py --dry-run` identifies one incoming contributor commit (6c290184), across five commits since marker 7659be63. Importer skips merge commits; its dry run does not prove split-aware applicability or equivalence to the merged public tree. Do not advance the marker solely from this dry run.

Live workshop has a tracked GXGeometry.c modification and active integrator/worker processes. Durable submission queue was read using SQLite mode=ro: batches table empty. Ten active unit leases belong to existing workers, none to this session. No sources, leases, marker, queue entries, or active build outputs were changed. No candidate compilation or new conversion is claimed.

## Required repair acceptance criteria

Repair from the current GitHub main, not by regenerating an older workshop export. Preserve Khoi Tran's console-write C and contributor credit. Remove stale UART ownership from EXIBios while retaining EXIUart and its schedule-specific flags. Reconcile the console caller's InitializeUART declaration with its new owner without reintroducing old assembly. Rebuild all_source and main.dol, verify retail SHA-1 421c88106697d3275a3fc26fb7a01bf6d816b271 and exact per-function/relocation parity, and verify hosted CI before marking the public baseline repaired. Coordinate import into the workshop with the active integrator; do not reset or stash another worker's source.

Open contribution PRs at observation: #8 TRK checked file I/O, #9 MSL strncmp/strncat, #10 PAD SPEC0/SPEC1. These were enumerated, not reviewed or tested in this session.
