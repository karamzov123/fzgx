#include "types.h"

typedef struct {
    u8 pad[2];
    s16 value;
    u8 rest[0x1f8];
} Entry;

typedef struct {
    u8 pad_0[0x48];
    u8 mode;
    u8 unk_49;
    u8 unk_4A;
    u8 unk_4B;
    u8 unk_4C;
    u8 rest[0x23];
} CameraState;

extern s8 fn_1_86624(void);
extern s8 fn_1_86634(s32 index);
extern Entry *game_camera_entries;
extern CameraState *lbl_1_bss_F68;
extern u8 lbl_1_data_3318[180];
extern char lbl_1_data_35E8[19];
extern void fn_8000C49C(...);

void fn_1_8298(void) {
    s8 found;
    s8 count;
    s32 i;

    found = 0;
    count = fn_1_86624();

    for (i = 0; i < 4; i++) {
        game_camera_entries[i].value = -1;
    }

    for (i = 0; i < count; i++) {
        s8 index = fn_1_86634(i);
        if (index != -1) {
            found++;
            game_camera_entries[index].value = i;
        }
    }

    switch (found) {
    case 0:
        fn_8000C49C(lbl_1_data_3318, 0x7d2, lbl_1_data_35E8);
        break;
    case 1:
        lbl_1_bss_F68->mode = 0;
        break;
    case 2:
        switch (lbl_1_bss_F68->unk_4B) {
        case 0:
            lbl_1_bss_F68->mode = 1;
            break;
        case 1:
            lbl_1_bss_F68->mode = 2;
            break;
        }
        break;
    case 3:
        switch (lbl_1_bss_F68->unk_4C) {
        case 0:
            lbl_1_bss_F68->mode = 3;
            break;
        case 1:
            lbl_1_bss_F68->mode = 4;
            break;
        case 2:
            lbl_1_bss_F68->mode = 5;
            break;
        case 3:
            lbl_1_bss_F68->mode = 6;
            break;
        case 4:
            lbl_1_bss_F68->mode = 6;
            break;
        }
        break;
    case 4:
        lbl_1_bss_F68->mode = 8;
        break;
    }
}
