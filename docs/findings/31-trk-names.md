# MetroTRK naming study — dolphin/metrotrk/* units (GFZE01)

Method: cross-reference GX fn profiles (`/tmp/gx_fn_db.json`, `findings/26`) against the melee
MetroTRK v2.0 reference (`/tmp/opencode/melee/src/MetroTRK/*` — same NUB; GX string
`MessageSend : cc_write returned %ld` @0x80095678, `ECUBE v2.0`). Evidence keys:
call-graph shape, struct/data refs, literal strings (dumped from DOL rodata),
command IDs (msgcmd.h), sizes/register-range bounds.

Confidence: **HIGH** = structural/string anchor; MED = strong call-graph inference;
LOW = plausible, needs disassembly review.

## Anchor strings recovered from DOL rodata

| VA | String |
|---|---|
| 80095678 | `MessageSend : cc_write returned %ld` |
| 800956A0 | `ERROR : No buffer available` |
| 800956C0 | `TRK_Packet_Header… / TRK_CMD_ReadMemory / … / TestForPacket returning %ld` block |
| 80095890 | `\nMetroTRK Option : SerialIO - ` (+ `Enable`) |
| 800958C4 | `DoContinue` |
| 800958F0 | `SendACK : Calling MessageSend` |
| 80095910 | `MessageSend err : %ld` |
| 80095A78 | `Calling MessageSend` |
| 80095BA8 | `TargetDoStep()` |
| 80095B30/58 | `TRK_Main ` / `END%s\n` region |

## trk_80088B00.o

| VA | fn_ sym | proposed | conf | evidence |
|---|---|---|---|---|
| 80088764 | fn_80088764 | TRKConstructEvent | MED | nubevent.o; `(event, type=2)` call shape from 8944C |
| 8008877C | fn_8008877C | TRKPostEvent | MED | nubevent.o; mutex-guarded queue append, called with id −1 reset |
| 80088B00 | fn_80088B00 | TRKMessageSend | HIGH | `TRK_WriteUARTN(fData, fLength)` + `cc_write returned %ld` MWTRACE; = melee msg.c body verbatim |
| 80088B44 | fn_80088B44 | TRKAppendBuffer_ui32 | MED | gTRKBigEndian loop; used by AccessDefault/Extended1 pair |
| 80088C34 | fn_80088C34 | TRKReadBuffer | LOW | memcpy-based bulk read; called by SuppAccessFile |
| 80088CCC | fn_80088CCC | TRKAppendBuffer1_ui64 | MED | 8-byte big-endian writer; FP/Ext2 paths |
| 80088DB4 | fn_80088DB4 | TRKReadBuffer_ui32 | MED | counterpart of 88B44 |
| 80088EB0 | fn_80088EB0 | TRKAppendBuffer (byte-generic) | HIGH | byte loop w/ `cmplwi 0x880` overflow (kMessageBufferSize) + pos/len update |
| 80088F18 | fn_80088F18 | TRKReadBuffer1_ui64 | MED | counterpart of 88CCC |
| 80089014 | fn_80089014 | TRKAppendBuffer1_ui32 | LOW | small value-appender used by Read/WriteMemory handlers |
| 800890A0 | fn_800890A0 | TRKReadBuffer1_ui16/_ui32 | LOW | small value-reader, same handlers |
| 80089144 | fn_80089144 | TRKSetBufferPosition | HIGH | `0x880` bound, sets fPosition, raises fLength — exact melee body |
| 80089174 | fn_80089174 | TRKMessageIntoReply | MED-HIGH | writes cmd+error bytes at pos, then handlers call MessageSend |
| 800891B4 | fn_800891B4 | TRKReleaseBuffer | HIGH | indexes gTRKMsgBufs (lbl_801A36E8, size 0x19B0 ≈ 3×0x88C), clears fInUse |
| 80089244 | fn_80089244 | TRKGetFreeBuffer | HIGH | mutex stubs + free-slot scan + `ERROR : No buffer available` |
| 8008944C | fn_8008944C | TRKProcessInput | HIGH | ConstructEvent(type 2), fBufferID=−1, PostEvent — melee serpoll verbatim |
| 800894FC | fn_800894FC | TRKTestForPacket | HIGH | `TRK_Packet_Header`/`TestForPacket returning %ld` traces, PollUART+ReadUARTN+GetFreeBuffer |
| 8008963C | fn_8008963C | usr_put (char out) | MED | walks string, masks via gTRKInputPendingPtr, OSReport per char (v0.12-style usr_put) |
| 8008983C | fn_8008983C | TRKDoPing (cmd 0x00) | MED | dispatch slot 0; `MetroTRK Option : SerialIO - Enable` + ACK 0x80 |
| 800898E4 | fn_800898E4 | TRKDoVersions (cmd 0x04) | MED | switch returning 0x704/0x706/0x21 version codes |
| 8008998C | fn_8008998C | TRKDoStop / stop-path handler (cmd slot) | LOW | emits ACK 0x80 w/ codes 0x11/0x12/0x16, touches gTRKStepStatus via B784/B6CC |
| 80089BAC | fn_80089BAC | TRKDoContinue (cmd 0x18) | HIGH | `DoContinue` trace; TRKTargetStopped gate → ACK → TRKTargetContinue |
| 80089C5C | fn_80089C5C | TRKDoReadRegisters (cmd 0x12) | HIGH | `MessageSend err : %ld`; switches Access{Default,FP,Ext1,Ext2} read |
| 80089EEC | fn_80089EEC | TRKDoWriteRegisters (cmd 0x13) | HIGH | same accessor switch, write direction, `Enable` trace |
| 8008A1CC | fn_8008A1CC | TRKDoReadMemory (cmd 0x10) | MED-HIGH | reads ui8/ui8/ui16/ui32 header, AccessMemory, error-switch jumptable |
| 8008A3C0 | fn_8008A3C0 | TRKDoWriteMemory (cmd 0x11) | MED-HIGH | same frame + AccessMemory write + double ui16 readback |
| 8008A5AC | fn_8008A5AC | TRKDoUnsupported (slot 7 Override stub) | LOW | `return 0` |
| 8008A5B4 | fn_8008A5B4 | TRKDoUnsupported (slot 5 SupportMask stub) | LOW | `return 0` |
| 8008A5BC | fn_8008A5BC | TRKDoOverride (cmd 0x07) | MED | ACK then fn_8008D028 (TRKTargetTranslate/vector copy path) |
| 8008A614 | fn_8008A614 | TRKDoReset (cmd 0x03) | MED | ACK then bl 0x80005518 (`__TRK_reset` region, .init) |
| 8008A66C | fn_8008A66C | TRKDoDisconnect (cmd 0x02) | MED-HIGH | ACK + ConstructEvent(shutdown)+PostEvent — melee TRKDoDisconnect exact |
| 8008A6E4 | fn_8008A6E4 | TRKDoSetOption (cmd 0x17) | LOW | small; ACK + gTRKState touch |
| 8008A748 | fn_8008A748 | TRK_SetInputPendingPtrStore (serpoll-level ptr store) | LOW | `stw r3 → 801A50B0` |
| 8008A754 | fn_8008A754 | TRK_IsInputPending (deref of 801A50B0) | MED | `lwz r3,0(ptr)`; gates every file-op + usr_put |
| 8008A764 | fn_8008A764 | usr_put string print helper | LOW | `'\n'` + MWTRACE loop |
| 8008A80C | fn_8008A80C | TRK_PositionFile client op (cmd 0xD4) | MED-HIGH | builds 0xD4 request, RequestSend(p1=3) |
| 8008A91C | fn_8008A91C | TRK_CloseFile client op (cmd 0xD3) | MED-HIGH | builds 0xD3 request |
| 8008AA04 | fn_8008AA04 | TRK_OpenFile client op (cmd 0xD2) | MED-HIGH | strlen on filename, 0xD2 request, tries=7 |
| 8008AB20 | fn_8008AB20 | TRKRequestSend | HIGH | `Calling MessageSend` trace; MessageSend/TestForPacket/ProcessInput retry loop — melee support.c exact |
| 8008AD00 | fn_8008AD00 | TRKSuppAccessFile | HIGH | 0x800-chunk loop, 0xD0/0xD1 selection, io_result 0x302 handling, need_reply — melee support.c exact |
| 8008AF40 | fn_8008AF40 | TRKAcquireMutex (stub) | HIGH | `li r3,0; blr` — melee mutex_TRK.c v2 stub |
| 8008AF48 | fn_8008AF48 | TRKReleaseMutex (stub) | HIGH | same |
| 8008AF58 | fn_8008AF58 | TRKDoNotifyStopped | HIGH | GetFreeBuffer + AddStopInfo + AddExceptionInfo + RequestSend + ReleaseBuffer — melee exact |
| 8008B028 | fn_8008B028 | TRKTargetCheckException | LOW | orphan (h-declared, unused in melee too) |
| 8008B0E0 | fn_8008B0E0 | __TRK_get_MSR | MED | leaf spr read used by FP/AccessMemory |
| 8008B0E8 | fn_8008B0E8 | __TRK_set_MSR | MED | counterpart |
| 8008B0F0 | fn_8008B0F0 | TRK_ppc_memcpy MSR-glue helper | LOW | 60B, called only by AccessMemory |
| 8008B484 | fn_8008B484 | TRKTargetVersions (value fetch) | MED | 24B load from gTRKState consumed by DoVersions |
| 8008AFF0 | fn_8008AFF0 | TRK_memcpy (mem_TRK) | MED-HIGH | 56B generic copy used everywhere memcpy appears |
| 8008B6BC | fn_8008B6BC | TRKTargetGetPC | MED-HIGH | single load gTRKCPUState.Default.PC |
| 8008B6CC | fn_8008B6CC | TRKTargetCheckStep | HIGH | `TargetDoStep()` trace + gTRKStepStatus (lbl_8015B884) logic |
| 8008B784 | fn_8008B784 | TRKTargetDoStep | MED-HIGH | same data; sets active + enable trace |
| 8008B830 | fn_8008B830 | TRKTargetAddExceptionInfo | MED-HIGH | exceptionInfo PC/instr/id append via C6E4+88EB0 |
| 8008B8B4 | fn_8008B8B4 | TRKTargetAddStopInfo | MED-HIGH | gTRKCPUState PC/instr/id append |
| 8008BB7C | fn_8008BB7C | TRKTargetAccessFP | HIGH | `cmplwi r4,0x1f` reg bound; FP reg save/restore instr block (0x7c99…/4e80…) |
| 8008BFB4 | fn_8008BFB4 | TRKTargetAccessExtended1 | HIGH | `cmplwi 0x60` bound + gTRKRestoreFlags TBR/DEC logic — melee unique |
| 8008C124 | fn_8008C124 | TRKTargetAccessExtended2 | HIGH | `cmplwi 0x21` bound; `ori r3,r3,0x2000` MSR (FP enable) + HID2/GQR paired-single path |
| 8008C5F0 | fn_8008C5F0 | TRKTargetAccessDefault | HIGH | `cmplwi 0x24` = 36 default regs (32 GPR+CR/LR/CTR/XER) |
| 8008C6E4 | fn_8008C6E4 | TRKTargetReadInstruction | MED-HIGH | wraps AccessMemory for 4-byte read; used by AddStop/ExceptionInfo |
| 8008C730 | fn_8008C730 | TRKTargetAccessMemory | HIGH | Translate(D154? see main.o)+ValidMemory32+flush+memcpy+exception guard — melee exact shape |
| 8008C87C | fn_8008C87C | TRKValidMemory32 | HIGH | self-recursion (range split) + lbl_80095B20 = gTRKMemMap (18 refs) |

## main.o

| VA | fn_ sym | proposed | conf | evidence |
|---|---|---|---|---|
| 8008D028 | fn_8008D028 | TRKTargetTranslate | HIGH | `0x3FFFFFFF\|0x80000000` mask, DBAT3 window check, gTRKInterruptVectorTable ref |
| 8008D154 | fn_8008D154 | TRK_flush_cache | MED | gTRKCPUState+lbl_801A5638; called by AccessMemory (melee calls TRK_flush_cache there) |
| 8008D398 | fn_8008D398 | **TRK_WriteUARTN** | HIGH | `gDBCommTable.write_func` (tbl+0x14) via bctrl; `neg/or/srawi` = `!r3?kNoError:kUARTError`. NOT a dispatch routine. Callers are msghndlr ACK/SendACK paths (22 fan-in = every reply sender) |
| 8008D3D4 | fn_8008D3D4 | TRK_ReadUARTN | HIGH | `gDBCommTable.read_func` (tbl+0x10); used by TestForPacket payload reads |
| 8008D410 | fn_8008D410 | TRKPollUART | HIGH | `gDBCommTable.peek_func` (tbl+0xC) |
| 8008D440 | fn_8008D440 | EnableEXI2Interrupts | HIGH | `gDBCommTable.initinterrupts_func` (tbl+0x8), gated on TRK_Use_BBA; called by named EnableMetroTRKInterrupts |
| 8008D7B0 | fn_8008D7B0 | AMC_IsStub | MED | reads stub flag 801A5650 (melee InitMetroTRKCommTable calls AMC_IsStub/Hu_IsStub) |
| 8008D7C0 | fn_8008D7C0 | AMC_SetStub | MED | writes 801A5650 |
| 8008D7CC | fn_8008D7CC | TRK_PositionFile (game-side, cmd 0xD4) | MED | mirrors 8008A80C; called from game debug hook fn_8007F684/fn_80085814 cluster |
| 8008D8A8 | fn_8008D8A8 | TRK_CloseFile (game-side, cmd 0xD3) | MED | mirrors A91C |
| 8008D92C | fn_8008D92C | TRK_OpenFile (game-side, cmd 0xD2) | MED | parses open-mode bits (rlwinm 0x1d/0x1a fields = melee DSOpenMode flags) |
| 8008DAA8 | fn_8008DAA8 | TRK_WriteFile (game-side, cmd 0xD0) | MED | mirrors AD00 inner chunk |
| 8008DB5C | fn_8008DB5C | TRK_ReadFile (game-side, cmd 0xD1) | MED | mirrors AD00 read path |
| 8008DC10 | fn_8008DC10 | TRK_WriteFileChecked (stub-gated 0xD0) | LOW | extra AMC_IsStub gate |
| 8008DCCC | fn_8008DCCC | TRK_ReadFileChecked (stub-gated 0xD1) | LOW | twin of DC10 |

Note: the D7CC–DCCC family are the host-facing file-I/O wrappers used by the game's
debug hooks (external callers `fn_8007F684`, `fn_80085814`); they duplicate the
A80C–AD00 (NUB-side SuppAccessFile/client ops) command encodings.

## custom.o

| VA | fn_ sym | proposed | conf | evidence |
|---|---|---|---|---|
| 8008E114 | fn_8008E114 | EXI2_ReadN | MED | interrupt-guarded memcpy-in loop (ddh/gdev `_cc_read` shape) |
| 8008E21C | fn_8008E21C | EXI2_WriteN | MED | memcpy-out counterpart |
| 8008E324 | fn_8008E324 | EXI2_Poll | LOW | calls nop fn_8008E76C, returns status |
| 8008E374 | fn_8008E374 | EXI2 nop stub | LOW | adjacent 8B stub |
| 8008E718 | fn_8008E718 | TRKReleaseMutex (OSRestoreInterrupts) | MED-HIGH | melee mutex_TRK.c real impl |
| 8008E73C | fn_8008E73C | TRKAcquireMutex (OSDisableInterrupts) | MED-HIGH | counterpart |
| 8008E770 | fn_8008E770 | EXI2 interrupt enable/handler attach | LOW | lbl_801A6678 (EXI context) ×5, __OSUnmaskInterrupts |
| 8008E9B4 | fn_8008E9B4 | EXI2 interrupt disable | LOW | counterpart |
| 8008EB8C | fn_8008EB8C | EXI2 lock/unlock helper | LOW | mask/unmask pair |
| 8008EBF8 | fn_8008EBF8 | EXI2 state get | LOW | |
| 8008EC78 | fn_8008EC78 | EXI2 state set | LOW | |
| 8008ECE0 | fn_8008ECE0 | EXI2 callback getter | LOW | |
| 8008ED30 | fn_8008ED30 | EXI2_SetInterruptHandler | LOW | __OSSetInterruptHandler + unmask |
| 8008ED70 | fn_8008ED70 | EXI2 buffer init (memset) | LOW | |
| 8008EDB0 | fn_8008EDB0 | EXI2 status query | LOW | |
| 8008EDF0 | fn_8008EDF0 | EXI2_Init | MED | orchestrates ED70/EBF8/E770/E9B4/EC78 — full init sequence |

## userregion.o

| VA | fn_ sym | proposed | conf | evidence |
|---|---|---|---|---|
| 8008EEAC | fn_8008EEAC | UserRegion init glue | LOW | ties custom.o mutexes + handler install |
| 8008EED8 | fn_8008EED8 | UserRegion read handler | LOW | EXI2_ReadN path + lbl_8015B900 string |
| 8008EFE0 | fn_8008EFE0 | UserRegion write handler | LOW | EXI2_WriteN path |
| 8008F29C | fn_8008F29C | UserRegion control/poll | LOW | |

## uart.o (hardware serial fallback driver)

| VA | fn_ sym | proposed | conf | evidence |
|---|---|---|---|---|
| 8008F45C | fn_8008F45C | InitializeUART (int-driven) | MED | 608B init: masks/unmasks ints, installs handlers |
| 8008F6BC | fn_8008F6BC | EnableUARTInterrupts | LOW | lbl_801A6E2C/34/3C flag trio |
| 8008F748 | fn_8008F748 | DisableUARTInterrupts | LOW | counterpart |
| 8008F7E4 | fn_8008F7E4 | UART_InstallInterruptHandlers | MED | __OSSetInterruptHandler ×2 (RX=fn_8008F8B0, TX=fn_8008F8F0) |
| 8008F838 | fn_8008F838 | UART poll/tick | LOW | |
| 8008F8B0 | fn_8008F8B0 | UART RX ISR | MED | referenced as handler |
| 8008F8F0 | fn_8008F8F0 | UART TX ISR | MED | referenced as handler |
| 8008F92C | fn_8008F92C | UART_ReadN internal | MED | drives fn_8008FC3C engine |
| 8008F9D8 | fn_8008F9D8 | UART_WriteN internal | MED | same |
| 8008FAB4 | fn_8008FAB4 | UART_ReadN polled variant | LOW | |
| 8008FB90 | fn_8008FB90 | UART_WriteN polled variant | LOW | |
| 8008FC3C | fn_8008FC3C | UART byte engine (shift in/out) | MED-HIGH | 0x298-bit-loop core under all four above |

## Cross-checks against named symbols (all consistent)

- `TRKNubMainLoop` @80088648 calls GetNextEvent → {DispatchMessage, TRKTargetInterrupt,
  TRKTargetSupportRequest}, GetInput, Idle(→TargetContinue/Stopped) — matches melee exactly.
- `TRKInitializeNub` @800889B4 call chain = melee nubinit.c order incl.
  InitializeProgramEndTrap (GX extra) and TRKInitializeIntDrivenUART.
- `TRK_main` @8008D1CC: MWTRACE(`TRK_Main`) → TRKInitializeNub → TRKNubWelcome →
  TRKNubMainLoop → TRKTerminateNub.
