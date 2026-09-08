#include "types.h"

/* fzgx:begin fn_1_12A2D0 */
#include "rel/main_rel/archive.h"

extern void fn_8000C49C(char *arg0, s32 arg1, ...);

void fn_1_12A2D0(s32 value) {
    s32 *entry;

    entry = (s32 *)((u8 *)&lbl_1_bss_897AC + value * 0x6c);
    if (*entry == 1) {
        lbl_1_bss_897A0 = value;
    } else {
        fn_8000C49C((char *)lbl_1_data_40608, 0x5d, lbl_1_data_40614);
    }
}
/* fzgx:end fn_1_12A2D0 */

/* fzgx:begin fn_1_12A32C */
extern void fn_1_12A350(void *arg0, s32 arg1);

void fn_1_12A32C(void *arg0) {
    fn_1_12A350(arg0, 0);
}
/* fzgx:end fn_1_12A32C */

/* fzgx:begin fn_1_12A6D8 */
#include "rel/main_rel/archive.h"

extern s32 fn_8006A480(void *arg0, void *arg1, void *arg2);
extern s32 fn_8006A998(void *arg0);

s32 fn_1_12A6D8(void *arg0) {
    u8 local[0x10];
    if (fn_8006A480((u8 *)&lbl_1_bss_897AC + lbl_1_bss_897A0 * 0x6c + 0x4c, arg0, local) != 0) {
        return fn_8006A998(local);
    }
    return 0;
}
/* fzgx:end fn_1_12A6D8 */

/* fzgx:begin fn_1_12AAC8 noprologue */
#include "types.h"

extern u32 lbl_1_bss_897A0;
extern u8 lbl_1_data_40608[0xA];
typedef struct {
    u32 unk_0;  // 1 loads, 0 stores
    u8 pad_4[0x64];
    u32 unk_68;  // 2 loads, 0 stores
    u8 pad_6C[0x1AC8];
} Obj_1_bss_897AC;
extern Obj_1_bss_897AC lbl_1_bss_897AC;
extern u8 lbl_1_data_4076C[0x28];

extern s32 fn_8006A480(void *arg0, void *arg1, void *arg2);
extern void fn_8006A9AC(void *arg0);
extern void fn_8000C49C(u8 *arg0, s32 arg1, u8 *arg2, ...);

void fn_1_12AAC8(void *arg0) {
    u8 local_8[0x18];
    u8 *obj;

    obj = (u8 *)&lbl_1_bss_897AC + lbl_1_bss_897A0 * 0x6C + 0x4C;
    if (fn_8006A480(obj, arg0, local_8) == 0) {
        fn_8000C49C(lbl_1_data_40608, 0x1BE, lbl_1_data_4076C);
    }
    fn_8006A9AC(local_8);
}
/* fzgx:end fn_1_12AAC8 */

/* fzgx:begin fn_1_12ABB4 */
extern s32 lbl_1_bss_897A4;
extern u32 lbl_801A6410;
extern char lbl_1_data_40608[10];
extern void fn_1_46B4(void *arg0, void *arg1, char *arg2, s32 arg3);

void fn_1_12ABB4(void *arg0) {
    if (lbl_1_bss_897A4 == 0) {
        fn_1_46B4((void *)lbl_801A6410, arg0, lbl_1_data_40608, 0x1e0);
    }
}
/* fzgx:end fn_1_12ABB4 */

/* fzgx:begin fn_1_12AC00 */
extern void fn_1_12AC28(void *arg0, s32 arg1, s32 arg2);

void fn_1_12AC00(void *arg0) {
    fn_1_12AC28(arg0, 0, 0);
}
/* fzgx:end fn_1_12AC00 */

/* fzgx:begin fn_1_12ADA0 */
extern void fn_1_12ADC8(void *arg0, void *arg1, s32 arg2, s32 arg3);

void fn_1_12ADA0(void *arg0, void *arg1) {
    fn_1_12ADC8(arg0, arg1, 0, 0);
}
/* fzgx:end fn_1_12ADA0 */

/* fzgx:begin fn_1_12AF50 */
extern u32 lbl_1_data_40600;

u32 fn_1_12AF50(u32 value) {
    u32 old_value = lbl_1_data_40600;
    lbl_1_data_40600 = value;
    return old_value;
}
/* fzgx:end fn_1_12AF50 */

/* fzgx:begin fn_1_12AF64 */
extern u32 lbl_1_data_40600;

u32 fn_1_12AF64(void) {
    u32 value;

    value = lbl_1_data_40600;
    lbl_1_data_40600 = -1;
    return value;
}
/* fzgx:end fn_1_12AF64 */

/* fzgx:begin fn_1_12B3BC */
typedef struct {
    u8 pad_0[0x4];
    void *unk_4;
    void *unk_8;
    u8 pad_C[0x10];
    void *unk_1C;
} FnObj;

extern FnObj lbl_1_bss_8B3A0;
extern void fn_1_12BB18(void *arg0, void *arg1, void *arg2, void *arg3, void *arg4);

void fn_1_12B3BC(FnObj *arg0, void *arg1, void *arg2, void *arg3,
                 void *arg4, void *arg5, void *arg6) {
    fn_1_12BB18(arg0->unk_4, arg0->unk_8, &lbl_1_bss_8B3A0,
                 arg0->unk_1C, arg6);
}
/* fzgx:end fn_1_12B3BC */

/* fzgx:begin fn_1_12C000 */
extern void fn_1_8D168(void *arg);

void fn_1_12C000(void *arg, s16 count) {
    u8 *p;
    s16 i;

    p = (u8 *)arg;
    i = 0;
    while (i < count) {
        fn_1_8D168(p);
        p += 100;
        i++;
    }
}
/* fzgx:end fn_1_12C000 */

/* fzgx:begin fn_1_12C060 */
typedef struct FnData FnData;

struct FnData {
    u16 unk0;
    u32 unk4;
    u8 unk8;
    u8 pad9[3];
    u8 (*unkC)(FnData *, u32);
    u32 unk10;
    s32 (*unk14)(FnData *, u32);
    u32 unk18;
};

s32 fn_1_12C060(FnData *arg) {
    if (arg->unk8 == 0) {
        return 0;
    }
    if (arg->unkC(arg, arg->unk10) == 1) {
        arg->unk8 = 0;
        arg->unk4 = 0;
        return arg->unk14(arg, arg->unk18);
    }
    arg->unk4++;
    return 0;
}
/* fzgx:end fn_1_12C060 */

/* fzgx:begin fn_1_12C0EC */
typedef struct {
    u16 unk0;
    u32 unk4;
    u8 unk8;
    u8 pad9[3];
    u32 unkC;
    u32 unk10;
    u32 unk14;
    u32 unk18;
} FnData;

void fn_1_12C0EC(FnData *arg) {
    arg->unk18 = 0;
    arg->unk14 = 0;
    arg->unk10 = 0;
    arg->unkC = 0;
    arg->unk8 = 0;
    arg->unk4 = 0;
    arg->unk0 = 0;
}
/* fzgx:end fn_1_12C0EC */

/* fzgx:begin fn_1_12C47C */
#include "rel/main_rel/archive.h"

extern void fn_1_14DB48(void *arg0, void *arg1, u32 arg2);

void fn_1_12C47C(void *arg0, void *arg1, s32 arg2) {
    void *entry;
    void *obj;
    u32 mask;
    s16 i;

    if ((lbl_1_bss_8B3A0.unk_94 & 0x02000000) != 0) {
        return;
    }
    entry = arg1;
    obj = arg0;
    i = 0;
    mask = 1 << arg2;
    while (i < 4) {
        if (((*(u32 *)entry & ((u32)1 << 31)) != 0) &&
            ((*(u32 *)entry & ((u32)1 << 30)) != 0)) {
            fn_1_14DB48(entry, obj, mask);
        }
        entry = (u8 *)entry + 0x81c0;
        obj = (u8 *)obj + 0xa20;
        i++;
    }
}
/* fzgx:end fn_1_12C47C */

/* fzgx:begin fn_1_12C6BC */
extern void fn_1_14DBCC(void *arg);

void fn_1_12C6BC(void *arg) {
    u8 *p;
    s16 i;

    p = (u8 *)arg;
    i = 0;
    while (i < 9) {
        fn_1_14DBCC(p);
        p += 0xa20;
        i++;
    }
}
/* fzgx:end fn_1_12C6BC */

/* fzgx:begin fn_1_12D354 */
extern char lbl_1_data_40798[];
extern u32 lbl_1_bss_8B56C[24];
extern void fn_8000C49C(char *format, ...);

void fn_1_12D354(void *arg0, void *arg1, void *arg2) {
    char *base = lbl_1_data_40798;
    u32 i;

    if (arg2 == 0) {
        fn_8000C49C(base + 0x358, 0x388, base + 0x368);
    }

    for (i = 0; i < 12; i++) {
        if (lbl_1_bss_8B56C[i * 2] == 0) {
            lbl_1_bss_8B56C[i * 2] = (u32)arg2;
            lbl_1_bss_8B56C[i * 2 + 1] = 0;
            return;
        }
    }

    fn_8000C49C(base + 0x358, 0x394, base + 0x390);
}
/* fzgx:end fn_1_12D354 */

/* fzgx:begin fn_1_12E424 */
extern u32 lbl_1_bss_8B3A0[83];
extern u32 fn_1_95120(void *arg);

u8 fn_1_12E424(void *arg0, u8 *arg1) {
    if (lbl_1_bss_8B3A0[37] & (1u << 31)) {
        return 1;
    }
    return (u8)fn_1_95120(arg1 + 0x148);
}
/* fzgx:end fn_1_12E424 */

/* fzgx:begin fn_1_12EF24 */
struct Table {
    s16 values[66];
};

extern const struct Table lbl_1_rodata_8230;

s16 fn_1_12EF24(s16 row, s16 column) {
    struct Table table = lbl_1_rodata_8230;
    return *(s16 *)((u8 *)table.values + row * 12 + column * 2);
}
/* fzgx:end fn_1_12EF24 */

/* fzgx:begin fn_1_12F10C */
extern u32 lbl_1_bss_8B5EC;

void fn_1_12F10C(u32 value) {
    lbl_1_bss_8B5EC = value;
}
/* fzgx:end fn_1_12F10C */

/* fzgx:begin fn_1_12F118 */
extern u32 lbl_1_bss_8B5EC;

u32 fn_1_12F118(void) {
    return lbl_1_bss_8B5EC;
}
/* fzgx:end fn_1_12F118 */

/* fzgx:begin fn_1_12F128 */
extern u32 lbl_801A66A0;
extern u32 lbl_1_bss_8B3A0[83];

void fn_1_12F128(s16 index) {
    lbl_1_bss_8B3A0[index + 56] = lbl_801A66A0;
}
/* fzgx:end fn_1_12F128 */

/* fzgx:begin fn_1_12F150 */
extern u32 lbl_801A66A0;
extern u32 lbl_1_bss_8B3A0[83];

void fn_1_12F150(s16 index, u32 value_100, u32 value_110) {
    lbl_1_bss_8B3A0[index + 60] = lbl_801A66A0;
    lbl_1_bss_8B3A0[index + 64] = value_100;
    lbl_1_bss_8B3A0[index + 68] = value_110;
}
/* fzgx:end fn_1_12F150 */

/* fzgx:begin fn_1_12F17C */
#include "rel/main_rel/archive.h"

extern u32 lbl_801A66A0;

void fn_1_12F17C(void) {
    lbl_1_bss_8B3A0.unk_120 = lbl_801A66A0;
}
/* fzgx:end fn_1_12F17C */

/* fzgx:begin fn_1_12F194 */
extern u32 lbl_1_bss_8B3A0[83];

void fn_1_12F194(void) {
    lbl_1_bss_8B3A0[72] = 0;
    lbl_1_bss_8B3A0[60] = 0;
    lbl_1_bss_8B3A0[64] = 0;
    lbl_1_bss_8B3A0[68] = 0;
    lbl_1_bss_8B3A0[56] = 0;
    lbl_1_bss_8B3A0[61] = 0;
    lbl_1_bss_8B3A0[65] = 0;
    lbl_1_bss_8B3A0[69] = 0;
    lbl_1_bss_8B3A0[57] = 0;
    lbl_1_bss_8B3A0[62] = 0;
    lbl_1_bss_8B3A0[66] = 0;
    lbl_1_bss_8B3A0[70] = 0;
    lbl_1_bss_8B3A0[58] = 0;
    lbl_1_bss_8B3A0[63] = 0;
    lbl_1_bss_8B3A0[67] = 0;
    lbl_1_bss_8B3A0[71] = 0;
    lbl_1_bss_8B3A0[59] = 0;
}
/* fzgx:end fn_1_12F194 */

/* fzgx:begin fn_1_12F1E8 */
typedef struct {
    u8 pad_0[0x94];
    u32 unk_94;
    u8 pad_98[0x8C];
    s32 unk_124;
    u8 unk_128;
} FnState;

extern FnState lbl_1_bss_8B3A0;

void fn_1_12F1E8(s32 value) {
    if ((lbl_1_bss_8B3A0.unk_94 & 0x40000000) == 0) {
        return;
    }

    if (value == 0) {
        lbl_1_bss_8B3A0.unk_124 = -1;
    } else {
        lbl_1_bss_8B3A0.unk_124 = value;
    }
    lbl_1_bss_8B3A0.unk_128 = 0;
}
/* fzgx:end fn_1_12F1E8 */

/* fzgx:begin fn_1_12F228 */
#include "rel/main_rel/archive.h"

u32 fn_1_12F228(void) {
    if ((lbl_1_bss_8B3A0.unk_94 & 0x40000000) == 0) {
        return 0;
    }
    return lbl_1_bss_8B3A0.unk_124 == -1;
}
/* fzgx:end fn_1_12F228 */

/* fzgx:begin fn_1_12F258 */
typedef struct {
    u8 pad_0[0x94];
    u32 unk_94;
    u8 pad_98[0x8C];
    s32 unk_124;
} Obj_1_bss_8B3A0;

extern Obj_1_bss_8B3A0 lbl_1_bss_8B3A0;

s32 fn_1_12F258(void) {
    s32 value = lbl_1_bss_8B3A0.unk_124;

    if ((lbl_1_bss_8B3A0.unk_94 & 0x40000000) == 0) {
        return -1;
    }
    if (value < 0) {
        return -1;
    }
    return value;
}
/* fzgx:end fn_1_12F258 */

/* fzgx:begin fn_1_12F28C */
extern void fn_1_7B2E8(u8 value);
extern void fn_1_435C(void *value);
extern char lbl_1_data_40BC0[19];
extern void fn_1_13C1A0(void);
extern void *fn_1_3F8C(void *table, void (*callback)(void), void *arg1, void *arg2);

void fn_1_12F28C(void *arg0, void **result, void *arg2, u8 *arg3) {
    fn_1_7B2E8(*arg3);
    fn_1_435C(arg0);
    *result = fn_1_3F8C(lbl_1_data_40BC0, fn_1_13C1A0, arg3, arg2);
}
/* fzgx:end fn_1_12F28C */

/* fzgx:begin fn_1_12F308 */
// fn_1_12F308: empty in retail (single blr).
void fn_1_12F308(void) {
}
/* fzgx:end fn_1_12F308 */

/* fzgx:begin fn_1_12F30C */
typedef struct Fn1_12F30CState {
    s32 value;
} Fn1_12F30CState;

extern void fn_1_435C(void *arg0, Fn1_12F30CState *state);
extern void fn_1_426C(s32 value);
extern void fn_1_7B4C0(void);

void fn_1_12F30C(void *arg0, Fn1_12F30CState *state) {
    if (state->value != -1) {
        fn_1_435C(arg0, state);
        fn_1_426C(state->value);
        state->value = -1;
    }
    fn_1_7B4C0();
}
/* fzgx:end fn_1_12F30C */

/* fzgx:begin fn_1_130F98 */
extern s32 lbl_1_bss_8CA44[10];
extern s32 lbl_1_bss_8CA40;
extern void (*lbl_1_data_40D50[8])(void);

void fn_1_130F98(void) {
    if (lbl_1_bss_8CA44[0] >= 0) {
        s32 value = lbl_1_bss_8CA44[0];

        lbl_1_bss_8CA44[0] = -1;
        lbl_1_bss_8CA40 = value;
    }

    if (lbl_1_bss_8CA40 >= 0) {
        lbl_1_data_40D50[lbl_1_bss_8CA40]();
    }
}
/* fzgx:end fn_1_130F98 */

/* fzgx:begin fn_1_131000 */
// fn_1_131000: empty in retail (single blr).
void fn_1_131000(void) {
}
/* fzgx:end fn_1_131000 */

/* fzgx:begin fn_1_131004 */
#include "rel/main_rel/archive.h"

extern void fn_1_435C();
extern void fn_1_426C();

struct Fn131004State {
    u8 pad_00[0x1C];
    u32 unk_1C;
    u32 unk_20;
    u32 unk_24;
    u32 unk_28;
};

void fn_1_131004(void) {
    struct Fn131004State *state = (struct Fn131004State *)&lbl_1_bss_8CA40;

    fn_1_435C(state->unk_1C);
    fn_1_426C(state->unk_24);
    fn_1_435C(state->unk_20);
    fn_1_426C(state->unk_28);
}
/* fzgx:end fn_1_131004 */

/* fzgx:begin fn_1_131050 */
extern u32 lbl_1_bss_8CA40;

void fn_1_131050(u32 value, u32 state) {
    u32* data = &lbl_1_bss_8CA40;
    s32 previous;

    if (value == 0) {
        return;
    }

    previous = *(s32*)data;
    data[3] = value;
    data[2] = state;
    data[11] = state;

    if (previous == -1) {
        data[1] = 2;
    }
}
/* fzgx:end fn_1_131050 */

/* fzgx:begin fn_1_1310E0 */
extern u32 lbl_1_bss_8CA40;

s32 fn_1_1310E0(void) {
    return lbl_1_bss_8CA40 != -1;
}
/* fzgx:end fn_1_1310E0 */

/* fzgx:begin fn_1_1310FC */
extern u32 lbl_1_bss_8CA40;
extern void fn_1_C2454(void);

void fn_1_1310FC(void) {
    u8* ptr = (u8*)&lbl_1_bss_8CA40;
    
    if (*(s32*)(ptr + 0x8) == -1) {
        *(u32*)(ptr + 0x0) = (u32)-1;
    } else {
        fn_1_C2454();
        *(u32*)(ptr + 0x4) = 1;
        *(u8*)(ptr + 0x30) = 0;
    }
}
/* fzgx:end fn_1_1310FC */

/* fzgx:begin fn_1_131154 */
extern s32 fn_1_C24A4(void);
extern u8 fn_1_B7C00(void);
extern s32 lbl_1_bss_8CA40;

void fn_1_131154(void) {
    if (fn_1_C24A4() == 0 && fn_1_B7C00() == 0) {
        lbl_1_bss_8CA40 = -1;
    }
}
/* fzgx:end fn_1_131154 */
