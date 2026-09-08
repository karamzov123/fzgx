#include "types.h"

typedef struct {
    u8 pad_0[0xa4];
    s16 unk_A4;
    u8 pad_A6[2];
    s16 unk_A8;
    u8 pad_AA[0x152];
} GameCameraEntry;

typedef struct {
    u8 pad_0[8];
    u16 unk_8;
    u8 pad_A[0xa];
} CameraTableEntry;

extern GameCameraEntry *game_camera_entries;
extern s32 lbl_1_bss_F78;
extern CameraTableEntry lbl_1_bss_6F1E4;
extern void fn_1_A2DF4(u32, u32, u32);

void fn_1_715C(u8 index, s16 value) {
    if (game_camera_entries[index].unk_A8 == 0 && index < 4) {
        if (value == 0) {
            if (lbl_1_bss_F78 == 0) {
                fn_1_A2DF4(index, 0xa5000000, 8);
                lbl_1_bss_F78 = 1;
            }
        } else if (game_camera_entries[index].unk_A4 == 0) {
            if (lbl_1_bss_F78 != 0) {
                fn_1_A2DF4(index, 0xa5000000, (&lbl_1_bss_6F1E4)[index].unk_8);
                lbl_1_bss_F78 = 0;
            }
        }
    }
    game_camera_entries[index].unk_A4 = value;
}
