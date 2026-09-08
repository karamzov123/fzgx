#include "types.h"

/* fzgx:begin fn_1_45890 */
extern u32 lbl_1_bss_384D0[2];

void fn_1_45890(void) {
    lbl_1_bss_384D0[0] = 0;
}
/* fzgx:end fn_1_45890 */

/* fzgx:begin fn_1_45AD4 */
extern s32 lbl_1_bss_384D0;
extern s32 lbl_1_bss_3DCE4[17];

int fn_1_45AD4(void) {
    s8 i;

    if (lbl_1_bss_384D0 != 0) {
        return 1;
    }

    for (i = 0; i < 16; i++) {
        if (lbl_1_bss_3DCE4[i] != 0) {
            return 1;
        }
    }

    return 0;
}
/* fzgx:end fn_1_45AD4 */

/* fzgx:begin fn_1_45B68 */
typedef struct Fn45B68Object {
    s32 state;
    u8 _pad[0x50];
    void *value;
} Fn45B68Object;

extern void *fn_80006DFC(void *arg);

void *fn_1_45B68(Fn45B68Object *obj) {
    switch (obj->state) {
    case 1:
        return obj->value;
    default:
        return fn_80006DFC((u8 *)obj + 4);
    }
}
/* fzgx:end fn_1_45B68 */

/* fzgx:begin fn_1_45BA4 */
extern u32 lbl_1_bss_384C8;
extern void fn_1_D3214(void);

void fn_1_45BA4(s32 value) {
    fn_1_D3214();
    lbl_1_bss_384C8 = value & (value >> 31);
}
/* fzgx:end fn_1_45BA4 */

/* fzgx:begin fn_1_45BE0 */
extern u32 lbl_1_bss_384C8;

void fn_1_45BE0(void) {
    lbl_1_bss_384C8 = 0;
}
/* fzgx:end fn_1_45BE0 */

/* fzgx:begin fn_1_45D78 */
#include "rel/main_rel/load.h"

typedef struct {
    u32 unk_0;
    u32 unk_4;
    u32 unk_8;
    u32 unk_C;
} LoadEntry;

int fn_1_45D78(u32 start, u32 size, u32 *out_end, s32 mode) {
    u32 limit;
    LoadEntry *entry;
    u32 i;
    u32 total;
    u32 end;

    limit = start + size;
    entry = (LoadEntry *)&lbl_1_bss_384D8;
    total = -1;
    for (i = 0; i < 0x400; i++, entry++) {
        if (entry->unk_0 != 0) {
            if (entry->unk_8 < limit) {
                end = entry->unk_8 + entry->unk_C;
                if (end > start) {
                    if (mode == 0 && (entry->unk_0 & 0x10000000) != 0) {
                        if (out_end != 0) {
                            *out_end = end;
                        }
                        return 0;
                    }
                    entry->unk_0 = 0;
                    if (total == (u32)-1) {
                        total = 0;
                    }
                    total += entry->unk_C;
                }
            }
        }
    }
    return total;
}
/* fzgx:end fn_1_45D78 */

/* fzgx:begin fn_1_465D0 */
extern s32 lbl_1_bss_384C0[];
extern s32 fn_8000700C(s32 arg0);
extern void fn_1_3BDC(s32 arg0);

s32 fn_1_465D0(s32 arg0, s32 arg1) {
    s32 *base = lbl_1_bss_384C0;
    s32 result;
    s32 index;
    s32 *entry;
    s32 next;

    if (base[0x161A] != 0) {
        switch (arg1) {
        case 3:
        case 1:
        case 5:
            arg1++;
            break;
        }
    }

    result = fn_8000700C(arg0);
    if (result < 0) {
        return -1;
    }

    if (base[0] == base[1]) {
        fn_1_3BDC(2);
    }

    index = base[0];
    next = index + 1;
    if (next >= 0x200) {
        next = 0;
    }

    if (base[1] == next) {
        return -1;
    }

    entry = base + 0x1006;
    entry += index * 3;
    base[0] = next;
    entry[0] = arg1;
    entry[1] = result;
    return index;
}
/* fzgx:end fn_1_465D0 */

/* fzgx:begin fn_1_467F4 */
extern void fn_8000659C(void);
extern u32 lbl_1_bss_384C0;
extern u32 lbl_1_bss_384C4;

u32 fn_1_467F4(void) {
    u32 value;

    fn_8000659C();
    value = lbl_1_bss_384C4;
    return lbl_1_bss_384C0 != value;
}
/* fzgx:end fn_1_467F4 */

/* fzgx:begin fn_1_469BC */
extern u32 lbl_1_bss_384C0[];

void fn_1_469BC(void) {
    u32 *base;
    u32 *entry;
    int i;

    base = lbl_1_bss_384C0;
    entry = base + 6;
    for (i = 0; i < 0x400;) {
        if (entry[0] == ((u32)1 << 31)) {
            entry[0] = 0;
            base[0x1606] = entry[2];
        }
        i++;
        entry += 4;
    }
    base[1] = base[0];
}
/* fzgx:end fn_1_469BC */

/* fzgx:begin fn_1_46A60 */
extern void fn_1_46A8C(u32 value);
extern u32 lbl_1_bss_3DCDC;

void fn_1_46A60(void) {
    fn_1_46A8C(lbl_1_bss_3DCDC);
}
/* fzgx:end fn_1_46A60 */

/* fzgx:begin fn_1_46C60 */
extern u32 lbl_1_bss_3DCD8;

u32 fn_1_46C60(void) {
    return lbl_1_bss_3DCD8;
}
/* fzgx:end fn_1_46C60 */

/* fzgx:begin fn_1_46EA8 */
extern u32 lbl_1_bss_3DD28[179];

void fn_1_46EA8(u32 value) {
    lbl_1_bss_3DD28[0] = value;
}
/* fzgx:end fn_1_46EA8 */

/* fzgx:begin fn_1_46EB4 */
extern u32 lbl_1_bss_3DCDC;

typedef struct {
    u32 unk_0;
    u8 pad_4[0x8];
} Obj_1_bss_3DFF4;

extern Obj_1_bss_3DFF4 lbl_1_bss_3DFF4;

void fn_1_46EB4(u32 value, u32 value2) {
    lbl_1_bss_3DCDC = value;
    lbl_1_bss_3DFF4.unk_0 = value2;
}
/* fzgx:end fn_1_46EB4 */

/* fzgx:begin fn_1_46EC8 */
extern u32 lbl_1_bss_3DCDC[2];

u32 fn_1_46EC8(void) {
    return lbl_1_bss_3DCDC[0];
}
/* fzgx:end fn_1_46EC8 */

/* fzgx:begin fn_1_46ED8 */
#include "rel/main_rel/load.h"

u32 fn_1_46ED8(void) {
    return lbl_1_bss_3DFF4.unk_0;
}
/* fzgx:end fn_1_46ED8 */

/* fzgx:begin fn_1_47184 */
#include "rel/main_rel/load.h"

u32 fn_1_47184(void) {
    return lbl_1_bss_3DFF4.unk_0 - lbl_1_bss_3DCDC;
}
/* fzgx:end fn_1_47184 */

/* fzgx:begin fn_1_479B0 */
typedef struct {
    u32 unk_0;
    u8 pad_4[8];
    u32 unk_C;
    u8 pad_10[8];
    u32 unk_18;
} Obj_1_data_6CA0;

extern Obj_1_data_6CA0 lbl_1_data_6CA0;
extern void fn_1_47EE4(s32);
extern void fn_1_485C8(s32);

void fn_1_479B0(void) {
    fn_1_47EE4(0);
    fn_1_485C8(0);
    lbl_1_data_6CA0.unk_0 = -1;
    lbl_1_data_6CA0.unk_C = -1;
    lbl_1_data_6CA0.unk_18 = -1;
}
/* fzgx:end fn_1_479B0 */

/* fzgx:begin fn_1_47A60 */
typedef struct {
    s32 value;
    s32 unk04;
} Fn147A60Entry;

extern Fn147A60Entry lbl_1_data_67F0[240];
extern void fn_1_48140(s32 value);
extern void fn_1_4DDC0(void);
extern void fn_1_4F724(void);

void fn_1_47A60(s16 index) {
    Fn147A60Entry *entry;
    s32 i;

    entry = &lbl_1_data_67F0[index * 10];
    for (i = 0; i < 10; i++, entry++) {
        if (entry->value != -1) {
            fn_1_48140(entry->value);
        } else {
            fn_1_4DDC0();
            fn_1_4F724();
            break;
        }
    }
}
/* fzgx:end fn_1_47A60 */

/* fzgx:begin fn_1_485E8 */
extern s16 *lbl_1_data_19FC4[188];
extern void **lbl_1_data_19098[188];
extern u8 lbl_1_bss_3E024[52];
extern char lbl_1_data_1A3AC[5];

extern void fn_80083DB0(void *arg0, void *arg1);

void *fn_1_485E8(s32 index, s32 value) {
    s32 i = 0;

    while (lbl_1_data_19FC4[index][i] != -1) {
        if (lbl_1_data_19FC4[index][i] == value) {
            fn_80083DB0(lbl_1_bss_3E024, lbl_1_data_19098[index][i]);
            return lbl_1_bss_3E024;
        }
        i++;
    }

    fn_80083DB0(lbl_1_bss_3E024, lbl_1_data_1A3AC);
    return lbl_1_bss_3E024;
}
/* fzgx:end fn_1_485E8 */
