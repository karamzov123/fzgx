#include "types.h"
#include "rel/main_rel/globals.h"
#include "rel/main_rel/motasglist.h"
typedef struct Sig_fn_1_431B8_Fn1431B8Object {
    u8 pad0[4];
    u16 flags;
    u16 index;
    u8 pad8[0xD8];
    u16 value0;
    u16 value1;
    u16 value2;
} Sig_fn_1_431B8_Fn1431B8Object;
typedef struct Fn14270CEntry {
    u8 pad0[0xD4];
    u16 counts0[6];
    u16 counts1[6];
    u16 counts2[6];
    u16 cursors0[6];
    u16 cursors1[6];
    u16 cursors2[6];
    u32 pointers0[6];
    u32 pointers1[6];
    u32 pointers2[6];
    u8 pad164[0x28];
} Fn14270CEntry;
struct fn_1_4270C_Arg1 {
    u8 pad_0[0x2];
    u16 unk_2;
    u16 unk_4;
    u16 unk_6;
};
extern const f32 lbl_1_rodata_F54;
extern const f64 lbl_1_rodata_F58;
extern s32 fn_1_431B8(Sig_fn_1_431B8_Fn1431B8Object *, Sig_fn_1_431B8_Fn1431B8Object *);
#pragma opt_dead_assignments on
#pragma opt_loop_invariants on
void fn_1_4270C(void *arg0, struct fn_1_4270C_Arg1 *arg1, u32 arg2) {
    u32 v0;
    u16 v1;
    u32 v26_2;
    u32 v2;
    u32 v3;
    s32 v4;
    u32 v5;
    s32 v6;
    u16 v7;
    u32 v8;
    s32 v9;
    s32 v10;
    u32 v11;
    u32 v12;
    u16 v13;
    u32 v14;
    u32 v15;
    s32 v16;
    s32 v17;
    u32 v18;
    u32 v19;
    u16 v20;
    u32 v2_2;
    u32 v21;
    u32 v22;
    s32 v23;
    s32 v24;
    u32 v25;
    u32 v26;
    u16 v27;
    u32 v28;
    u32 v29;
    s32 v30;
    struct { u32 value; } v31;
    u32 v32;
    u32 v33;
    v5 = *(u32 *)((((*(u32 *)((u8 *)arg0 + 36)) + (8)) + (((arg2 & 0xFFFF) << 2))));
    *(f32 *)((u8 *)arg0 + 40) = lbl_1_rodata_F54;
    *(f32 *)((u8 *)arg0 + 44) = lbl_1_rodata_F54;
    *(u16 *)((u8 *)arg0 + 2) &= 778;
    *(f32 *)((u8 *)arg0 + 48) = lbl_1_rodata_F54;
    *(f32 *)((u8 *)arg0 + 52) = lbl_1_rodata_F54;
    *(f32 *)((u8 *)arg0 + 56) = lbl_1_rodata_F54;
    if (((0) != (arg1))) {
        if (*(u16 *)((u8 *)arg0 + 68) >= arg1->unk_4) {
            *(u16 *)((u8 *)arg0 + 68) = arg1->unk_4;
        } else {
            *(u16 *)((u8 *)arg0 + 68) = *(u16 *)((u8 *)v5);
        }
        *(u16 *)((u8 *)arg0 + 66) = arg1->unk_2;
        *(u16 *)((u8 *)arg0 + 70) = arg1->unk_6;
        *(u16 *)((u8 *)arg0 + 84) = 0;
    } else {
        *(u16 *)((u8 *)arg0 + 68) = *(u16 *)((u8 *)v5);
        *(u16 *)((u8 *)arg0 + 66) = 0;
        *(u16 *)((u8 *)arg0 + 70) = 0;
        *(u16 *)((u8 *)arg0 + 84) = 0;
    }
    *(u16 *)((u8 *)arg0 + 64) = 0;
    *(f32 *)((u8 *)arg0 + 80) = (f32)(u32)*(u16 *)((u8 *)v5);
    *(f32 *)((u8 *)arg0 + 76) = (f32)(u32)*(u16 *)((u8 *)arg0 + 66);
    v2 = *(u16 *)((u8 *)v5 + 2);
    if (*(u16 *)arg0 < v2) {
        v2 = *(u16 *)arg0;
    }
    v6 = 0;
    v4 = 0;
    v5 += 4;
    while ((u32)v6 < v2) {
        v7 = *(u16 *)((u8 *)v5);
        v9 = 0;
        v10 = 0;
        v11 = v7;
        v5 += 4;
        {
    u32 fzgx_loop_v8_2766;
for (fzgx_loop_v8_2766 = 3; ((0) != (fzgx_loop_v8_2766)); fzgx_loop_v8_2766--) {
            if (((1) & (v11))) {
                v12 = v5;
                v12 = v12 + 4;
                v13 = *(u16 *)((u8 *)v5);
                v5 += 2;
                ((Fn14270CEntry *)*(u32 *)((u8 *)arg0 + 8))[v6].counts0[v9] = v13;
                ((Fn14270CEntry *)*(u32 *)((u8 *)arg0 + 8))[v6].pointers0[v10] = v12;
                v5 += ((Fn14270CEntry *)*(u32 *)((u8 *)arg0 + 8))[v6].counts0[v9] << 4;
                v5 += 2;
            } else {
                ((Fn14270CEntry *)*(u32 *)((u8 *)arg0 + 8))[v6].counts0[v9] = 0;
                ((Fn14270CEntry *)*(u32 *)((u8 *)arg0 + 8))[v6].pointers0[v10] = 0;
            }
            v11 = ((0x7FFF) & ((v11 >> 1)));
            v10++;
            ((Fn14270CEntry *)*(u32 *)((u8 *)arg0 + 8))[v6].cursors0[v9] = 0;
            v9++;
        }
    v8 = fzgx_loop_v8_2766;
}
        v16 = 0;
        v17 = 0;
        v18 = v11;
        {
    u32 fzgx_loop_v15_3672;
for (fzgx_loop_v15_3672 = 3; ((0) != (fzgx_loop_v15_3672)); fzgx_loop_v15_3672--) {
            if (((1) & (v18))) {
                v19 = v5 + 4;
                v20 = *(u16 *)((u8 *)v5);
                v5 += 2;
                ((Fn14270CEntry *)*(u32 *)((u8 *)arg0 + 8))[v6].counts1[v17] = v20;
                ((Fn14270CEntry *)*(u32 *)((u8 *)arg0 + 8))[v6].pointers1[v16] = v19;
                v5 += ((Fn14270CEntry *)*(u32 *)((u8 *)arg0 + 8))[v6].counts1[v17] << 4;
                v5 += 2;
            } else {
                ((Fn14270CEntry *)*(u32 *)((u8 *)arg0 + 8))[v6].counts1[v17] = 0;
                ((Fn14270CEntry *)*(u32 *)((u8 *)arg0 + 8))[v6].pointers1[v16] = 0;
            }
            v18 = ((0x7FFF) & ((v18 >> 1)));
            v16++;
            ((Fn14270CEntry *)*(u32 *)((u8 *)arg0 + 8))[v6].cursors1[v17] = 0;
            v17++;
        }
    v15 = fzgx_loop_v15_3672;
}
        v23 = 0;
        v24 = 0;
        {
    u32 fzgx_loop_v22_4540;
for (fzgx_loop_v22_4540 = 3; fzgx_loop_v22_4540 != 0; fzgx_loop_v22_4540--) {
            if (v18 & 1) {
                v26 = v5 + 4;
                v27 = *(u16 *)((u8 *)v5);
                v5 += 2;
                ((Fn14270CEntry *)*(u32 *)((u8 *)arg0 + 8))[v6].counts2[v24] = v27;
                ((Fn14270CEntry *)*(u32 *)((u8 *)arg0 + 8))[v6].pointers2[v23] = v26;
                v5 += ((Fn14270CEntry *)*(u32 *)((u8 *)arg0 + 8))[v6].counts2[v24] << 4;
                v5 += 2;
            } else {
                ((Fn14270CEntry *)*(u32 *)((u8 *)arg0 + 8))[v6].counts2[v24] = 0;
                ((Fn14270CEntry *)*(u32 *)((u8 *)arg0 + 8))[v6].pointers2[v23] = 0;
            }
            v18 = (v18 >> 1) & 0x7FFF;
            v23++;
            ((Fn14270CEntry *)*(u32 *)((u8 *)arg0 + 8))[v6].cursors2[v24] = 0;
            v24++;
        }
    v22 = fzgx_loop_v22_4540;
}
        v4 += 396;
        v6++;
    }
    v30 = 0;
    v31.value = 0;
    while ((u32)v30 < *(u16 *)arg0) {
        v26_2 = *(u32 *)((u8 *)arg0 + 8);
        v32 = v26_2;
        v33 = v32 + v31.value;
        if ((*(u16 *)((u8 *)v33 + 4) & 1) && *(u16 *)((u8 *)v33 + 236) == 0 && *(u16 *)((u8 *)v33 + 238) == 0 && *(u16 *)((u8 *)v33 + 240) == 0 && fn_1_431B8((Sig_fn_1_431B8_Fn1431B8Object *)v32, (Sig_fn_1_431B8_Fn1431B8Object *)v33)) {
            *(u16 *)((u8 *)v33 + 4) |= 256;
        }
        v31.value += 396;
        v30++;
    }
}
#pragma opt_loop_invariants reset

#pragma opt_dead_assignments reset
