#include "types.h"

/* fzgx:begin fn_8_FF9C */
struct fn_8_FF9C_lbl_8_bss_550 {
    u32 unk_0;
};
struct fn_8_FF9C_lbl_801A6410 {
    u32 unk_0;
};

extern s16 lbl_1_bss_960;
extern struct fn_8_FF9C_lbl_801A6410 lbl_801A6410;
extern struct fn_8_FF9C_lbl_8_bss_550 lbl_8_bss_550;
extern u32 fn_1_412A0(u32);
extern u32 fn_1_426C(u32);
extern u32 fn_1_435C(u32);
extern u32 fn_1_46B4(u32, u32, u32, u32);
extern u32 fn_1_48140(u32);
extern u32 fn_1_A8F78(void);
extern u32 lbl_8_data_A288;
extern u8 lbl_8_bss_554;

void fn_8_FF9C(void) {
    u32 v0;
    u32 v1;
    u32 t4, t5;
    v0 = lbl_8_bss_550.unk_0;
    lbl_8_bss_554 = 1;
    fn_1_435C(*(u32 *)((u8 *)v0 + 4));
    fn_1_426C(*(u32 *)((u8 *)lbl_8_bss_550.unk_0 + 24));
    fn_1_A8F78();
    fn_1_48140(143);
    if (lbl_1_bss_960 != 1) {
    if (lbl_1_bss_960 != 3) {
    v1 = 1;
    t4 = fn_1_412A0(v1);
    v1 = t4;
    v1 = 154;
    t5 = fn_1_48140(v1);
    v1 = t5;
    }
    v1 = 158;
    fn_1_48140(v1);
    }
    fn_1_435C(*(u32 *)((u8 *)lbl_8_bss_550.unk_0 + 8));
    fn_1_426C(*(u32 *)((u8 *)lbl_8_bss_550.unk_0 + 28));
    fn_1_46B4(lbl_801A6410.unk_0, lbl_8_bss_550.unk_0, (u32)&lbl_8_data_A288, 1378);
    lbl_8_bss_550.unk_0 = 0;
}
/* fzgx:end fn_8_FF9C */

/* fzgx:begin fn_8_10084 */
extern struct fn_8_10084_lbl_8_bss_550 lbl_8_bss_550;
extern u32 fn_80083DB0(u32, u32);

struct fn_8_10084_lbl_8_bss_550 {
    u32 unk_0;
};

void fn_8_10084(u32 arg0) {
    fn_80083DB0(arg0, (lbl_8_bss_550.unk_0 + 40));
}
/* fzgx:end fn_8_10084 */

/* fzgx:begin fn_8_100B4 */
extern u8 lbl_8_bss_555;

void fn_8_100B4(void) {
    lbl_8_bss_555 = 1;
}
/* fzgx:end fn_8_100B4 */
