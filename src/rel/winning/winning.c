#include "types.h"

/* fzgx:begin fn_15_B8C */
// fn_15_B8C: empty in retail (single blr).
void fn_15_B8C(void) {
}
/* fzgx:end fn_15_B8C */

/* fzgx:begin fn_15_1EE0 */
extern struct fn_15_1EE0_lbl_15_bss_0 lbl_15_bss_0;
extern u32 camera_enable_flags(u32);
extern u32 fn_15_236C(u32, u32);

struct fn_15_1EE0_lbl_15_bss_0 {
    u8 pad_0[0xC];
    u32 unk_C;
    u32 unk_10;
    u32 unk_14;
};

void fn_15_1EE0(void) {
    struct fn_15_1EE0_lbl_15_bss_0 *p_lbl_15_bss_0;
    u32 t0;
    p_lbl_15_bss_0 = (struct fn_15_1EE0_lbl_15_bss_0 *)&lbl_15_bss_0;
    t0 = fn_15_236C(p_lbl_15_bss_0->unk_C, p_lbl_15_bss_0->unk_10);
    if ((p_lbl_15_bss_0->unk_14 & 0x1) != 0) {
    camera_enable_flags(t0);
    }
}
/* fzgx:end fn_15_1EE0 */

/* fzgx:begin fn_15_2CEC */
// fn_15_2CEC: empty in retail (single blr).
void fn_15_2CEC(void) {
}
/* fzgx:end fn_15_2CEC */

/* fzgx:begin fn_15_3AFC */
// fn_15_3AFC: empty in retail (single blr).
void fn_15_3AFC(void) {
}
/* fzgx:end fn_15_3AFC */

/* fzgx:begin fn_15_3B00 */
extern u32 fn_1_79100(void);
extern u32 fn_1_7EAE8(u32);

void fn_15_3B00(void) {
    u32 t0;
    t0 = fn_1_79100();
    fn_1_7EAE8(t0);
}
/* fzgx:end fn_15_3B00 */

/* fzgx:begin fn_15_405C */
// fn_15_405C: empty in retail (single blr).
void fn_15_405C(void) {
}
/* fzgx:end fn_15_405C */

/* fzgx:begin fn_15_4060 */
extern struct fn_15_4060_lbl_15_bss_0 lbl_15_bss_0;
extern struct fn_15_4060_lbl_801A6410 lbl_801A6410;
extern u32 fn_1_41A8(u32);
extern u32 fn_1_435C(u32);
extern u32 fn_1_46B4(u32, u32, void *, u32);
extern u32 fn_1_58854(u32);
extern u32 fn_1_85878(void);
extern u32 fn_1_982C4(u32);
extern u32 fn_1_C47B4(u32);
extern u32 fn_1_C6F80(u32);
extern u32 lbl_15_data_A4;

struct fn_15_4060_lbl_15_bss_0 {
    u8 pad_0[0x21];
    u8 unk_21;
    u8 pad_22[0x2];
    u32 unk_24;
    u32 unk_28;
    u8 pad_2C[0x124];
    u32 unk_150;
};
struct fn_15_4060_lbl_801A6410 {
    u32 unk_0;
};

void fn_15_4060(void) {
    struct fn_15_4060_lbl_15_bss_0 *p_lbl_15_bss_0;
    u32 t0, t1, t2, t6, t8;
    p_lbl_15_bss_0 = (struct fn_15_4060_lbl_15_bss_0 *)&lbl_15_bss_0;
    t0 = fn_1_85878();
    t1 = fn_1_982C4(t0);
    t2 = fn_1_58854(t1);
    fn_1_C47B4(t2);
    fn_1_C6F80(0);
    fn_1_46B4(lbl_801A6410.unk_0, p_lbl_15_bss_0->unk_150, &lbl_15_data_A4, 2849);
    t6 = fn_1_435C(p_lbl_15_bss_0->unk_24);
    fn_1_41A8(t6);
    t8 = fn_1_435C(p_lbl_15_bss_0->unk_28);
    fn_1_41A8(t8);
    fn_1_46B4(lbl_801A6410.unk_0, p_lbl_15_bss_0->unk_24, &lbl_15_data_A4, 2854);
    fn_1_46B4(lbl_801A6410.unk_0, p_lbl_15_bss_0->unk_28, &lbl_15_data_A4, 2855);
    p_lbl_15_bss_0->unk_21 = 0;
}
/* fzgx:end fn_15_4060 */

/* fzgx:begin fn_15_4568 */
extern u16 lbl_15_bss_154;

void fn_15_4568(void) {
    lbl_15_bss_154 = 0;
}
/* fzgx:end fn_15_4568 */

/* fzgx:begin fn_15_545C */
// fn_15_545C: empty in retail (single blr).
void fn_15_545C(void) {
}
/* fzgx:end fn_15_545C */
