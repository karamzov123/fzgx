#include "types.h"

typedef struct Fn80064D4CEntry {
    u8 pad_0[8];
    u8 unk_8;
    u8 unk_9;
    u8 unk_a;
    u8 pad_b[0xe];
    u8 unk_19;
    u8 unk_1a;
    u8 unk_1b;
    u8 unk_1c;
    u8 unk_1d;
    u8 pad_1e[2];
} Fn80064D4CEntry;

typedef struct Fn80064D4CData {
    u8 pad_0[0x580];
    Fn80064D4CEntry entries[0x40];
    u8 pad_d80[8];
    u8 indices[0x4000];
} Fn80064D4CData;

extern Fn80064D4CData *lbl_801A6C80;

void fn_80064D4C(u8 arg0, u8 arg1) {
    u8 i;
    u8 j;
    u8 idx;
    u8 flag;

    for (i = 0; i < 0x10; i++) {
        for (j = 0; j < 4; j++) {
            idx = lbl_801A6C80->indices[arg0 * 0x40 + i * 4 + j];
            if (idx == 0xff) {
                continue;
            }
            flag = 0;
            if (lbl_801A6C80->entries[idx].unk_9 & 1) {
                if (arg1 == 0) {
                    flag = 1;
                }
            } else if (arg1 == 1) {
                flag = 1;
            }
            if (flag) {
                lbl_801A6C80->entries[idx].unk_8 = 0;
                lbl_801A6C80->entries[idx].unk_9 = 0;
                *(u32 *)((u8 *)&lbl_801A6C80->entries[idx] + 0x20) = 0;
                *(u32 *)((u8 *)&lbl_801A6C80->entries[idx] + 0x24) = 0;
                lbl_801A6C80->entries[idx].unk_a = 0;
                lbl_801A6C80->entries[idx].unk_19 = 0x40;
                lbl_801A6C80->entries[idx].unk_1a = 0;
                lbl_801A6C80->entries[idx].unk_1b = 0;
                lbl_801A6C80->entries[idx].unk_1c = 0;
                lbl_801A6C80->entries[idx].unk_1d = 0;
                lbl_801A6C80->indices[arg0 * 0x40 + i * 4 + j] = 0xff;
            }
        }
    }
}
