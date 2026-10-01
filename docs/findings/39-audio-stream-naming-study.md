# Naming study — audio/stream region (session sa-2)

Scope: fn_80026D70 (axmix, fan-in ~18), fn_8004E270 (adxt, fan-in ~11),
fn_800589BC (adxt/SVM, fan-in ~15), plus a sweep of adxt/criadx units for
string-backed unnamed fns not yet renamed. Repo inspected read-only;
no files modified in-repo.

---

## Target functions

### fn_80026D70 — axmix per-device control-block clear
- **Suggested name:** `axmix_device_ctrl_clear` (family: `AXMix_*` device-control accessors)
- **Evidence:** `src/game/axmix_80025504.c` (asm @1829). Trivial accessor:
  `idx = r3->0x18`; entry = `lbl_80176160 + idx*0x60`; store 0 to entry+0.
  It sits in a dense accessor family over the same 0x60-byte-stride table
  (`lbl_80176160[6144]` = 64 entries × 0x60):
  - fn_80026D90 stores arg→+8 and ORs flag 0x1000 into +4
  - fn_80026DB8 reads +8; fn_80026DD4 clears bit31/oris 0x4000; fn_80026E04 oris 0x4000|1
  - fn_80026E2C stores arg→+0xC, OR 0x4000; fn_80026E84 OR 0x4000|2; fn_80026EAC stores +0x10 when mode==3
  - fn_80025EF4 is the initializer (fills entry from args, clamps volume −904..60
    through a table at +0x710)
- **Callers:** adxt_8005A24C (×4), adxt_8005BC20 (×2), axmix_80026EE0,
  gamehead_8005C120 (sound alloc path), MTXHead.o profile (shared asm tail).
  All callers pass a device/handle whose +0x18 is an index < 64.
- **Confidence:** medium on exact role (clears slot field +0 of the mixer
  device table), high that it is an axmix device-table accessor.

### fn_8004E270 — ADXT handle state-code getter
- **Suggested name:** `adxt_hnd_get_state` (or CRI-style `ADXT_GetCmdState_inline`)
- **Evidence:** `src/game/adxt_8004E098.c` @240: `lwz r3, 0xc(r3); blr`.
  Field +0xC of the ADXT handle is the state/command code driven by the whole
  state machine in the same unit:
  - fn_8004E098 advances 1→2→3 (exec decode, calls fn_8004F55C/fn_8004EFA8)
  - fn_8004E198 resets 3→0; fn_8004E1E4 sets 0→1; fn_8004E204 sets 0→2 (start);
    fn_8004E238 sets 0→pause-pending (1)
- **Callers:** all in criadx container parsers (criadx_800424B8 ×3,
  criadx_800433A4 ×3, criadx_800443AC ×3, criadx_800452FC, criadx_80041460):
  pattern is `if (h->4 == 1 && adxt_hnd_get_state(h->8) == 0) { run decoder
  callback }` — i.e. gate streaming-decode execution on handle state==0 (idle).
- **Confidence:** high on semantics (returns state word of ADXT handle);
  medium on the best public-API-style name (this is an inline accessor, not an
  asserted export).

### fn_800589BC — SVM ring-buffer consume/read primitive
- **Suggested name:** `svm_ringbuf_read`
- **Evidence:** `src/game/adxt_800589BC.c` @30 (whole unit is CRI SVM/GC Ver.1.51,
  cf. findings/23 blob map @0x80092371). Logic:
  ```
  w = r3->0; avail = r3->4;      // ring buffer ctrl: base, size
  *r5 = base; *r6 = avail;       // out: ptr, old fill level
  if (avail > r4) avail = r4;    // clamp request
  r3->4 = avail;
  r6->4 = avail; if (avail==0) { r6->0 = 0; return; }
  r6->0 = base + avail;          // advance read cursor
  ```
  i.e. consumes up to `r4` bytes from the SVM circular buffer, returning
  [data ptr, consumed count] via r5/r6.
- **Callers (fan-in ~15):** adxt_8004A560, adxt_8004B7F4 (×2), adxt_8004CD70 (×2),
  adxt_80050180, adxt_80053EA0, adxt_8005A24C (×3, feeding function-pointer
  callbacks with the returned span), criadx_80041460 (×4), criadx_80041BF8 —
  every caller immediately hands (ptr,len) to a stream/sound-device callback.
- **Confidence:** high (structure is unambiguous); medium on naming convention
  ("svm" prefix chosen because the unit's own asserts say `_SVM_DelCbSvr`,
  `_SVM_SetCbSvr`).

Note: findings/27 lists fan-in as 18/11/15 respectively (counted across all
linked objs incl. ones absent from this worktree's obj set); the parent's
10/10/9 numbers presumably come from the reduced obj set. Same functions either way.

---

## Assert/string-backed unnamed fns in adxt units not yet renamed

Only two symbols in these units carry real names today (`ADXT_GetDecNumSmpl`
@8004BA40, `ADXT_Create` @8004CD70); everything below is still `fn_*` despite
naming itself in its assert/error string. High-confidence renames:

### adxt_8004B7F4.c (ADXT parameter-getter/setter API)
| VA | suggested name | string |
|---|---|---|
| 8004BF0C | ADXT_SetOutVol | E02080823 |
| 8004BF68 | ADXT_GetOutPan | E02080826 |
| 8004BFAC | ADXT_SetOutPan | E02080825 (+E8101208) |
| 8004C05C | ADXT_GetNumChan | E02080820 |
| 8004C0B4 | ADXT_GetSfreq | E02080819 |
| 8004C10C | ADXT_GetNumSmpl | E02080817 |

### adxt_8004C164.c
| VA | name | string |
|---|---|---|
| 8004C658 | ADXT_GetStat | E02080814 |
| 8004C698 | ADXT_Stop | E02080813 |
| 8004C794 | ADXT_StartSj | E02080812 |

### adxt_8004CD70.c
| VA | name | string |
|---|---|---|
| 8004D18C | ADXT_StartFname | E02080807 |
| 8004D220 | ADXT_ExecHndl | E02080842 |
| 8004D528 | ADXT_StatDecInfo (channel-count guard) | E9081001 "stat_decinfo can't play this number of channel" |
| 8004DBBC | adxt_trap_entry (not-enough-data handler) | E8101201 |

### adxt_8004F00C.c (ADXF)
| VA | name | string |
|---|---|---|
| 8004FC94 | ADXF_Stop | E9040822/E9040823 "'pxf->stm' is NULL.(ADXF_Stop)" — corroborates findings/23 @0x80050698 |
| 8004F818 | ADXF_SetPtdId_family | E9040828 "ptid/flid is range outside" |

### adxt_8004A560.c
| VA | name | string |
|---|---|---|
| 8004A5F4 | ADXSTMF_StatExec_family | E02110501 "adxstmf_stat_exec can't open" |

### adxt_80051448.c (AHX attach, corroborates findings/23 AHX block)
| VA | name | string |
|---|---|---|
| 80051E64 | ADXT_AttachAHX | E1052501 + "can not attach AHX." |
| 80051DC4 / 80051CC0 | ahx_callback_helpers | installed as callbacks right after successful attach |

### adxt_80053EA0.c (CVFS wrapper layer — each wraps a vtbl call with paired asserts)
| VA | name | string pair |
|---|---|---|
| 800546A0 | cvFsGetStat | cvFsGetStat_1/_2 |
| 800547C8 | cvFsStopTr | cvFsStopTr_1/_2 |
| 80054870 | cvFsReqRd | cvFsReqRd_1/_2 |
| 80054930 | cvFsSeek | cvFsSeek_1/_2 |
| 800549F0 | cvFsTell | cvFsTell_1/_2 |
| 80054AB0 | cvFsClose | cvFsClose_1/_2 |
| 800551D0 | cvFsSetDefDev | cvFsSetDefDev_1/_2 |

### adxt_800589BC.c (SVM unit)
| VA | name | string |
|---|---|---|
| 800592E0 | SVM_DelCbSvr | "_SVM_DelCbSvr illegal id" |
| 80059428 | SVM_SetCbSvr | "_SVM_SetCbSvr too many server function" — matches findings/23 @0x80059DD8 estimate region |
| 800595A4 | svm_error_log_helper | formats into lbl_8018FEE0 + user errcb (lbl_8018FF60) |
| 80059CA4 / 80059D04 / 80059D68 | svm_status_accessors (get u32 / get s8 / read-entry) | E0040301/E0092912 "handl is null" |

### criadx units (container parsers, lower priority)
- criadx_800424B8: RIFF/WAVE parser family
- criadx_800433A4: AIFF/FORM parser
- criadx_800443AC: "snd " parser
- criadx_800452FC: SPSD parser
- criadx_800462F8: "(c)CRI" header probe

## Cross-reference with live-verified names
~/projects/fzero-gx-online/findings/ contains no live-verified audio symbol
names beyond confirming the adxt/axmix/criadx unit split (findings/69/71);
the online sessions never traced audio. So no contradictions, but also no
live confirmation — the renames above rest on the middleware's own assert
strings (strong) and structural analysis (medium).
