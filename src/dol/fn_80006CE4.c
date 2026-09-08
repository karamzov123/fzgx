#include "types.h"

extern u32 DVDClose(u32);

struct fn_80006CE4_Arg0 {
    u32 unk_0;
};

u32 fn_80006CE4(struct fn_80006CE4_Arg0 *arg0, u32 arg1) {
    u32 v0;
    u32 v1;
    u32 t0;
    v0 = (u32)arg0;
    v1 = arg1;
    if ((s32)arg0->unk_0 != 1) {
    v0 += 4;
    t0 = DVDClose(v0);
    v0 = t0;
    v1 = v0;
    }
    v0 = v1;
    return v0;
}
