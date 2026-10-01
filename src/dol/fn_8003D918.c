#include "types.h"

typedef struct Entry {
    u32 unk0;
    u32 unk4;
    u32 unk8;
    u32 unkC;
} Entry;

typedef u32 (*AllocFn)(u32);

extern const char *lbl_801A6580;
extern AllocFn lbl_801A6C34;
extern u32 lbl_801A6C38;
extern Entry *lbl_801A6C48;
extern Entry *lbl_801A6C4C;
extern u32 lbl_801A6C50;
extern u32 lbl_801A6C54;
extern u32 lbl_801A6C58[2];
extern void OSRegisterVersion(const char *);
extern void fn_8003E9EC(u32);
extern void fn_8003D698(void);
extern void fn_80034378(void (*func)(void));
extern void fn_80039B38(void);

u32 fn_8003D918(u32 arg0, u32 arg1, u32 arg2, AllocFn arg3, u32 arg4, u32 arg5) {
    u32 size;
    u32 i;

    OSRegisterVersion(lbl_801A6580);
    lbl_801A6C34 = arg3;
    lbl_801A6C38 = arg4;
    lbl_801A6C58[0] = arg1;
    lbl_801A6C54 = arg2;
    lbl_801A6C50 = arg0;
    size = arg1 * 16 + arg1 * (arg0 * 0xb0);
    size += arg2 * 16;
    lbl_801A6C4C = (Entry *)arg3(arg1 * 16);
    for (i = 0; i < lbl_801A6C58[0]; i++) {
        lbl_801A6C4C[i].unk0 = lbl_801A6C34(arg0 * 0xb0);
        lbl_801A6C4C[i].unk4 = 0;
    }
    lbl_801A6C48 = (Entry *)lbl_801A6C34(arg2 * 16);
    for (i = 0; i < arg2; i++) {
        lbl_801A6C48[i].unk0 = 0;
        lbl_801A6C48[i].unk8 = -1;
    }
    fn_8003E9EC(arg5);
    fn_80034378(fn_8003D698);
    fn_80039B38();
    return size;
}
