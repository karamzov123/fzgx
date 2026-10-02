#include "types.h"

typedef s32 (*MovieModuleCb)(void *self, u32 *out1, u32 *out2);

typedef struct MovieModuleEntry {
    u8 pad_0[0x48];
    s32 value;
    u8 pad_4c[4];
    s32 unk_50;
    u8 pad_54[0x910];
    s32 unk_964;
    u8 pad_968[0x5e8];
    MovieModuleCb unk_f50;
    volatile s32 unk_f54; /* retail emits a fresh load here instead of reusing the value compared against -5 */
    volatile s32 unk_f58; /* same: loaded in source order before the subtract, matching retail register order */
    s32 unk_f5c;
} MovieModuleEntry;

struct fn_12_2E490_Arg1 {
    u32 unk_0;
};
struct fn_12_2E490_Arg2 {
    u32 unk_0;
};

s32 fn_12_2E490(MovieModuleEntry *arg0, struct fn_12_2E490_Arg1 *arg1, struct fn_12_2E490_Arg2 *arg2) {
    u32 o2;
    u32 o1;
    s32 v;
    s32 w;
    s32 ret;
    if ((s32)arg0->value != 4 && (s32)arg0->value != -4 && (s32)arg0->value != 6 && (s32)arg0->value != -6) {
        arg1->unk_0 = -1;
        arg2->unk_0 = 1;
        v = 0;
    } else {
        v = 1;
    }
    if (v == 0) {
        return 0;
    }
    if (arg0->unk_f50 == 0) {
        arg1->unk_0 = -2;
        arg2->unk_0 = 1;
        return 0;
    }
    ret = arg0->unk_f50(arg0, &o2, &o1);
    if ((s32)arg0->value != 4) {
        w = 0;
    } else if (arg0->unk_50 != 0) {
        w = 0;
    } else if (arg0->unk_964 != 0) {
        w = 0;
    } else {
        w = 1;
    }
    if (w != 0) {
        if (arg0->unk_f54 != -5) {
            arg0->unk_f58 = arg0->unk_f58 + (o2 - arg0->unk_f54);
        }
    }
    arg0->unk_f54 = o2;
    arg0->unk_f5c = o1;
    arg1->unk_0 = arg0->unk_f58;
    arg2->unk_0 = arg0->unk_f5c;
    return ret;
}
