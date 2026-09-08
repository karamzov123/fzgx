#include "types.h"

typedef struct CameraGlobals {
    u8 pad_00[0x08];
    u32 unk_08;
    u32 unk_0C;
    u32 unk_10;
    u32 unk_14;
    u32 unk_18;
    u32 unk_1C;
    u32 unk_20;
    u32 unk_24;
    u32 unk_28;
    u8 pad_2C[0x10];
    u32 unk_3C;
    u8 pad_40[0x88];
    u32 src_C8;
    u32 src_CC;
    u32 src_D0;
    u32 src_D4;
    u32 src_D8;
    u32 src_DC;
    u32 src_E0;
    u32 src_E4;
    u32 src_E8;
} CameraGlobals;

extern CameraGlobals lbl_1_bss_1010;

// Snapshots the current camera values and resets the transient state.
void camera_snapshot(void) {
    lbl_1_bss_1010.unk_08 = lbl_1_bss_1010.src_C8;
    lbl_1_bss_1010.unk_0C = lbl_1_bss_1010.src_CC;
    lbl_1_bss_1010.unk_10 = lbl_1_bss_1010.src_D0;
    lbl_1_bss_1010.unk_14 = lbl_1_bss_1010.src_D4;
    lbl_1_bss_1010.unk_18 = lbl_1_bss_1010.src_D8;
    lbl_1_bss_1010.unk_1C = lbl_1_bss_1010.src_DC;
    lbl_1_bss_1010.unk_20 = lbl_1_bss_1010.src_E0;
    lbl_1_bss_1010.unk_24 = lbl_1_bss_1010.src_E4;
    lbl_1_bss_1010.unk_28 = lbl_1_bss_1010.src_E8;
    lbl_1_bss_1010.unk_3C = 0;
}
