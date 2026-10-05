#include "types.h"

typedef struct TRKExceptionStatus {
    u32 unk0;
    u32 unk4;
    u32 unk8;
    u8 unkC;
    u8 unkD;
    u8 unkE;
    u8 unkF;
} TRKExceptionStatus;

typedef struct TRKBuffer {
    u32 mutex;
    BOOL isInUse;
    u32 length;
    u32 position;
    u8 data[0x880];
} TRKBuffer;

typedef struct ASMTemplate {
    u32 code[10];
} ASMTemplate;

extern TRKExceptionStatus gTRKExceptionStatus;
extern ASMTemplate lbl_80095B30;
extern ASMTemplate lbl_80095B58;
extern u8 lbl_801A5624[20];

extern void fn_8008AFF0(u32, u32);
extern int TRKAppendBuffer1_ui64(TRKBuffer *, u64);
extern int TRKReadBuffer1_ui64(TRKBuffer *, u64 *);

s32 fn_8008BB7C(u32 firstRegister, u32 lastRegister, TRKBuffer *buffer, u32 *bytesReadOrWritten, BOOL readRegisters) {
    TRKExceptionStatus *status;
    u32 sprVal;
    u32 val[2];
    TRKExceptionStatus save;
    ASMTemplate buf1;
    ASMTemplate buf2;
    ASMTemplate buf3;
    ASMTemplate buf_write;
    ASMTemplate buf_read;
    u32 reg;
    s32 err;

    if (lastRegister > 31) {
        return 0x701;
    }

    status = &gTRKExceptionStatus;
    save = *status;
    status->unkD = 0;

    buf1 = lbl_80095B30;
    buf1.code[0] = 0x7C98E2A6;
    buf1.code[1] = 0x90830000;
    buf1.code[9] = 0x4E800020;
    fn_8008AFF0((u32)&buf1, 0x28);
    ((void (*)(void *, void *))&buf1)(&sprVal, lbl_801A5624);

    sprVal |= 0xA0000000;

    buf2 = lbl_80095B30;
    buf2.code[0] = (0x8083U << 16);
    buf2.code[1] = 0x7C98E3A6;
    buf2.code[9] = 0x4E800020;
    fn_8008AFF0((u32)&buf2, 0x28);
    ((void (*)(void *, void *))&buf2)(&sprVal, lbl_801A5624);

    sprVal = 0;

    buf3 = lbl_80095B30;
    buf3.code[0] = (0x8083U << 16);
    buf3.code[1] = 0x7C90E3A6;
    buf3.code[9] = 0x4E800020;
    fn_8008AFF0((u32)&buf3, 0x28);
    ((void (*)(void *, void *))&buf3)(&sprVal, lbl_801A5624);

    *bytesReadOrWritten = 0;
    err = 0;
    for (reg = firstRegister; reg <= lastRegister && err == 0; reg++, *bytesReadOrWritten += 8) {
        if (readRegisters) {
            buf_write = lbl_80095B58;
            buf_write.code[0] = readRegisters ? ((reg << 21) | 0xF0030000) : ((reg << 21) | 0xE0030000);
            buf_write.code[9] = 0x4E800020;
            fn_8008AFF0((u32)&buf_write, 0x28);
            ((void (*)(void *, void *))&buf_write)(val, lbl_801A5624);
            err = TRKAppendBuffer1_ui64(buffer, *(u64 *)val);
        } else {
            TRKReadBuffer1_ui64(buffer, (u64 *)val);
            buf_read = lbl_80095B58;
            buf_read.code[0] = readRegisters ? ((reg << 21) | 0xF0030000) : ((reg << 21) | 0xE0030000);
            buf_read.code[9] = 0x4E800020;
            fn_8008AFF0((u32)&buf_read, 0x28);
            ((void (*)(void *, void *))&buf_read)(val, lbl_801A5624);
            err = 0;
        }
    }

    if (status->unkD != 0) {
        err = 0x702;
        *bytesReadOrWritten = 0;
    }

    gTRKExceptionStatus = save;
    return err;
}
