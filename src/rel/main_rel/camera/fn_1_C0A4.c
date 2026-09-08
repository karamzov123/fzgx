#include "types.h"

typedef struct {
    u32 unk_0;
    u32 unk_4;
    u32 unk_8;
    u32 unk_C;
    u32 unk_10;
    u32 unk_14;
    u32 unk_18;
    u32 unk_1C;
    u32 unk_20;
} CameraData;

extern CameraData lbl_1_bss_1018;
extern CameraData lbl_1_bss_10D8[];

void fn_1_C0A4(u8 index) {
    CameraData *dst = &lbl_1_bss_10D8[index];

    dst->unk_0 = lbl_1_bss_1018.unk_0;
    dst->unk_4 = lbl_1_bss_1018.unk_4;
    dst->unk_8 = lbl_1_bss_1018.unk_8;
    dst->unk_C = lbl_1_bss_1018.unk_C;
    dst->unk_10 = lbl_1_bss_1018.unk_10;
    dst->unk_14 = lbl_1_bss_1018.unk_14;
    dst->unk_18 = lbl_1_bss_1018.unk_18;
    dst->unk_1C = lbl_1_bss_1018.unk_1C;
    dst->unk_20 = lbl_1_bss_1018.unk_20;
}
