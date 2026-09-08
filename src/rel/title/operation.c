#include "types.h"

/* fzgx:begin fn_8_68D8 */
extern u32 lbl_8_bss_2A8;
extern u32 lbl_8_bss_2C0;

void fn_8_68D8(void) {
    lbl_8_bss_2C0 = 4;
    lbl_8_bss_2A8 = 0;
}
/* fzgx:end fn_8_68D8 */

/* fzgx:begin fn_8_69D8 */
extern struct fn_8_69D8_lbl_801A6410 lbl_801A6410;
extern u32 fn_1_412A0(u32);
extern u32 fn_1_46B4(u32, u32, u32, u32);
extern u32 lbl_801A66B4;
extern u32 lbl_8_bss_2B0;
extern u32 lbl_8_data_8AD4;

struct fn_8_69D8_lbl_801A6410 {
    u32 unk_0;
};

void fn_8_69D8(void) {
    if ((s32)lbl_801A66B4 == 5) {
    fn_1_412A0(4);
    } else {
    fn_1_412A0(5);
    }
    if (lbl_8_bss_2B0 != 0) {
    fn_1_46B4(lbl_801A6410.unk_0, lbl_8_bss_2B0, (u32)&lbl_8_data_8AD4, 1199);
    lbl_8_bss_2B0 = 0;
    }
}
/* fzgx:end fn_8_69D8 */

/* fzgx:begin fn_8_6C50 */
extern u32 fn_1_412A0(u32);
extern u32 lbl_801A66B4;

void fn_8_6C50(void) {
    if ((s32)lbl_801A66B4 == 5) {
    fn_1_412A0(4);
    } else {
    fn_1_412A0(5);
    }
}
/* fzgx:end fn_8_6C50 */

/* fzgx:begin fn_8_8C44 */
// fn_8_8C44: empty in retail (single blr).
void fn_8_8C44(void) {
}
/* fzgx:end fn_8_8C44 */

/* fzgx:begin fn_8_9B04 */
extern struct fn_8_9B04_lbl_8_bss_2A8 lbl_8_bss_2A8;
extern u32 fn_80008BA8(u32, u32, u32);
extern u32 fn_8_5C54(u32);

struct fn_8_9B04_lbl_8_bss_2A8 {
    u8 pad_0[0x8];
    u32 unk_8;
    u8 pad_C[0x130];
    u32 unk_13C;
    u8 pad_140[0x11C];
    u32 unk_25C;
};

void fn_8_9B04(void) {
    fn_80008BA8(lbl_8_bss_2A8.unk_8, lbl_8_bss_2A8.unk_13C, lbl_8_bss_2A8.unk_25C);
    fn_8_5C54(1);
}
/* fzgx:end fn_8_9B04 */

/* fzgx:begin fn_8_9B40 */
// fn_8_9B40: empty in retail (single blr).
void fn_8_9B40(void) {
}
/* fzgx:end fn_8_9B40 */

/* fzgx:begin fn_8_CF58 */
// fn_8_CF58: returns a constant.
int fn_8_CF58(void) {
    return 0;
}
/* fzgx:end fn_8_CF58 */

/* fzgx:begin fn_8_CF60 */
// fn_8_CF60: returns a constant.
int fn_8_CF60(void) {
    return 0;
}
/* fzgx:end fn_8_CF60 */

/* fzgx:begin fn_8_CF68 */
// fn_8_CF68: returns a constant.
int fn_8_CF68(void) {
    return 2;
}
/* fzgx:end fn_8_CF68 */

/* fzgx:begin fn_8_CF70 */
extern u32 fn_1_D0790(void);

s32 fn_8_CF70(void) {
    fn_1_D0790();
    return 0;
}
/* fzgx:end fn_8_CF70 */

/* fzgx:begin fn_8_D020 */
// fn_8_D020: empty in retail (single blr).
void fn_8_D020(void) {
}
/* fzgx:end fn_8_D020 */

/* fzgx:begin fn_8_D024 */
extern u32 lbl_8_bss_544;

void fn_8_D024(void) {
    lbl_8_bss_544 = 4;
}
/* fzgx:end fn_8_D024 */

/* fzgx:begin fn_8_D5F4 */
extern u32 fn_1_14BD74(u32);
extern u32 fn_1_15555C(u32);
extern u32 fn_1_48140(u32);

void fn_8_D5F4(void) {
    u32 t2, t3;
    fn_1_48140(149);
    fn_1_48140(152);
    t2 = fn_1_48140(151);
    t3 = fn_1_15555C(t2);
    fn_1_14BD74(t3);
}
/* fzgx:end fn_8_D5F4 */
