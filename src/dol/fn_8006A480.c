#include "types.h"

struct fn_8006A480_Entry {
    u32 unk_0;
    u32 unk_4;
    u32 unk_8;
};
struct fn_8006A480_Arg0 {
    u32 unk_0;
    struct fn_8006A480_Entry *entries;
};
struct fn_8006A480_Out {
    void *unk_0;
    u32 unk_4;
    u32 unk_8;
};
extern s32 fn_8006A554(struct fn_8006A480_Arg0 *, u32, struct fn_8006A480_Out *);
extern void fn_8006A8CC(struct fn_8006A480_Arg0 *, void *, u32);
extern char lbl_801327AC[70];
extern void OSReport(const char *, ...);

s32 fn_8006A480(struct fn_8006A480_Arg0 *arg0, u32 arg1, struct fn_8006A480_Out *arg2) {
    struct fn_8006A480_Entry *entries;
    s32 idx;
    s32 flag;
    char buf[0x80];
    entries = arg0->entries;
    idx = fn_8006A554(arg0, arg1, arg2);
    if (idx < 0) {
        fn_8006A8CC(arg0, buf, 0x80);
        OSReport(lbl_801327AC, arg1, buf);
        return 0;
    }
    if (idx < 0 || ((entries[idx].unk_0 & 0xFF000000) == 0 ? 0 : 1)) {
        return 0;
    }
    arg2->unk_0 = arg0;
    arg2->unk_4 = entries[idx].unk_4;
    arg2->unk_8 = entries[idx].unk_8;
    return 1;
}
