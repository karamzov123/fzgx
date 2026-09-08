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

/* fzgx:begin fn_13_FA8 */
// fn_13_FA8: empty in retail (single blr).
void fn_13_FA8(void) {
}
/* fzgx:end fn_13_FA8 */
