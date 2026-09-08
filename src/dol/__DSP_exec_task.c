#include "types.h"

extern void DSPSendMailToDSP(u32 mail);
extern u32 DSPCheckMailToDSP(void);

typedef struct DSPTaskInfo {
    u32 unk_0;
    u8 unk_4[0x8];
    u32 unk_c;
    u32 unk_10;
    u32 unk_14;
    u32 unk_18;
    u32 unk_1c;
    u32 unk_20;
    u16 unk_24;
    u16 unk_26;
} DSPTaskInfo;

void __DSP_exec_task(DSPTaskInfo *task, DSPTaskInfo *next) {
    if (task != 0) {
        DSPSendMailToDSP(task->unk_18);
        while (DSPCheckMailToDSP() != 0) {
        }
        DSPSendMailToDSP(task->unk_1c);
        while (DSPCheckMailToDSP() != 0) {
        }
        DSPSendMailToDSP(task->unk_20);
        while (DSPCheckMailToDSP() != 0) {
        }
    } else {
        DSPSendMailToDSP(0);
        while (DSPCheckMailToDSP() != 0) {
        }
        DSPSendMailToDSP(0);
        while (DSPCheckMailToDSP() != 0) {
        }
        DSPSendMailToDSP(0);
        while (DSPCheckMailToDSP() != 0) {
        }
    }

    DSPSendMailToDSP(next->unk_c);
    while (DSPCheckMailToDSP() != 0) {
    }
    DSPSendMailToDSP(next->unk_10);
    while (DSPCheckMailToDSP() != 0) {
    }
    DSPSendMailToDSP(next->unk_14);
    while (DSPCheckMailToDSP() != 0) {
    }

    if (next->unk_0 == 0) {
        DSPSendMailToDSP(next->unk_24);
        while (DSPCheckMailToDSP() != 0) {
        }
        DSPSendMailToDSP(0);
        while (DSPCheckMailToDSP() != 0) {
        }
        DSPSendMailToDSP(0);
        while (DSPCheckMailToDSP() != 0) {
        }
        DSPSendMailToDSP(0);
        while (DSPCheckMailToDSP() != 0) {
        }
    } else {
        DSPSendMailToDSP(next->unk_26);
        while (DSPCheckMailToDSP() != 0) {
        }
        DSPSendMailToDSP(next->unk_18);
        while (DSPCheckMailToDSP() != 0) {
        }
        DSPSendMailToDSP(next->unk_1c);
        while (DSPCheckMailToDSP() != 0) {
        }
        DSPSendMailToDSP(next->unk_20);
        while (DSPCheckMailToDSP() != 0) {
        }
    }
}
