#include "types.h"

/* fzgx:begin fn_13_E8C */
extern struct fn_13_E8C_lbl_1_bss_960 lbl_1_bss_960;
extern struct fn_13_E8C_lbl_801A63D0 lbl_801A63D0;
extern u32 fn_1_1547FC(u32);
extern u32 fn_1_154D84(u32);
extern u32 fn_1_1596DC(u32);
extern u32 fn_1_412A0(u32);
extern u32 fn_1_47A60(u32);
extern u32 fn_1_484CC(u32);
extern u32 fn_1_A0AA4(u32);
extern u32 lbl_13_bss_4C;

struct fn_13_E8C_lbl_1_bss_960 {
    s16 unk_0;
};
struct fn_13_E8C_lbl_801A63D0 {
    u32 unk_0;
};

void fn_13_E8C(void) {
    u32 t1, t5;
    fn_1_1596DC(2);
    t1 = fn_1_484CC(2);
    fn_1_A0AA4(t1);
    fn_1_47A60(lbl_1_bss_960.unk_0);
    fn_1_412A0(1);
    t5 = fn_1_154D84(lbl_801A63D0.unk_0);
    lbl_13_bss_4C = t5;
    fn_1_1547FC(t5);
}
/* fzgx:end fn_13_E8C */

/* fzgx:begin fn_13_EF0 */
extern u32 lbl_13_bss_50;

void fn_13_EF0(void) {
    lbl_13_bss_50 = 3;
}
/* fzgx:end fn_13_EF0 */

/* fzgx:begin fn_13_F00 */
extern s16 lbl_1_bss_968;
extern struct fn_13_F00_lbl_13_bss_0 lbl_13_bss_0;
extern struct fn_13_F00_lbl_801A6410 lbl_801A6410;
extern u16 lbl_1_bss_96A;
extern u32 fn_13_0(void);
extern u32 fn_1_41A8(u32);
extern u32 fn_1_435C(u32);
extern u32 fn_1_46B4(u32, u32, void *, u32);
extern u32 lbl_13_data_48;

struct fn_13_F00_lbl_13_bss_0 {
    u8 pad_0[0x40];
    u32 unk_40;
    u32 unk_44;
    u8 pad_48[0x8];
    u32 unk_50;
};
struct fn_13_F00_lbl_801A6410 {
    u32 unk_0;
};

void fn_13_F00(void) {
    struct fn_13_F00_lbl_13_bss_0 *p_lbl_13_bss_0;
    u32 v0;
    u32 v1;
    u32 t1;
    p_lbl_13_bss_0 = (struct fn_13_F00_lbl_13_bss_0 *)&lbl_13_bss_0;
    v0 = p_lbl_13_bss_0->unk_50;
    p_lbl_13_bss_0->unk_50 = (v0 - 1);
    if ((s32)v0 <= 0) {
    if (lbl_1_bss_968 != 1) {
    if ((s32)p_lbl_13_bss_0->unk_40 != 167) {
    fn_13_0();
    }
    }
    v1 = p_lbl_13_bss_0->unk_44;
    if (v1 != 0) {
    t1 = fn_1_435C(v1);
    fn_1_41A8(t1);
    fn_1_46B4(lbl_801A6410.unk_0, p_lbl_13_bss_0->unk_44, &lbl_13_data_48, 713);
    p_lbl_13_bss_0->unk_44 = 0;
    }
    lbl_1_bss_96A = p_lbl_13_bss_0->unk_40;
    }
}
/* fzgx:end fn_13_F00 */

/* fzgx:begin fn_13_FA8 */
// fn_13_FA8: empty in retail (single blr).
void fn_13_FA8(void) {
}
/* fzgx:end fn_13_FA8 */
