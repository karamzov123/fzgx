#include "types.h"

typedef struct Fn80064A30Entry {
    u8 unk_0;
    u8 unk_1;
    u8 unk_2;
    u8 unk_3;
    u8 pad_4[0x14];
    u32 unk_18;
    void *unk_1c;
    u8 pad_20[0xf8];
} Fn80064A30Entry;

typedef struct Fn80064A30Data {
    u8 pad_0[0x444];
    u32 unk_444;
    u8 pad_448[0xfc0];
    Fn80064A30Entry entries[0x40];
} Fn80064A30Data;

extern Fn80064A30Data *lbl_801A6C80;
extern void fn_8005FBDC(u32 arg0);
extern void fn_8005FCD8(u32 arg0);
extern void fn_8005C9D8(u32 arg0);
extern void fn_8005C478(u32 arg0);
extern void fn_8005C298(u32 arg0, u32 arg1);
extern void fn_8005C7C0(u32 arg0);
extern void fn_8005C5C8(u32 arg0);

void fn_80064A30(u16 arg0, u32 arg1) {
    u32 i;

    for (i = 0; i < 0x40; i++) {
        if (lbl_801A6C80->entries[i].unk_0 == 0xff) {
            continue;
        }
        if (lbl_801A6C80->entries[i].unk_0 == 3) {
            continue;
        }
        if (lbl_801A6C80->entries[i].unk_0 == 4) {
            continue;
        }
        if ((lbl_801A6C80->unk_444 & arg1) !=
            (lbl_801A6C80->entries[i].unk_18 & arg1)) {
            continue;
        }
        switch (arg0) {
        case 0x8000:
            if (lbl_801A6C80->entries[i].unk_1 != 1) {
                break;
            }
            if ((lbl_801A6C80->entries[i].unk_3 & 8) == 8) {
                break;
            }
            if (lbl_801A6C80->entries[i].unk_3 & 1) {
                lbl_801A6C80->entries[i].unk_3 |= 2;
            } else {
                fn_8005FBDC(i);
            }
            break;
        case 0xA001:
        case 0xA004:
        case 0xA005:
        case 0xA010:
        case 0xA011:
        case 0xA034:
        case 0xA040:
            fn_8005C9D8(i);
            break;
        case 0xA002:
            if (lbl_801A6C80->entries[i].unk_0 == 1) {
                fn_8005FCD8(i);
            }
            break;
        case 0xA007:
            fn_8005C478(i);
            break;
        case 0xA01C:
            if (lbl_801A6C80->entries[i].unk_0 == 1) {
                fn_8005C9D8(i);
            }
            break;
        case 0xB001:
            fn_8005C298(i, 1);
            break;
        case 0xB002:
            fn_8005C298(i, 2);
            break;
        case 0xB007:
            fn_8005C9D8(i);
            break;
        case 0xB00A:
            if (((u8 *)lbl_801A6C80->entries[i].unk_1c)[6] & 0x80) {
                fn_8005C7C0(i);
            }
            break;
        case 0xB00D:
            if (((u8 *)lbl_801A6C80->entries[i].unk_1c)[0x15] != 0) {
                fn_8005C5C8(i);
            }
            break;
        case 0xB040:
            lbl_801A6C80->entries[i].unk_3 &= 0xfe;
            if (((u8 *)lbl_801A6C80)[lbl_801A6C80->entries[i].unk_2 * 32 +
                0x59a] != 0) {
                lbl_801A6C80->entries[i].unk_3 |= 1;
            } else if (lbl_801A6C80->entries[i].unk_3 & 2) {
                fn_8005FBDC(i);
            }
            break;
        case 0xB078:
            if ((lbl_801A6C80->entries[i].unk_18 & 0xf000000) !=
                (lbl_801A6C80->unk_444 & 0xf000000)) {
                break;
            }
            fn_8005FCD8(i);
            break;
        case 0xE000:
            fn_8005C478(i);
            break;
        }
    }
}
