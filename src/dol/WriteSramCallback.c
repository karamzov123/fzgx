#include "types.h"

struct WriteSramCallback_Scb_8015BFC0 {
    u8 pad_0[0x40];
    u32 unk_40;
    u8 pad_44[0x8];
    u32 unk_4C;
};

extern struct WriteSramCallback_Scb_8015BFC0 Scb_8015BFC0;
extern u32 WriteSram(u32, u32, u32);

void WriteSramCallback(void) {
    struct WriteSramCallback_Scb_8015BFC0 *p_Scb_8015BFC0;
    u32 v0;
    u32 v1;
    u32 t0;
    p_Scb_8015BFC0 = (struct WriteSramCallback_Scb_8015BFC0 *)&Scb_8015BFC0;
    v0 = (u32)((u8 *)(u32)p_Scb_8015BFC0 + 64);
    v1 = p_Scb_8015BFC0->unk_40;
    p_Scb_8015BFC0->unk_4C = (WriteSram((u32)((u8 *)(u32)p_Scb_8015BFC0 + v1), v1, (64 - v1)));
    if ((s32)p_Scb_8015BFC0->unk_4C != 0) {
    *(u32 *)((u8 *)v0 + 0) = 64;
    }
}
