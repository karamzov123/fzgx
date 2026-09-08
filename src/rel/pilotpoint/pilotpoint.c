#include "types.h"

/* fzgx:begin fn_14_220 */
extern struct fn_14_220_lbl_14_bss_1C lbl_14_bss_1C;
extern struct fn_14_220_lbl_14_bss_20 lbl_14_bss_20;
extern u32 fn_1_3CC4(u32);
extern u32 fn_1_407C(u32);
extern u32 fn_1_435C(u32);

struct fn_14_220_lbl_14_bss_1C {
    u32 unk_0;
};
struct fn_14_220_lbl_14_bss_20 {
    u32 unk_0;
};

void fn_14_220(void) {
    u32 t0, t1, t3;
    t0 = fn_1_435C(lbl_14_bss_1C.unk_0);
    t1 = fn_1_407C(t0);
    fn_1_3CC4(t1);
    t3 = fn_1_435C(lbl_14_bss_20.unk_0);
    fn_1_407C(t3);
}
/* fzgx:end fn_14_220 */

/* fzgx:begin fn_14_900 */
// fn_14_900: empty in retail (single blr).
void fn_14_900(void) {
}
/* fzgx:end fn_14_900 */

/* fzgx:begin fn_14_258C */
// fn_14_258C: empty in retail (single blr).
void fn_14_258C(void) {
}
/* fzgx:end fn_14_258C */

/* fzgx:begin fn_14_315C */
extern struct fn_14_315C_lbl_14_bss_110 lbl_14_bss_110;

struct fn_14_315C_lbl_14_bss_110 {
    u8 pad_0[0x4C];
    u32 unk_4C;
};

void fn_14_315C(u32 arg0) {
    lbl_14_bss_110.unk_4C = arg0;
}
/* fzgx:end fn_14_315C */

/* fzgx:begin fn_14_600C */
extern struct fn_14_600C_lbl_14_bss_92E8 lbl_14_bss_92E8;
extern u32 fn_80074188(u32, u32, u32, u32);

struct fn_14_600C_lbl_14_bss_92E8 {
    s16 unk_0;
    s16 unk_2;
    s16 unk_4;
    s16 unk_6;
};

void fn_14_600C(void) {
    fn_80074188(lbl_14_bss_92E8.unk_0, lbl_14_bss_92E8.unk_2, lbl_14_bss_92E8.unk_4, lbl_14_bss_92E8.unk_6);
}
/* fzgx:end fn_14_600C */

/* fzgx:begin fn_14_645C */
extern struct fn_14_645C_lbl_14_bss_92F0 lbl_14_bss_92F0;
extern u32 fn_80074188(u32, u32, u32, u32);

struct fn_14_645C_lbl_14_bss_92F0 {
    s16 unk_0;
    s16 unk_2;
    s16 unk_4;
    s16 unk_6;
};

void fn_14_645C(void) {
    fn_80074188(lbl_14_bss_92F0.unk_0, lbl_14_bss_92F0.unk_2, lbl_14_bss_92F0.unk_4, lbl_14_bss_92F0.unk_6);
}
/* fzgx:end fn_14_645C */

/* fzgx:begin fn_14_A244 */
extern struct fn_14_A244_lbl_14_bss_20 lbl_14_bss_20;
extern u16 lbl_14_bss_9452;
extern u32 fn_1_12F1E8(u32);
extern u32 fn_1_3F8C(u32, u32, u32, u32);
extern u32 fn_1_435C(u32);
extern u32 fn_1_47F74(u32);
extern u32 lbl_14_data_30D4;
extern void fn_14_A3DC(void);

struct fn_14_A244_lbl_14_bss_20 {
    u32 unk_0;
};

void fn_14_A244(void) {
    u32 t3;
    fn_1_47F74(151);
    fn_1_12F1E8(4800);
    fn_1_435C(lbl_14_bss_20.unk_0);
    t3 = fn_1_3F8C((u32)&lbl_14_data_30D4, (u32)fn_14_A3DC, 0, 8);
    lbl_14_bss_9452 = t3;
}
/* fzgx:end fn_14_A244 */

/* fzgx:begin fn_14_A398 */
extern struct fn_14_A398_lbl_14_bss_20 lbl_14_bss_20;
extern struct fn_14_A398_lbl_14_bss_9452 lbl_14_bss_9452;
extern u32 fn_1_426C(u32);
extern u32 fn_1_435C(u32);
extern u32 fn_1_48140(u32);

struct fn_14_A398_lbl_14_bss_20 {
    u32 unk_0;
};
struct fn_14_A398_lbl_14_bss_9452 {
    s16 unk_0;
};

void fn_14_A398(void) {
    fn_1_48140(151);
    fn_1_435C(lbl_14_bss_20.unk_0);
    fn_1_426C(lbl_14_bss_9452.unk_0);
}
/* fzgx:end fn_14_A398 */

/* fzgx:begin fn_14_B480 */
// fn_14_B480: empty in retail (single blr).
void fn_14_B480(void) {
}
/* fzgx:end fn_14_B480 */

/* fzgx:begin fn_14_BCDC */
extern u32 lbl_14_bss_0;
extern u32 lbl_14_bss_9460;

void fn_14_BCDC(void) {
    lbl_14_bss_9460 = 0;
    lbl_14_bss_0 = 366;
}
/* fzgx:end fn_14_BCDC */

/* fzgx:begin fn_14_C4F4 */
// fn_14_C4F4: empty in retail (single blr).
void fn_14_C4F4(void) {
}
/* fzgx:end fn_14_C4F4 */

/* fzgx:begin fn_14_C698 */
// fn_14_C698: empty in retail (single blr).
void fn_14_C698(void) {
}
/* fzgx:end fn_14_C698 */

/* fzgx:begin fn_14_DC28 */
// fn_14_DC28: empty in retail (single blr).
void fn_14_DC28(void) {
}
/* fzgx:end fn_14_DC28 */

/* fzgx:begin fn_14_DC6C */
extern u32 lbl_14_data_6C8;

u32 fn_14_DC6C(u32 arg0, u32 arg1) {
    return *(u32 *)((u8 *)((u8 *)&lbl_14_data_6C8 + ((s16)arg0 * 24)) + ((s16)arg1 << 2));
}
/* fzgx:end fn_14_DC6C */
