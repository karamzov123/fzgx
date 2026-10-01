# Finding 38 — Session 23 naming studies (7 parallel subagents) + applied waves 7–10

Session 23 (2026-08-23 evening), baseline 503e7c4, gate green at every commit.

## Audit result (findings/36 + tool commits dcbb25d/fba7f7a)
Repo-wide reloc-parity audit: **2225/2225 functions linked-exact, zero flags.**
Tool now covers SDA21 on all d-form load/store ops, REL14 branches, ADDR16_HI,
and duplicate-symbol disambiguation by size. objdiff sub-100% fuzzy is scoring
noise everywhere; there is NO hidden byte-mismatch backlog.

## Naming waves applied (all byte-neutral, fresh-build sha1 green each time)
- **Wave 7 (57986dc), 11 HIGH names**: OSPanic (string+backchain-dump evidence),
  OSGetStackPointer, DVDOpen ("Warning: DVDOpen(): file '%s' was not found
  under %s."), DVDConvertPathToEntrynum (FST parse shape), __CARDAccess
  (melee CARDOpen.c:25 semantic match — new identification),
  sqrtf (frsqrte+2NR), atan2f (table-poly + fp-classify prologue),
  strncmp/strncpy/strcpy/sprintf (MSL canonical shapes).
- **Wave 8 (a5715be), 20 self-named CRI fns**: ADXT_SetOutVol/GetOutPan/
  SetOutPan/GetNumChan/GetSfreq/GetNumSmpl/GetStat/Stop/StartFname/ExecHndl/
  StatDecInfo/AttachAHX, ADXF_Stop-family site, cvFsGetStat/StopTr/ReqRd/Seek/
  Tell/Close/SetDefDev, SVM_DelCbSvr — names taken verbatim from the
  middleware's own E-code assert strings (primary evidence).
- **Wave 9 (a65a925), 12 MED structural names**: criErr_CallErrCallback
  (fan-in 25 error sink), svmErrPrintf/gcciErrPrintf, svmLockServer/
  svmUnlockServer, svmEnterCritical/svmExitCritical, svm_ringbuf_read,
  ADXT_GetCmdState, TRKDoWrite, OSCreateHeap_wrapper_A/B (online findings/11).
- **Wave 10 (d0c3cf9)**: g_currentHeapHandle (online findings/12 correction:
  live current-heap var, not a constant), getCupModeConst accessor.

Total: **45 new semantic names** this session. objdiff unchanged at
1258/2235 exact / 215920B (renames are byte-neutral by construction).

## Deferred (insufficient evidence per forward-risk rule #4)
- fn_8008A754 getter / fn_8008A748 setter pair @0x801A50B0 (USB/EXI flag?)
- fn_80012678 OSSetWirelessID candidate (MED-HIGH but name collision risk with
  existing SI symbols — resolve before applying)
- fn_80017228 DVDCancel wrapper (needs fn_80019B78=DVDCancelAsync named first)
- ModelSetCachedParam_F0/1F0/2F0/430/570 cluster (descriptive; needs model
  struct layout before final naming)
- fn_80046718/46738 thunks, svm pub-thunks (structural only)

## Study reports (full detail)
/tmp/naming-{card,math,game1,msl,os,audio,online}.md (subagent output;
key rows transcribed above). Remaining fan-in targets: findings/37.
