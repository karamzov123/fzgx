#include "types.h"

/* fzgx:begin fn_1_35124 */
extern u32 OSGetTick(void);
extern void fn_1_A0978(void);

void fn_1_35124(void) {
    OSGetTick();
    fn_1_A0978();
    OSGetTick();
}
/* fzgx:end fn_1_35124 */

/* fzgx:begin fn_1_3514C */
extern void fn_1_35174(void);

void fn_1_3514C(void* arg0, int arg1) {
    if (arg1 == 1) {
        fn_1_35174();
    }
}
/* fzgx:end fn_1_3514C */

/* fzgx:begin fn_1_35174 */
// fn_1_35174: empty in retail (single blr).
void fn_1_35174(void) {
}
/* fzgx:end fn_1_35174 */

/* fzgx:begin fn_1_3E5D0 */
extern u32 lbl_1_bss_25B88[4];
extern u32 lbl_1_bss_25CA4[11];
extern void fn_1_435C(u32 value);
extern void fn_1_4310(s32 value);
extern void fn_1_4811C(s32 value);
extern void fn_1_A8F78(void);

void fn_1_3E5D0(void) {
    fn_1_435C(lbl_1_bss_25B88[0]);
    fn_1_4310(0);
    fn_1_435C(lbl_1_bss_25CA4[0]);
    fn_1_4310(0);
    fn_1_4811C(0x8a);
    fn_1_A8F78();
}
/* fzgx:end fn_1_3E5D0 */

/* fzgx:begin fn_1_3E628 */
// fn_1_3E628: empty in retail (single blr).
void fn_1_3E628(void) {
}
/* fzgx:end fn_1_3E628 */

/* fzgx:begin fn_1_3E66C */
// fn_1_3E66C: empty in retail (single blr).
void fn_1_3E66C(void) {
}
/* fzgx:end fn_1_3E66C */

/* fzgx:begin fn_1_3EF14 */
extern u8 lbl_1_bss_3C12[10];
extern s16 lbl_1_bss_960;
extern u8 lbl_1_bss_3C30[5308];

extern void fn_80008BA8(void *arg1, u8 *arg2, u32 arg3);
extern void fn_80008BEC(void *arg1, u32 arg2, u32 arg3);

void fn_1_3EF14(void *arg1) {
    if (lbl_1_bss_3C12[0] != 0 ||
        ((u16)(lbl_1_bss_960 - 13) <= 1) ||
        lbl_1_bss_960 == 10 ||
        lbl_1_bss_960 == 16) {
        fn_80008BA8(arg1, lbl_1_bss_3C30, 0x14b8);
    } else {
        fn_80008BEC(arg1, 0, 0x14b8);
    }
}
/* fzgx:end fn_1_3EF14 */

/* fzgx:begin fn_1_3EF8C */
extern u8 lbl_1_bss_3C12[10];
extern u8 lbl_1_data_5B7C;

void fn_1_3EF8C(u8 value) {
    if (lbl_1_bss_3C12[0] != 0) {
        return;
    }
    lbl_1_data_5B7C = value;
}
/* fzgx:end fn_1_3EF8C */

/* fzgx:begin fn_1_3EFF0 */
extern u32 lbl_1_bss_25E74[7];
extern u8 lbl_1_bss_3C10;
extern u8 lbl_1_bss_3C11;
extern u8 lbl_1_bss_26C58;
extern u32 lbl_1_bss_26C64;

void fn_1_3EFF0(u32 arg1, u8 arg2) {
    lbl_1_bss_25E74[0] = arg1;
    lbl_1_bss_3C10 = arg2;
    lbl_1_bss_3C11 = 1;
    lbl_1_bss_26C58 = 0;
    lbl_1_bss_26C64 = 0;
}
/* fzgx:end fn_1_3EFF0 */

/* fzgx:begin fn_1_3F038 */
extern u8 lbl_1_bss_26C58;
extern u8 lbl_1_bss_3C11;
extern void fn_1_36330(void);
extern void fn_1_36B10(void);
extern void fn_1_3C98(void);
extern u32 lbl_1_bss_3C30[1327];
extern void fn_1_37AA8(void);
extern void fn_1_37E98(void);

int fn_1_3F038(void) {
    if (lbl_1_bss_26C58 != 0) {
        return 1;
    }
    if (lbl_1_bss_3C11 != 0) {
        fn_1_36330();
        lbl_1_bss_3C11 = 0;
    }
    fn_1_36B10();
    fn_1_3C98();
    if ((lbl_1_bss_3C30[0] & 0x00000004) == 0) {
        fn_1_37AA8();
    }
    if (lbl_1_bss_26C58 != 0) {
        fn_1_37E98();
        return 1;
    }
    return 0;
}
/* fzgx:end fn_1_3F038 */

/* fzgx:begin fn_1_3F0D8 */
extern f32 lbl_1_bss_26304[4];
extern u8 lbl_1_bss_26B04[20];
extern u8 lbl_1_bss_26314[2032];

u8 *fn_1_3F0D8(u32 index, f32 *value, u8 *flag) {
    u8 idx = (u8)index;
    *value = lbl_1_bss_26304[idx];
    *flag = lbl_1_bss_26B04[idx];
    return &lbl_1_bss_26314[idx * 0x1fc];
}
/* fzgx:end fn_1_3F0D8 */

/* fzgx:begin fn_1_3F114 */
extern u8 lbl_1_bss_3C12[10];
extern u8 lbl_1_bss_3C30[5308];

u8 fn_1_3F114(void) {
    if (lbl_1_bss_3C12[0] != 0) {
        return lbl_1_bss_3C30[4];
    }
    return 0;
}
/* fzgx:end fn_1_3F114 */

/* fzgx:begin fn_1_3F1D4 */
extern u8 lbl_1_bss_3C30[5308];

s32 fn_1_3F1D4(void) {
    if (lbl_1_bss_3C30[5] == 1) {
        return lbl_1_bss_3C30[0x13f6];
    }
    return -1;
}
/* fzgx:end fn_1_3F1D4 */

/* fzgx:begin fn_1_3F23C */
extern u8 lbl_1_bss_2625C[32];

u8 fn_1_3F23C(u32 index) {
    return lbl_1_bss_2625C[index & 0xff];
}
/* fzgx:end fn_1_3F23C */

/* fzgx:begin fn_1_3F250 */
extern u8 lbl_1_bss_523C[32];

u8 fn_1_3F250(u32 index) {
    return lbl_1_bss_523C[index & 0xff];
}
/* fzgx:end fn_1_3F250 */

/* fzgx:begin fn_1_3F264 */
typedef struct {
    u8 unk[0xb];
    u8 value;
} Entry;

extern Entry *lbl_1_bss_53F8[34];
extern s16 lbl_1_bss_25C48[46];

u8 fn_1_3F264(u32 index) {
    u32 masked = index & 0xff;
    Entry *entry = lbl_1_bss_53F8[masked];

    if (entry != 0) {
        return entry->value;
    }

    return (u8)lbl_1_bss_25C48[masked];
}
/* fzgx:end fn_1_3F264 */

/* fzgx:begin fn_1_3F7E0 */
extern u8 lbl_1_bss_3C30[5308];

u8 fn_1_3F7E0(void) {
    u32 value = *(u32*)lbl_1_bss_3C30;
    if (value & 0x00008000) {
        return lbl_1_bss_3C30[9];
    }
    return lbl_1_bss_3C30[10];
}
/* fzgx:end fn_1_3F7E0 */

/* fzgx:begin fn_1_3F810 */
extern u8 lbl_1_bss_3C30[5308];

void fn_1_3F810(u8 value, u32 input) {
    lbl_1_bss_3C30[0x146C] = value;
    lbl_1_bss_3C30[0x146D] = (u8)((input / 5) >> 1);
}
/* fzgx:end fn_1_3F810 */

/* fzgx:begin fn_1_3F834 */
extern u8 lbl_1_bss_3C30[5308];

void fn_1_3F834(u8 value) {
    lbl_1_bss_3C30[0x1470] = value;
}
/* fzgx:end fn_1_3F834 */

/* fzgx:begin fn_1_3F844 */
extern u8 lbl_1_bss_3C30[5308];

void fn_1_3F844(u16 value) {
    *(u16 *)&lbl_1_bss_3C30[0x146e] = value;
}
/* fzgx:end fn_1_3F844 */

/* fzgx:begin fn_1_3F854 */
extern u8 lbl_1_bss_3C30[5308];

u8 fn_1_3F854(void) {
    return lbl_1_bss_3C30[8];
}
/* fzgx:end fn_1_3F854 */

/* fzgx:begin fn_1_3FC8C */
extern u8 lbl_1_bss_3C30[5308];

// fn_1_3FC8C: Extract bit 6 from the first word of the global buffer.
u32 fn_1_3FC8C(void) {
    u32 value = *(u32*)lbl_1_bss_3C30;
    return value & 0x02000000;
}
/* fzgx:end fn_1_3FC8C */

/* fzgx:begin fn_1_3FC9C */
extern u8 lbl_1_bss_8B3A0[332];

u32 fn_1_3FC9C(void) {
    s16 value = *(s16*)(lbl_1_bss_8B3A0 + 8);
    return (u32)value & 0xff;
}
/* fzgx:end fn_1_3FC9C */

/* fzgx:begin fn_1_40224 */
extern int fn_1_F9FEC(void);
extern int fn_1_FA180(int, int);

int fn_1_40224(void) {
    int result;

    if (fn_1_F9FEC() != 0) {
        return 1;
    }
    if (fn_1_FA180(2, 0) == 0) {
        return 0;
    }
    if (fn_1_FA180(2, 1) == 0) {
        return 0;
    }
    result = fn_1_FA180(2, 2);
    return result != 0;
}
/* fzgx:end fn_1_40224 */

/* fzgx:begin fn_1_40E08 */
typedef struct {
    u32 unk_0;
    u8 pad_4[0xA];
} Obj_1_bss_3C1C;

extern s16 lbl_1_bss_960;
extern Obj_1_bss_3C1C lbl_1_bss_3C1C;
extern u32 lbl_1_bss_38210;
extern u32 lbl_1_bss_38214;
extern u8 lbl_1_data_662C[13];
extern u32 lbl_801A63C0;
extern void fn_1_40BE4(void);
extern void fn_1_3F8C(u8 *, void (*)(void), s32, s32);

void fn_1_40E08(u32 arg0) {
    u32 value;
    u32 half;
    u32 max;
    u32 remainder;

    if (lbl_1_bss_960 == 0x10) {
        lbl_1_bss_3C1C.unk_0 = -1;
    }

    lbl_1_bss_38210 = arg0;
    if (arg0 < 5) {
        lbl_1_bss_38210 = 5;
    }

    value = lbl_801A63C0 * 0x676A4B6B + 0x33CB;
    lbl_801A63C0 = value;

    half = lbl_1_bss_38210 >> 1;
    max = lbl_1_bss_38210 - 5;
    remainder = ((value >> 16) & 0x7FFF) % half;
    lbl_1_bss_38214 = half + remainder;
    if (lbl_1_bss_38214 > max) {
        lbl_1_bss_38214 = max;
    }

    fn_1_3F8C(lbl_1_data_662C, fn_1_40BE4, 0, 1);
}
/* fzgx:end fn_1_40E08 */

/* fzgx:begin fn_1_40ED4 */
extern u8 lbl_1_bss_3820C;

u8 fn_1_40ED4(void) {
    return lbl_1_bss_3820C;
}
/* fzgx:end fn_1_40ED4 */

/* fzgx:begin fn_1_40F34 */
extern u32 lbl_1_bss_26B4C;

u32 fn_1_40F34(void) {
    return lbl_1_bss_26B4C;
}
/* fzgx:end fn_1_40F34 */

/* fzgx:begin fn_1_40F44 */
extern u8 lbl_1_bss_25E90[8];

u8 fn_1_40F44(void) {
    return lbl_1_bss_25E90[0];
}
/* fzgx:end fn_1_40F44 */

/* fzgx:begin fn_1_40F54 */
extern void fn_1_48B0(void *arg0, int arg1);

void fn_1_40F54(void *arg0) {
    fn_1_48B0(arg0, 0x20);
}
/* fzgx:end fn_1_40F54 */

/* fzgx:begin fn_1_40F78 */
#include "rel/main_rel/game.h"

extern u32 lbl_801A6CE0;
extern u32 lbl_1_bss_38450;
extern u32 lbl_1_bss_38454;
extern u32 fn_80070DE0(void (*)(void));
extern void fn_1_40F54(void);
extern u32 fn_1_D3884(u8 *);
extern u32 fn_1_D358C(u8 *, u32);

void fn_1_40F78(void) {
    u32 resource;
    u32 data;

    if ((lbl_801A6CE0 & 1) == 0) {
        lbl_1_bss_38450 = 0;
        lbl_1_bss_38454 = 0;
    } else {
        resource = fn_80070DE0(fn_1_40F54);
        data = fn_1_D3884(lbl_1_data_66C0);
        lbl_1_bss_38450 = data;
        lbl_1_bss_38454 = fn_1_D358C(lbl_1_data_66D0, data);
        fn_80070DE0((void (*)(void))resource);
    }
}
/* fzgx:end fn_1_40F78 */

/* fzgx:begin fn_1_4100C */
extern char lbl_1_data_66E0[16];
extern char lbl_1_data_66F0[16];
extern void fn_1_465D0(char *arg0, u32 arg1);

void fn_1_4100C(void) {
    fn_1_465D0(lbl_1_data_66E0, 1);
    fn_1_465D0(lbl_1_data_66F0, 1);
}
/* fzgx:end fn_1_4100C */

/* fzgx:begin fn_1_41048 */
#include "rel/main_rel/game.h"

extern void *fn_1_D3884(u8 *arg0);
extern void *fn_1_D358C(u8 *arg0, void *arg1);

void fn_1_41048(void) {
    void *value;

    if (lbl_1_bss_38458 == 0) {
        value = fn_1_D3884(lbl_1_data_66E0);
        lbl_1_bss_3845C = (u32)value;
        lbl_1_bss_38458 = (Obj_1_bss_38458_Target *)fn_1_D358C(lbl_1_data_66F0, value);
    }
}
/* fzgx:end fn_1_41048 */

/* fzgx:begin fn_1_410A0 */
#include "rel/main_rel/game.h"

extern void fn_800711A8(Obj_1_bss_38458_Target *target);
extern void fn_80071718(u32 value);

void fn_1_410A0(void) {
    if (lbl_1_bss_38458 != 0) {
        fn_800711A8(lbl_1_bss_38458);
        lbl_1_bss_38458 = 0;
    }

    if (lbl_1_bss_3845C != 0) {
        fn_80071718(lbl_1_bss_3845C);
        lbl_1_bss_3845C = 0;
    }
}
/* fzgx:end fn_1_410A0 */

/* fzgx:begin fn_1_41104 */
extern char *lbl_1_data_66A0[8];
extern void fn_1_41134(u32 index, char *value);

void fn_1_41104(u32 index) {
    fn_1_41134(index, lbl_1_data_66A0[index]);
}
/* fzgx:end fn_1_41104 */

/* fzgx:begin fn_1_41134 */
extern char lbl_1_data_6700[7];
extern char lbl_1_data_6708[7];
extern void fn_8008069C(char *buffer, char *format, ...);
extern void fn_1_465D0(char *value, u32 flag);

void fn_1_41134(void *unused, char *value) {
    char buffer[128];

    fn_8008069C(buffer, lbl_1_data_6700, value);
    fn_1_465D0(buffer, 1);
    fn_8008069C(buffer, lbl_1_data_6708, value);
    fn_1_465D0(buffer, 1);
}
/* fzgx:end fn_1_41134 */

/* fzgx:begin fn_1_411A4 */
extern u32 lbl_1_data_66A0[8];
extern void fn_1_411D4(u32 index, u32 value);

// fn_1_411A4: Load array element by index and call fn_1_411D4 with both index and value.
void fn_1_411A4(u32 index) {
    fn_1_411D4(index, lbl_1_data_66A0[index]);
}
/* fzgx:end fn_1_411A4 */

/* fzgx:begin fn_1_41328 */
typedef struct RelocData {
    u32 count;
    u32 values[1];
} RelocData;

void fn_1_41328(RelocData *data) {
    u32 *values = data->values;
    u32 count = data->count;
    u32 i = 0;

    if (count & ((u32)1 << 31)) {
        return;
    }

    for (i = 0; i <= count; i++) {
        values[i] += (u32)data;
    }

    data->count |= ((u32)1 << 31);
}
/* fzgx:end fn_1_41328 */

/* fzgx:begin fn_1_41418 */
typedef struct Fn41418Data {
    u32 count;
    char *strings;
} Fn41418Data;

extern u32 strlen(const char *str);

static inline u32 add_lengths(u32 length, u32 address) {
    return length + address;
}

char *fn_1_41418(Fn41418Data *data, u32 index) {
    u32 count = data->count & 0x7fffffff;
    u32 str = (u32)data->strings;

    if (index >= count) {
        return 0;
    }

    while (index != 0) {
        str = add_lengths(strlen((char *)str), str);
        index -= 1;
        str += 1;
    }

    return (char *)str;
}
/* fzgx:end fn_1_41418 */

/* fzgx:begin fn_1_41488 */
typedef struct Fn41488Data {
    u32 count;
    char *strings;
} Fn41488Data;

extern u32 strlen(const char *str);
extern int fn_80083BCC(const char *left, const char *right);

static inline u32 add_lengths(u32 length, u32 address) {
    return length + address;
}

int fn_1_41488(Fn41488Data *data, const char *value) {
    u32 count = data->count & 0x7fffffff;
    u32 str = (u32)data->strings;
    u32 index = 0;

    while (index < count) {
        if (fn_80083BCC(value, (const char *)str) == 0) {
            return index;
        }
        str = add_lengths(strlen((const char *)str), str);
        index += 1;
        str += 1;
    }

    return -1;
}
/* fzgx:end fn_1_41488 */
