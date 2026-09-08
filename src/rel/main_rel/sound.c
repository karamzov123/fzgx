#include "types.h"

/* fzgx:begin fn_1_9FE74 */
extern void *lbl_1_bss_6EA9C;
extern u32 lbl_1_data_33F1C;
extern void *lbl_801A6410;
extern u32 fn_1_44A4(void);
extern void *fn_80008E84(void *arg);
extern u32 fn_1_45D0(void *arg0, void *arg1, u32 *arg2, int arg3);

u32 fn_1_9FE74(void *arg) {
    u32 result;

    lbl_1_bss_6EA9C = fn_80008E84((void *)fn_1_44A4());
    result = fn_1_45D0(lbl_801A6410, arg, &lbl_1_data_33F1C, 0x94);
    fn_80008E84(lbl_1_bss_6EA9C);
    return result;
}
/* fzgx:end fn_1_9FE74 */

/* fzgx:begin fn_1_9FEE8 */
extern u32 lbl_1_bss_6EA9C;
extern u32 lbl_1_data_33F1C;
extern u32 lbl_801A6410;
extern u32 fn_80008E84();
extern void fn_1_44A4(void);
extern void fn_1_46B4(u32, void *, void *, u32);

void fn_1_9FEE8(void *arg) {
    fn_1_44A4();
    lbl_1_bss_6EA9C = fn_80008E84();
    fn_1_46B4(lbl_801A6410, arg, &lbl_1_data_33F1C, 0x9c);
    fn_80008E84(lbl_1_bss_6EA9C);
}
/* fzgx:end fn_1_9FEE8 */

/* fzgx:begin fn_1_9FF50 */
// fn_1_9FF50: empty in retail (single blr).
void fn_1_9FF50(void) {
}
/* fzgx:end fn_1_9FF50 */

/* fzgx:begin fn_1_9FF54 */
// fn_1_9FF54: empty in retail (single blr).
void fn_1_9FF54(void) {
}
/* fzgx:end fn_1_9FF54 */

/* fzgx:begin fn_1_9FF58 */
extern s8 lbl_1_bss_6EAA6;
extern u8 lbl_1_bss_6EAA4;

extern void fn_80067974(void);
extern void fn_800280F0(void);
extern void fn_80026FE0(void);

void fn_1_9FF58(void) {
    if (lbl_1_bss_6EAA6) {
        fn_80067974();
        fn_800280F0();
        fn_80026FE0();
    }
    lbl_1_bss_6EAA4 = 0;
}
/* fzgx:end fn_1_9FF58 */

/* fzgx:begin fn_1_9FF9C */
extern u8 lbl_1_bss_6EAA8[8];
extern u8 lbl_1_bss_6EAB0;

extern void fn_8001EA30(u8 *arg0, int arg1);
extern void fn_80020530(void);

void fn_1_9FF9C(void) {
    fn_8001EA30(lbl_1_bss_6EAA8, 2);
    fn_80020530();
    lbl_1_bss_6EAB0 = 0;
}
/* fzgx:end fn_1_9FF9C */

/* fzgx:begin fn_1_A04AC */
extern u8 lbl_1_bss_6F1E0;
extern u8 lbl_1_bss_6F1E1;

void fn_1_A04AC(void) {
    lbl_1_bss_6F1E0 = 1;
    lbl_1_bss_6F1E1 = 1;
}
/* fzgx:end fn_1_A04AC */

/* fzgx:begin fn_1_A0C68 */
#include "rel/main_rel/sound.h"

extern void fn_1_A2DF4(s32, void *, s32);
extern void fn_1_A2E24(s32, void *, s32);
extern void fn_80008BEC(void *, s32, s32);

enum {
    ADDR_A6000000 = 0xA6000000,
    ADDR_A5100000 = 0xA5100000,
    ADDR_A0000300 = 0xA0000300,
    ADDR_A0000400 = 0xA0000400
};

typedef struct SoundState {
    u8 pad_2e[0x2e];
    u8 unk_2e;
    u8 pad_74c[0x74c - 0x2f];
    u8 unk_74c[0x50];
    u8 pad_7ac[0x7ac - 0x79c];
    u8 unk_7ac;
} SoundState;

void fn_1_A0C68(void) {
    SoundState *state;
    s32 i;

    state = (SoundState *)&lbl_1_bss_6EA98;
    if (state->unk_2e != 0) {
        fn_1_A2DF4(0x10, (void *)ADDR_A6000000, 0);
        fn_1_A2DF4(0x10, (void *)ADDR_A5100000, 0);

        for (i = 1; i < 4; i++) {
            fn_1_A2E24(i, (void *)ADDR_A0000300, 0);
        }

        fn_1_A2E24(0x10, (void *)ADDR_A0000400, 0);
        fn_80008BEC(&state->unk_74c, 0, 0x50);
        state->unk_7ac = 0;
    }
}
/* fzgx:end fn_1_A0C68 */

/* fzgx:begin fn_1_A1340 */
extern void fn_1_A0680(void);

void fn_1_A1340(void) {
    fn_1_A0680();
}
/* fzgx:end fn_1_A1340 */

/* fzgx:begin fn_1_A1360 */
// fn_1_A1360: empty in retail (single blr).
void fn_1_A1360(void) {
}
/* fzgx:end fn_1_A1360 */

/* fzgx:begin fn_1_A2D84 */
extern u8 lbl_1_bss_6F1D8;
extern u32 lbl_1_bss_6F1DC;
extern void fn_80067898(u32 arg0);

void fn_1_A2D84(u32 arg0) {
    if (lbl_1_bss_6F1D8 == 0 && lbl_1_bss_6F1DC <= 0x2D) {
        fn_80067898(arg0);
    }
}
/* fzgx:end fn_1_A2D84 */

/* fzgx:begin fn_1_A2DC4 */
extern u32 lbl_1_bss_6F1DC;
extern void fn_80067898(u32 arg0);

void fn_1_A2DC4(u32 arg0) {
    if (lbl_1_bss_6F1DC <= 0x2D) {
        fn_80067898(arg0);
    }
}
/* fzgx:end fn_1_A2DC4 */

/* fzgx:begin fn_1_A2E24 */
#include "rel/main_rel/sound.h"

extern void fn_800674FC(u16 arg0, u32 arg1, s16 arg2);

void fn_1_A2E24(u32 arg0, u32 arg1, s32 arg2) {
    if (lbl_1_bss_6EAC6.unk_0 != 0) {
        fn_800674FC(arg0, arg1, arg2);
    }
}
/* fzgx:end fn_1_A2E24 */

/* fzgx:begin fn_1_A50C8 */
extern u32 lbl_1_rodata_45D0[];
extern u8 lbl_1_bss_6F1D8;
extern u32 lbl_1_bss_6F1DC;

extern void fn_80067344(s32, s32, u32, s32);
extern void fn_80067898(u32);

void fn_1_A50C8(s32 index) {
    u32 value;

    if (index > 0x28) {
        value = 0xA9100700;
    } else {
        value = lbl_1_rodata_45D0[index * 2];
    }

    fn_80067344(2, 0xF, 0xB0270000, 0x7F);

    if (!lbl_1_bss_6F1D8 && lbl_1_bss_6F1DC <= 0x2D) {
        fn_80067898(value);
    }
}
/* fzgx:end fn_1_A50C8 */

/* fzgx:begin fn_1_A5330 */
extern u8 lbl_1_bss_6F41C[260];

void fn_1_A5330(u8 value, s16 index) {
    lbl_1_bss_6F41C[index] = value;
}
/* fzgx:end fn_1_A5330 */

/* fzgx:begin fn_1_A5590 */
// fn_1_A5590: empty in retail (single blr).
void fn_1_A5590(void) {
}
/* fzgx:end fn_1_A5590 */
