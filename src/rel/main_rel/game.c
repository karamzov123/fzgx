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

/* fzgx:begin fn_1_35178 */
#include "rel/main_rel/game.h"

extern int fn_1_3EFA8(void* arg0);
extern u16 lbl_1_bss_26B7A[3];
extern u16 lbl_1_bss_26300;

void fn_1_35178(void* arg0) {
    if (fn_1_3EFA8(arg0) == 0) {
        switch (lbl_1_bss_3C2A) {
        case 0x29:
            if ((lbl_1_bss_3C30.unk_0 & 0x00000800) == 0) {
                if (lbl_1_bss_3C30.unk_5 == 2) {
                    u32 value = 0 - *(u32*)arg0;

                    if (lbl_1_bss_3C30.unk_14A8 != value) {
                        lbl_1_bss_3C30.unk_14A8 = value;
                    }
                    if (*(s16*)&lbl_1_bss_26B7A[0] == -1 ||
                        *(s16*)&lbl_1_bss_26300 == -1) {
                        lbl_1_bss_3C30.unk_5 = 6;
                    }
                }
                *(u32*)arg0 = *(u32*)arg0 - 1;
            }
            break;
        default:
            break;
        }
    }
}
/* fzgx:end fn_1_35178 */

/* fzgx:begin fn_1_36AD0 */
extern u8 lbl_1_bss_5480[132864];

u8* fn_1_36AD0(void) {
    return lbl_1_bss_5480;
}
/* fzgx:end fn_1_36AD0 */

/* fzgx:begin fn_1_36ADC */
extern u8 lbl_1_bss_5480[132864];
extern void fn_80008BEC(void* arg0, int arg1, int arg2);

void fn_1_36ADC(void) {
    fn_80008BEC(lbl_1_bss_5480, 0, 0x20700);
}
/* fzgx:end fn_1_36ADC */

/* fzgx:begin fn_1_3DDAC */
#include "rel/main_rel/game.h"

extern void fn_1_4A00(u32 arg0, u32 arg1, u32 arg2);
extern void fn_1_5370(u32 arg0, u32 arg1);
extern void fn_1_15E27C(void);
extern void fn_1_D0790(void);
extern void fn_1_4811C(u32 arg0);
extern void fn_1_48004(u32 arg0, u32 arg1);
extern void fn_1_435C(u32 arg0);
extern void fn_1_4310(u32 arg0);
extern void fn_1_A1588(u32 arg0, u32 arg1);

void fn_1_3DDAC(void) {
    fn_1_4A00(1, 0x1e, lbl_1_bss_5100);
    fn_1_5370(3, 0);
    fn_1_15E27C();

    lbl_1_bss_26C28 = 0;
    fn_1_D0790();

    lbl_1_bss_3C30.unk_0 |= 0x8;
    lbl_1_bss_6EAB4.unk_0 |= 0x2c;
    fn_1_4811C(0x94);

    fn_1_48004(0x8a, 1);
    fn_1_435C(lbl_1_bss_25CA4.unk_0);
    fn_1_4310(0x20000);
    fn_1_435C(lbl_1_bss_25B88.unk_0);
    fn_1_4310(0x2c3);

    fn_1_A1588(lbl_1_bss_6EAD0.unk_0->unk_0, 0x1e);
}
/* fzgx:end fn_1_3DDAC */

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

/* fzgx:begin fn_1_3E670 */
#include "rel/main_rel/game.h"

extern void fn_1_4A00(u32 arg0, u32 arg1, u32 arg2);
extern void fn_1_D0790(void);

void fn_1_3E670(void) {
    fn_1_4A00(1, 0x1e, lbl_1_bss_5100);
    lbl_1_bss_26C28 = 0;
    fn_1_D0790();
    lbl_1_bss_3C30.unk_0 |= 8;
    lbl_1_bss_6EAB4.unk_0 |= 0x2c;
}
/* fzgx:end fn_1_3E670 */

/* fzgx:begin fn_1_3EA70 */
extern void fn_1_A8F78(void);

void fn_1_3EA70(void) {
    fn_1_A8F78();
}
/* fzgx:end fn_1_3EA70 */

/* fzgx:begin fn_1_3EF08 */
extern u8 lbl_1_bss_262F4;

void fn_1_3EF08(u8 value) {
    lbl_1_bss_262F4 = value;
}
/* fzgx:end fn_1_3EF08 */

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

/* fzgx:begin fn_1_3EFA8 */
typedef struct MainRelState {
    u8 pad_0000[0x12];
    u8 unk_12;
    u8 pad_0013[0x1D];
    s32 unk_30;
    u8 pad_0034[0x14C4];
    s32 unk_14F8;
} MainRelState;

extern MainRelState lbl_1_bss_3C00;
extern s32 lbl_1_bss_26B50;

s32 fn_1_3EFA8(void) {
    s32 result = 0;
    MainRelState *state = &lbl_1_bss_3C00;

    if (state->unk_14F8 != 0 ||
        (state->unk_30 & 2) != 0 ||
        lbl_1_bss_26B50 != 0) {
        if (state->unk_12 != 0) {
            result = 1;
        }
    }
    return result;
}
/* fzgx:end fn_1_3EFA8 */

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

/* fzgx:begin fn_1_3F02C */
extern u32 lbl_1_bss_26C64;

void fn_1_3F02C(u32 arg1) {
    lbl_1_bss_26C64 = arg1;
}
/* fzgx:end fn_1_3F02C */

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

/* fzgx:begin fn_1_3F0C8 */
#include "rel/main_rel/game.h"

s16 fn_1_3F0C8(void) {
    return lbl_1_bss_3C2A;
}
/* fzgx:end fn_1_3F0C8 */

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

/* fzgx:begin fn_1_3F13C */
#include "rel/main_rel/game.h"

u8 fn_1_3F13C(void) {
    if (lbl_1_bss_3C12.unk_0 != 0) {
        return lbl_1_bss_3C30.unk_5;
    }
    return 0xff;
}
/* fzgx:end fn_1_3F13C */

/* fzgx:begin fn_1_3F164 */
typedef struct {
    u8 unk_00[0x12];
    u8 unk_12;
    u8 unk_13[0x1d];
    s32 unk_30;
    u8 unk_34[0x14c4];
    s32 unk_14f8;
} MainState;

extern MainState lbl_1_bss_3C00;
extern s32 lbl_1_bss_26B50;

s32 fn_1_3F164(void) {
    s32 result = 0;
    MainState *state = &lbl_1_bss_3C00;

    if (state->unk_14f8 != 0 ||
        (state->unk_30 & 2) != 0 ||
        lbl_1_bss_26B50 != 0) {
        if (state->unk_12 != 0) {
            result = 1;
        }
    }
    return result;
}
/* fzgx:end fn_1_3F164 */

/* fzgx:begin fn_1_3F1D4 */
extern u8 lbl_1_bss_3C30[5308];

s32 fn_1_3F1D4(void) {
    if (lbl_1_bss_3C30[5] == 1) {
        return lbl_1_bss_3C30[0x13f6];
    }
    return -1;
}
/* fzgx:end fn_1_3F1D4 */

/* fzgx:begin fn_1_3F1F8 */
#include "rel/main_rel/game.h"

extern u32 fn_1_12EF24(u8 arg0, u8 arg1);

u8 fn_1_3F1F8(void) {
    if (lbl_1_bss_3C30.unk_A6 != 0xff) {
        return fn_1_12EF24(lbl_1_bss_3C30.unk_A6, lbl_1_bss_3C30.unk_A7);
    }
    return lbl_1_bss_3C30.unk_6;
}
/* fzgx:end fn_1_3F1F8 */

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

/* fzgx:begin fn_1_3F440 */
typedef struct {
    u8 pad_0[8];
    u8 unk_8;
    u8 pad_9[5];
    u8 unk_E[256];
} Obj_1_bss_3C30;

extern u32 lbl_1_data_62EC;
extern char lbl_1_data_65E8[23];
extern Obj_1_bss_3C30 lbl_1_bss_3C30;

extern void fn_8000C49C(const void *, u32, const char *, ...);

s32 fn_1_3F440(u8 index) {
    if (index > 0x1d) {
        fn_8000C49C(&lbl_1_data_62EC, 0x2654, lbl_1_data_65E8);
    }

    if (lbl_1_bss_3C30.unk_8 > lbl_1_bss_3C30.unk_E[index]) {
        return lbl_1_bss_3C30.unk_E[index];
    }

    return -1;
}
/* fzgx:end fn_1_3F440 */

/* fzgx:begin fn_1_3F75C */
extern void fn_1_451E4(void);
extern void fn_1_D3214(void);
extern void fn_8001AF64(void);
extern s32 fn_1_467F4(void);
extern s32 fn_1_13018(void);

void fn_1_3F75C(void) {
    while (fn_1_467F4() || fn_1_13018()) {
        fn_1_451E4();
        fn_1_D3214();
        fn_8001AF64();
    }
}
/* fzgx:end fn_1_3F75C */

/* fzgx:begin fn_1_3F7A0 */
extern u32 fn_1_46C60(void);

void fn_1_3F7A0(u32* self) {
    self[0] = fn_1_46C60();
}
/* fzgx:end fn_1_3F7A0 */

/* fzgx:begin fn_1_3F7D0 */
extern u32 lbl_1_bss_26C28;

u32 fn_1_3F7D0(void) {
    return lbl_1_bss_26C28;
}
/* fzgx:end fn_1_3F7D0 */

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

/* fzgx:begin fn_1_3F800 */
extern u8 lbl_1_bss_3C30[5308];

void fn_1_3F800(u32 value) {
    *(u32*)(lbl_1_bss_3C30 + 0x1464) = value;
}
/* fzgx:end fn_1_3F800 */

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

/* fzgx:begin fn_1_3F864 */
typedef struct {
    u32 unk_0;
} Obj_1_bss_3C30;

extern Obj_1_bss_3C30 lbl_1_bss_3C30;
extern s16 lbl_1_bss_960;

u8 fn_1_3F864(void) {
    if ((lbl_1_bss_3C30.unk_0 & 0x00100000) != 0 ||
        lbl_1_bss_960 == 0x000c) {
        return 1;
    }
    return 0;
}
/* fzgx:end fn_1_3F864 */

/* fzgx:begin fn_1_3F894 */
extern u8 lbl_1_bss_3C30[5308];
extern u16 lbl_1_bss_53F4;
extern u32 lbl_1_bss_25BA0[1];
extern u32 lbl_1_bss_26C2C[2];

void fn_1_3F894(void) {
    lbl_1_bss_53F4 = lbl_1_bss_3C30[7];
    lbl_1_bss_25BA0[0] = 1;
    lbl_1_bss_26C2C[0] = 1;
}
/* fzgx:end fn_1_3F894 */

/* fzgx:begin fn_1_3F8C0 */
extern u32 lbl_1_bss_262F8[2];

u32 fn_1_3F8C0(void) {
    return lbl_1_bss_262F8[0];
}
/* fzgx:end fn_1_3F8C0 */

/* fzgx:begin fn_1_3F8D0 */
#include "rel/main_rel/game.h"

void fn_1_3F8D0(void) {
    u16 *values = (u16 *)&lbl_1_bss_38204;

    values[0] = 0;
    values[1] = 0;
    values[2] = 0;
    values[3] = 0;
}
/* fzgx:end fn_1_3F8D0 */

/* fzgx:begin fn_1_3FC18 */
extern u16 lbl_1_bss_26C68[35526];

u16 fn_1_3FC18(void) {
    return lbl_1_bss_26C68[0];
}
/* fzgx:end fn_1_3FC18 */

/* fzgx:begin fn_1_3FC28 */
#include "rel/main_rel/game.h"

u32 fn_1_3FC28(void) {
    return lbl_1_bss_3C30.unk_0 & 0x00800000;
}
/* fzgx:end fn_1_3FC28 */

/* fzgx:begin fn_1_3FC38 */
#include "rel/main_rel/game.h"

u8 fn_1_3FC38(void) {
    return lbl_1_bss_3C12.unk_0;
}
/* fzgx:end fn_1_3FC38 */

/* fzgx:begin fn_1_3FC48 */
#include "rel/main_rel/game.h"

u32 fn_1_3FC48(void) {
    return lbl_1_bss_3C30.unk_0 & 0x100;
}
/* fzgx:end fn_1_3FC48 */

/* fzgx:begin fn_1_3FC58 */
#include "rel/main_rel/game.h"

u32 fn_1_3FC58(void) {
    return lbl_1_bss_3C30.unk_0 & 0x01000000;
}
/* fzgx:end fn_1_3FC58 */

/* fzgx:begin fn_1_3FC68 */
extern u32 lbl_1_bss_3C30;

void fn_1_3FC68(s32 enabled) {
    lbl_1_bss_3C30 = enabled != 0
        ? lbl_1_bss_3C30 & ~0x02000000
        : lbl_1_bss_3C30 | 0x02000000;
}
/* fzgx:end fn_1_3FC68 */

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

/* fzgx:begin fn_1_3FDA8 */
#include "rel/main_rel/game.h"

extern s32 ghost_test_record_flag0(u8 value);
extern s32 fn_1_F9D44(u32 value);
extern u8 fn_1_F8C50(u8 value);
extern void *fn_1_435C(void *value);
extern void fn_1_2DB50(void);
extern void fn_1_3F8C(void *arg0, void (*arg1)(void), Obj_1_bss_5138 *arg2, u32 arg3);
extern void fn_1_2D888(void);
extern void fn_1_2D524(void);

void fn_1_3FDA8(void *arg0, void *arg1) {
    s32 condition;
    void *value;

    if (lbl_1_bss_5138.unk_0 != 0) {
        lbl_1_bss_5138.unk_E1 = 0;
    } else {
        if (lbl_1_bss_5138.unk_E3 == 0xff) {
            condition = 0;
        } else if (ghost_test_record_flag0(lbl_1_bss_3C30.unk_6) != 0) {
            condition = 0;
        } else {
            condition = 1;
        }
        if (condition != 0) {
            lbl_1_bss_5138.unk_E1 = 1;
        } else {
            if (lbl_1_bss_5138.unk_E5 == 0xff) {
                condition = 0;
            } else if (fn_1_F9D44(lbl_1_bss_5138.unk_E5 & 0xf) != 0) {
                condition = 0;
            } else {
                condition = 1;
            }
            if (condition != 0) {
                lbl_1_bss_5138.unk_E1 = 2;
            } else {
                if (lbl_1_bss_5138.unk_EB == 0xff) {
                    condition = 0;
                } else if (fn_1_F8C50(lbl_1_bss_5138.unk_EB) != 0) {
                    condition = 0;
                } else {
                    condition = 1;
                }
                if (condition != 0) {
                    lbl_1_bss_5138.unk_E1 = 3;
                } else if (lbl_1_bss_5138.unk_3 + lbl_1_bss_5138.unk_2 + lbl_1_bss_5138.unk_E4 != 0) {
                    lbl_1_bss_5138.unk_E1 = 4;
                } else {
                    // Skip setup when no state transition is selected.
                    goto done;
                }
            }
        }
    }

    value = fn_1_435C(arg0);
    fn_1_3F8C(lbl_1_data_6608, fn_1_2DB50, &lbl_1_bss_5138, 30);
    fn_1_435C(arg1);
    fn_1_3F8C(lbl_1_data_6618, fn_1_2D888, &lbl_1_bss_5138, 30);
    fn_1_435C(value);
    lbl_1_bss_5138.unk_E2 = 1;
    fn_1_2D524();

done:
    return;
}
/* fzgx:end fn_1_3FDA8 */

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

/* fzgx:begin fn_1_40B14 */
extern u8 lbl_1_bss_26B54[8];

u8 *fn_1_40B14(void) {
    return lbl_1_bss_26B54;
}
/* fzgx:end fn_1_40B14 */

/* fzgx:begin fn_1_40BB4 */
#include "rel/main_rel/game.h"

u8 fn_1_40BB4(void) {
    return (s8)lbl_1_bss_5104.unk_0 != -1;
}
/* fzgx:end fn_1_40BB4 */

/* fzgx:begin fn_1_40BD4 */
extern u8 lbl_1_bss_3820C;

void fn_1_40BD4(void) {
    lbl_1_bss_3820C = 0;
}
/* fzgx:end fn_1_40BD4 */

/* fzgx:begin fn_1_40D44 */
#include "rel/main_rel/game.h"

extern u32 fn_1_40EE4(void);
extern u32 lbl_801A63D0;
extern u32 lbl_1_bss_26C34[9];
extern u8 lbl_1_bss_38218[568];
extern void fn_800205A0(u8 *arg0, u32 arg1, u32 arg2, u32 arg3, u32 arg4,
                        u32 arg5, u32 arg6, u32 arg7);
extern void fn_1_4060(void);

void fn_1_40D44(void) {
    u32 result;
    u32 state;
    u32 arg;

    result = fn_1_40EE4();
    state = lbl_1_bss_3C1C.unk_0;
    if ((state + 0x10000) == 0xffff) {
        lbl_1_bss_3C1C.unk_0 = 0;
        arg = lbl_801A63D0;
    } else {
        arg = state;
    }

    fn_800205A0(lbl_1_bss_38218, 0, 0, 1, arg, result,
                lbl_1_bss_26C34[0], 0);

    lbl_1_bss_3820C = lbl_1_bss_3820C + 1;
    if (lbl_1_bss_3820C > 6 || result >= 0x00f95fff) {
        lbl_1_bss_3820C = 6;
    }

    fn_1_4060();
}
/* fzgx:end fn_1_40D44 */

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

extern u32 lbl_1_bss_38450;
extern u8 lbl_1_data_66C0[0x10];
extern u8 lbl_1_data_66D0[0x10];

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

/* fzgx:begin fn_1_41048 noprologue */
#include "types.h"

typedef struct {
    u8 pad_0[0x8];
    u32 unk_8;  // 58 loads, 0 stores
} Obj_1_bss_38458_Target;
extern u32 lbl_1_bss_3845C;
extern u8 lbl_1_data_66E0[0x10];
extern u8 lbl_1_data_66F0[0x10];
extern Obj_1_bss_38458_Target *lbl_1_bss_38458;

extern u32 lbl_1_bss_3845C;
extern u8 lbl_1_data_66E0[0x10];
extern u8 lbl_1_data_66F0[0x10];

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
