#include "types.h"

struct fn_80070068_lbl_801A6D30_T {
    u8 pad_0[0x14];
    u32 unk_14;
    u32 unk_18;
};

extern struct fn_80070068_lbl_801A6D30_T *lbl_801A6D30;
extern u32 GXSetCPUFifo(u32);
extern u32 fn_80032360(void);

s32 fn_80070068(void) {
    u32 v0;
    u32 v1;
    u32 t0, t1, t2;
    t0 = fn_80032360();
    v0 = lbl_801A6D30->unk_14;
    v1 = t0;
    if (t0 != v0) {
    v1 = v0;
    t1 = GXSetCPUFifo(v1);
    v1 = t1;
    v1 = 1;
    } else {
    v1 = lbl_801A6D30->unk_18;
    t2 = GXSetCPUFifo(v1);
    v1 = t2;
    v1 = 0;
    }
    return v1;
}
