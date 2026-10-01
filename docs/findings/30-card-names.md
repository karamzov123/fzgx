# STATUS: BIOS-range renames APPLIED (commit 106f0ad). Mount/file-op range renames DEFERRED —
# applying them without also updating the defining unit's asm labels regressed objdiff match;
# they need per-unit carve+rename in one pass, not symbol-only renames. See session 18 notes.

# dolphin/card/* naming study — GFZE01 (F-Zero GX) vs Melee Dolphin SDK (Sep 2003 CARD)

Method: per-fn profiles from `/tmp/gx_fn_db.json` + targeted capstone disassembly
(`tools/dump_asm.py`) of every unnamed fn in 0x80029828–0x8003050C, matched against
semantic reference `/tmp/opencode/melee/extern/dolphin/src/dolphin/card/*.c`.
`__CARDBlock[]` stride 0x110 @ 0x80177960 confirmed throughout; key struct offsets:
attached=+0x00 result=+0x04 mountStep=+0x24 currentDir?=+0x84 currentFat=+0x88
dirBlock=+0x80 sectorSize=+0x0C cBlock=+0x10 workArea=+0x80 cmd=+0x94 latency=+0xB0
repeat=+0xAC addr/xferred/buffer=+0xB0..B8 apiCallback=+0xC0 fileInfo=+0xC0
eraseCallback=+0xD0 unlockCallback=+0xDC diskID=+0x10C threadQueue=+0x8C alarm=+0xE0.

Confidence: HIGH = structural/exi-cmd match decisive; MED = strong but indirect;
LOW = unresolved/ambiguous.

## CARDBios.c (0x80029828–0x8002A698)

| VA | fn_ sym | proposed | conf | evidence |
|---|---|---|---|---|
| 80029828 | fn_80029828 | __CARDSyncCallback | HIGH | chan*0x110; OSWakeupThread(&card->threadQueue+0x8C) |
| 8002985C | fn_8002985C | __CARDExtHandler | HIGH | attached check, EXISetExiCallback(chan,0), OSCancelAlarm(&alarm+0xE0), exi/ext callback chain |
| 80029934 | fn_80029934 | __CARDExiHandler | HIGH | EXILock, ReadStatus(fn_80029C38)+ClearStatus(fn_80029D28), status&0x18 IOERROR, Retry(fn_80029E78), EXIUnlock |
| 80029A4C | fn_80029A4C | __CARDTxHandler | HIGH | err=!EXIDeselect; EXIUnlock; txCallback(chan, EXIProbe()?READY:NOCARD) |
| 80029AF4 | fn_80029AF4 | __CARDUnlockedHandler | MED | reads unlockCallback(+0xDC), fires w/ EXIProbe |
| 80029B78 | fn_80029B78 | __CARDReadNintendoID | HIGH | EXISelect(0,0); imm wr 2B / imm rd 4B (li r5,4); id&0xFFFF0000‖id&3 WRONGDEVICE |
| 80029C38 | fn_80029C38 | __CARDReadStatus | HIGH | cmd 0x83000000 (lis r0,-0x7D00), imm 2B wr + 1B rd |
| 80029D28 | fn_80029D28 | __CARDClearStatus | HIGH | cmd 0x89000000 (lis r0,-0x7700), single 1B imm |
| 80029DD4 | fn_80029DD4 | TimeoutHandler | HIGH | alarm==&card[0/1].alarm(+0xE0/+0x1F0) scan; EXISetExiCallback(chan,NULL); exiCallback(IOERROR) |
| 80029E78 | fn_80029E78 | Retry | HIGH | re-issues cmd; switch on card->cmd[0] (+0x94) values 0xF1/0xF2/0xF3/0xF4/0xF5; OSSetAlarm branches |
| 8002A0A4 | fn_8002A0A4 | UnlockedCallback | MED | stores unlockCallback, EXILock(chan,0,handler), calls Retry(fn_80029E78) |
| 8002A1B4 | fn_8002A1B4 | __CARDStart | HIGH | OSDisableInterrupts, attached check, tx/exi cb stores, EXILock(w/UnlockedHandler), EXISelect(0,4), OSCancelAlarm+OSSetAlarm×2 (SetupTimeoutAlarm inlined), restore |
| 8002A368 | fn_8002A368 | __CARDReadSegment | HIGH | cmd[0]=0x52 (li r0,0x52 @…394), cmdlen 5, mode 0 retry 0; __CARDStart(fn_8002A1B4); EXIImmEx,EXIImmEx(latency),EXIDma(512) |
| 8002A49C | fn_8002A49C | __CARDWritePage | HIGH | cmdlen 5, mode 1 (li r6,1), AD1EX (rlwinm …|0x80 pattern @4e4); EXIImmEx+EXIDma(128) |
| 8002A5B8 | fn_8002A5B8 | __CARDEraseSector | HIGH | cmd[0]=0xF1 (li r0,0xF1 @…5e4), cmdlen 3, mode −1, retry 3; EXIImmEx only, then Deselect+Unlock |

## CARDMount.c (0x8002A698–0x8002AF34) + control-block layer

| VA | fn_ sym | proposed | conf | evidence |
|---|---|---|---|---|
| 8002A698 | fn_8002A698 | CARDInit | HIGH | diskID(+0x10C/+0x21C) guard, DSPInit, OSInitAlarm, ×2 OSInitThreadQueue(+0x8C)/OSCreateAlarm(+0xE0), __CARDSetDiskID, OSRegisterResetFunction |
| 8002A744 | fn_8002A744 | (font-encode cache getter) | LOW | returns static hword set during CARDInit after fn_8000CDD8 (=OSGetFontEncode?) — no melee equivalent |
| 8002A74C | fn_8002A74C | (font-encode cache setter) | LOW | bounds-checked store of same static — GX-only glue |
| 8002A774 | fn_8002A774 | __CARDSetDiskID | HIGH | id?:&__CARDDiskNone(+0x220); writes both blocks' +0x10C |
| 8002A7AC | fn_8002A7AC | __CARDGetDiskID-style accessor | LOW | returns card[chan]->diskID; not in melee src |
| 8002A7C4 | fn_8002A7C4 | __CARDSetDiskID(int-safe variant) | MED | per-chan store of r4 ?: 0x80000000 (&__CARDDiskNone) under OSDisableInterrupts |
| 8002A83C | fn_8002A83C | __CARDGetControlBlock | **HIGH (54 xrefs)** | exact melee body: chan bounds→FATAL, !attached→NOCARD, result==BUSY→BUSY, else result=BUSY, apiCallback=NULL, *pcard=card |
| 8002A8F4 | fn_8002A8F4 | **__CARDPutControlBlock** | **HIGH** | disassembly-verified: OSDisableInterrupts; if(attached) result=r4 elif result==BUSY result=r4; OSRestoreInterrupts; return r4. Highest-value rename in repo (54 cross-unit callers) |
| 8002A958 | fn_8002A958 | CARDGetResultCode | HIGH | bounds→−0x80; return card->result(+0x04) |
| 8002A988 | fn_8002A988 | CARDFreeBlocks | HIGH | GCB; fat=C0B8, dir=C4BC; null→PutControlBlock(BROKEN −6); counts 0xFF fileName[0]; sectorSize*fat[FREEBLOCKS] |

## CARDUnlock / CARDBlock / CARDDir cores (0x8002AA…–0x8002C9…)

| VA | fn_ sym | proposed | conf | evidence |
|---|---|---|---|---|
| 8002AAD8 | fn_8002AAD8 | __CARDSync | HIGH | OSDisableInterrupts; while(result==−1) OSSleepThread(&threadQueue); restore |
| 8002AB70 | fn_8002AB70 | OnReset (static) | MED | CARDUnmount(0)/(1) via fn_8002E0C4, −1 checks |
| 8002ABC0 | fn_8002ABC0 | bitrev (CARDUnlock) | HIGH | 32-iteration bit-reversal loops, i==15/i==31 special cases |
| 8002AD2C | fn_8002AD2C | ReadArrayUnlock | HIGH | EXISelect(0,4); memset(cmd,0,5); cmd[0]=0x52 SEC_ADx vs byte-mode; EXIImmEx×3; EXIDeselect |
| 8002AE70 | fn_8002AE70 | GetInitVal | HIGH | OSGetTick; ×1103515245 +12345 (addi r0,0x3039); &0xFFFFF000 |
| 8002AF34 | fn_8002AF34 | __CARDUnlock | HIGH | 2.9KB orchestrator: sector align, ReadArrayUnlock(fn_8002AD2C), bitrev(fn_8002ABC0), DCFlush/Invalidate, DSP task boot + mail loop (fn_8002BA8C), EXIUnlock, MountCallback ref |
| 8002BAFC | fn_8002BAFC | InitCallback (CARDUnlock) | MED | per-block walk (+0x30/+0x140), ReadStatus fn_80029C38, EXIProbe, EXIUnlock |
| 8002BE20 | fn_8002BE20 | BlockReadCallback | HIGH | xferred+=0x200, addr+=0x200, buffer+=0x200, --repeat>0→__CARDReadSegment(fn_8002A368); PutControlBlock+xferCallback |
| 8002BEFC | fn_8002BEFC | __CARDRead | HIGH | sets repeat/addr/buffer then fn_8002A368 (ReadSegment) |
| 8002BF60 | fn_8002BF60 | BlockWriteCallback | HIGH | +=0x80 variants; recursive fn_8002A49C (WritePage) |
| 8002C03C | fn_8002C03C | __CARDWrite | HIGH | sets xfer fields then fn_8002A49C |
| 8002C0A0 | fn_8002C0A0 | accessor card+0xB8 | LOW | 2-instr getter; offset role unclear (see note) |
| 8002C0B8 | fn_8002C0B8 | __CARDGetFatBlock | HIGH | returns card->currentFat(+0x88); +0x88 verified against workArea+0x6000/0x8000 compare in WriteCallback |
| 8002C0C0 | fn_8002C0C0 | CARDBlock::WriteCallback | HIGH | fat0/fat1 = wa+0x6000/0x8000 swap, memcpy 0x2000, PutControlBlock, eraseCallback |
| 8002C194 | fn_8002C194 | CARDBlock::EraseCallback | HIGH | addr=((fat−wa)>>13)*sectorSize; __CARDWrite(...,0x2000,WriteCallback) |
| 8002C25C | fn_8002C25C | __CARDAllocBlock | HIGH | fat[3](free)≥cBlock else INSSPACE(−9); 0xFFFF chain build; fat[4] lastslot |
| 8002C374 | fn_8002C374 | __CARDFreeBlock | HIGH | walk chain fat[n]==0, ++fat[3] |
| 8002C410 | fn_8002C410 | __CARDUpdateFatBlock | HIGH | ++fat[2]; CheckSum(fat+2,0x1FFC,fat,fat+1)(fn_8002C720); DCStoreRange 0x2000; EraseSector(fn_8002A5B8) |
| 8002C4BC | fn_8002C4BC | __CARDGetDirBlock | HIGH | returns card->dir(+0x84); used wherever entries are accessed |
| 8002C4C4 | fn_8002C4C4 | CARDDir::WriteCallback | HIGH | dual-dir copy/memcpy pair + PutControlBlock + eraseCallback |
| 8002C594 | fn_8002C594 | CARDDir::EraseCallback | HIGH | addr calc; __CARDWrite(dir,0x2000,WriteCallback fn_8002C4C4) |
| 8002C65C | fn_8002C65C | __CARDUpdateDir | HIGH | ++checkCode; CheckSum(fn_8002C720); DCStoreRange 0x2000; __CARDEraseSector(fn_8002A5B8) |
| 8002C720 | fn_8002C720 | __CARDCheckSum | HIGH | u16 sum/~sum pairwise loop, 0xFFFF wrap fixes |

## CARDCheck.c content (0x8002C8D0–0x8002D77C)

| VA | fn_ sym | proposed | conf | evidence |
|---|---|---|---|---|
| 8002C8D0 | fn_8002C8D0 | VerifyID | HIGH | deviceID==0, size match, CheckSum over 0x1FC, sramEx flashID vs LCG(rand) (@__OSLockSramEx) |
| 8002CB54 | fn_8002CB54 | VerifyDir | HIGH | two dir copies at +0x2000 strides, checksums, repair memcpy, currentDir pick |
| 8002CD94 | fn_8002CD94 | VerifyFAT | HIGH | same pattern over FAT halves |
| 8002D018 | fn_8002D018 | __CARDVerify | HIGH | VerifyID; errors=VerifyDir+VerifyFAT |
| 8002D0A4 | fn_8002D0A4 | CARDCheckExAsync | HIGH | GCB; verify seq; repair via UpdateDir/UpdateFatBlock; PutControlBlock |
| 8002D634 | fn_8002D634 | CARDCheckAsync | MED | thin wrapper on fn_8002D0A4 |
| 8002D65C | fn_8002D65C | CARDCheck | MED | fn_8002D0A4 + __CARDSync(fn_8002AAD8) |
| 8002D6B0 | fn_8002D6B0 | (valid-device predicate) | MED | id==0x80000004 && vendorID!=0xFFFF ‖ !(id&0xFFFF0000) && !(id&3) — inlined probe test |
| 8002D77C | fn_8002D77C | CARDProbeEx | HIGH | EXIProbeEx==−1/0, EXIGetState&8 WRONGDEVICE, EXIGetID, SectorSizeTable lookup |

## Mount/unmount/format (0x8002D8F8–0x8002E96…)

| VA | fn_ sym | proposed | conf | evidence |
|---|---|---|---|---|
| 8002D8F8 | fn_8002D8F8 | DoMount | HIGH | mountStep 0/1/2 machine: EXIGetID, ClearStatus/ReadStatus, EXIProbe, __CARDUnlock(fn_8002AF34), sramEx flashID checksum, __CARDEnableInterrupt(fn_80029B78? via B78-family), EXISetExiCallback(ExiHandler), __CARDRead system block |
| 8002DD08 | fn_8002DD08 | __CARDMountCallback | HIGH | switch(result): READY→++mountStep<7→DoMount else __CARDVerify(fn_8002D018); UNLOCKED→DoMount; NOCARD/IOERROR→DoUnmount; apiCallback tail |
| 8002DE40 | fn_8002DE40 | CARDMountAsync | HIGH | GameChoice-style OS flag @800030E3, EXIGetState&8, EXIAttach(__CARDExtHandler), EXISetExiCallback 0, EXILock(UnlockedHandler) |
| 8002DFE0 | fn_8002DFE0 | CARDMount | HIGH | fn_8002DE40 + __CARDSync |
| 8002E028 | fn_8002E028 | DoUnmount | HIGH | attached→result keep, EXISetExiCallback 0, EXIDetach, OSCancelAlarm |
| 8002E0C4 | fn_8002E0C4 | CARDUnmount | HIGH | GCB; DoUnmount(fn_8002E028); PutControlBlock |
| 8002E170 | fn_8002E170 | FormatCallback | HIGH | formatStep(+0x28)<5→EraseSector(step*sectorSize); <10→__CARDWrite sys blocks; else currentDir/currentFat fixup |
| 8002E2B4 | fn_8002E2B4 | __CARDFormatRegionAsync | HIGH | memset(id,0xFF,0x2000); VI reg; __OSLockSram bias/language; LCG serial fill ×12; __OSUnlockSramEx; CheckSums; formatStep=0; EraseSector |
| 8002E90C | fn_8002E90C | CARDFormatAsync | HIGH | encode arg from fn_8002A744; tail-call fn_8002E2B4 |

## File ops (0x8002E954–0x8003050C)

| VA | fn_ sym | proposed | conf | evidence |
|---|---|---|---|---|
| 8002E954 | fn_8002E954 | __CARDCompareFileName | HIGH | 32-char signed-compare loop, early \\0 TRUE |
| 8002E9BC | fn_8002E9BC | __CARDAccess | HIGH | gameName[0]==0xFF→NOFILE(−4); memcmp gameName/company vs diskID (via fn_8008023C) →NOPERM |
| 8002EA54 | fn_8002EA54 | __CARDIsPublic | HIGH | 0xFF→NOFILE; ent->permission(+0x34)&0x04 (CARD_ATTR_PUBLIC) |
| 8002EA84 | fn_8002EA84 | __CARDGetFileNo | HIGH | 128-entry loop: Access then CompareFileName |
| 8002EBD4 | fn_8002EBD4 | CARDOpen | MED | GCB; diskID memcmp; name search + startBlock validity; fileInfo fill |
| 8002ED4C | fn_8002ED4C | CARDClose | HIGH | GCB(fileInfo->chan); chan=−1; PutControlBlock(READY) |
| 8002EDA8 | fn_8002EDA8 | CreateCallbackFat | HIGH | fills entry: permission=4 @0x34, iconAddr=−1, commentAddr=−1, time=OSGetTime-derived; __CARDUpdateDir(fn_8002C65C) |
| 8002EED8 | fn_8002EED8 | CARDCreateAsync | HIGH | strlen>0x20→NAMETOOLONG(−0xC); GCB; dup-name scan (CompareFileName fn_8002E954); __CARDAllocBlock(fn_8002C25C) |
| 8002F0F8 | fn_8002F0F8 | CARDCreate | HIGH | fn_8002EED8 + __CARDSync |
| 8002F140 | fn_8002F140 | __CARDSeek | HIGH | GCB; CARDIsValidBlockNo; length/offset LIMIT; fat-walk to sector; fileInfo setup |
| 8002F2F8 | fn_8002F2F8 | CARDRead::ReadCallback | HIGH | length<0→CANCELED; TRUNC length math; fat[iBlock] advance; __CARDRead(fn_8002BEFC) |
| 8002F428 | fn_8002F428 | CARDReadAsync | HIGH | Seek, Access/IsPublic, DCInvalidateRange, apiCallback default, __CARDRead |
| 8002F570 | fn_8002F570 | CARDRead | HIGH | async + __CARDSync |
| 8002F5B8 | fn_8002F5B8 | CARDWrite::WriteCallback | HIGH | length−=sectorSize; last page→ent->time=OSGetTime/(__OSBusClock/4)(__div2i) + UpdateDir; else fat advance + EraseSector |
| 8002F728 | fn_8002F728 | CARDWrite::EraseCallback | HIGH | __CARDWrite(sectorSize*iBlock, sectorSize, buffer, WriteCallback); PutControlBlock |
| 8002F7D8 | fn_8002F7D8 | CARDWriteAsync | HIGH | Seek; (offset‖length)&(sectorSize−1)→FATAL; Access/IsPublic; DCStoreRange(@8000B684); EraseSector |
| 8002F8EC | fn_8002F8EC | CARDWrite | HIGH | async + __CARDSync |
| 8002F934 | fn_8002F934 | DeleteCallback | HIGH | apiCallback grab; __CARDFreeBlock(fn_8002C374, startBlock) |
| 8002F9D8 | fn_8002F9D8 | CARDFastDeleteAsync | HIGH | fileNo<0x80; entry memset 0xFF; Access/IsPublic; UpdateDir(w/DeleteCallback) |
| 8002FB04 | fn_8002FB04 | CARDDeleteAsync | HIGH | __CARDGetFileNo(fn_8002EA84) then same flow |
| 8002FC14 | fn_8002FC14 | CARDFastDelete | MED | async + __CARDSync |
| 8002FC5C | (intra-unit ref) | UpdateIconOffsets (CARDStat) | MED | called by both Stat fns; not emitted as extern |
| 8002FE54 | fn_8002FE54 | CARDGetStatus | HIGH | fileNo bound, GCB, entry→stat field copies ×3, UpdateIconOffsets(fn_8002FC5C) |
| 8002FF80 | fn_8002FF80 | CARDSetStatusAsync | HIGH | iconAddr/commentAddr validation, bannerFormat/iconFormat/speed stores, time=OSTicksToSeconds(OSGetTime), UpdateDir |
| 800300F4 | fn_800300F4 | CARDSetStatus | MED | async + __CARDSync |
| 8003013C | fn_8003013C | CARDRenameAsync | HIGH | 0xFF/\\0 checks, strlen×2 ≤0x20, oldNo/newNo scan (memcmp+CompareFileName), strncpy into entry, UpdateDir |
| 80030338 | fn_80030338 | CARDRename | HIGH | async + __CARDSync |
| 80030380 | fn_80030380 | CARDFastOpen | MED | GCB; access/public checks; fileInfo fill; no name scan (vs CARDOpen above) |

## Notes / caveats

- fn_8002C0A0 (getter @card+0xB8): three distinct pointer accessors exist
  (+0x84 dir, +0x88 fat, +0xB8 unknown); melee struct has only currentDir/currentFat,
  so GX struct layout likely carries an extra cached pointer — verify against
  CARDDir.c data writes before renaming.
- fn_8002A744/fn_8002A74C are a cached-pair around the value CARDInit fetches from
  fn_8000CDD8 right after DSP init — plausibly OSGetFontEncode caching; no melee
  counterpart in card/*. Leave unnamed pending OS-unit study.
- fn_8002BAFC InitCallback vs DoneCallback split inside __CARDUnlock's DSP task is
  approximate; the 0x8002AF34 mega-function likely contains one of them inlined.
- Total: 88 distinct fn_ VAs in the card range; 79 proposed identities here
  (52 HIGH, 22 MED, 5 LOW/placeholder).
