#include "types.h"

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
