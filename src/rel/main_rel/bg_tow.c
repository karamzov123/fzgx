#include "rel/main_rel/globals.h"
#include "rel/main_rel/bg_tow.h"

extern void fn_1_B9BE0(void);
extern void fn_1_B9DE8(Obj_1_bss_8EDA4 *arg0);
extern void fn_1_48418(s32 arg0);
extern void fn_1_159440(s32 arg0, s32 arg1);
extern u8 lbl_1_bss_8EDA0;
extern s32 fn_1_BA144(Obj_1_bss_8EDA4 *arg0);
extern void fn_1_1596DC(s32 arg0);
extern void fn_1_484CC(s32 arg0);
extern void fn_1_BC310(Obj_1_bss_8EDA4 *arg0);

/* fzgx:begin fn_1_1548A8 */
#include "types.h"

// Initialize the tow settings and select the mode-dependent input value.
void fn_1_1548A8(void) {
    s16 mode;
    s16 *mode_ptr;
    Obj_1_bss_8EDA4 *obj;

    fn_1_B9BE0();
    fn_1_B9DE8(&lbl_1_bss_8EDA4);

    mode_ptr = (s16 *)&lbl_1_bss_960;
    obj = &lbl_1_bss_8EDA4;
    mode = *mode_ptr;
    obj->unk_0 = 0x20;
    if (mode == 0xc) {
        obj->unk_4 = 0xa;
    } else {
        obj->unk_4 = 9;
    }

    if (mode != 2) {
        fn_1_48418(2);
        fn_1_159440(2, 0);
    }

    lbl_1_data_49AB4.unk_0 = 1;
}
/* fzgx:end fn_1_1548A8 */

/* fzgx:begin fn_1_154930 */
void fn_1_154930(void) {
    s8 result;

    result = fn_1_BA144(&lbl_1_bss_8EDA4);
    if (result == 0) {
        lbl_1_data_49AB4.unk_0 = 2;
    } else if (result == 1) {
        lbl_1_data_49AB4.unk_0 = 4;
        lbl_1_bss_8EDA0 = 1;
        fn_1_1596DC(2);
        fn_1_484CC(2);
    } else {
        fn_1_BC310(&lbl_1_bss_8EDA4);
    }
}
/* fzgx:end fn_1_154930 */

/* fzgx:begin fn_1_1549B8 noprologue */
#include "types.h"

extern struct fn_1_1549B8_lbl_1_bss_8ED90 lbl_1_bss_8ED90;
extern u32 fn_1_C0510(void *, u32, u32);
extern u32 lbl_1_data_49AB4;

struct fn_1_1549B8_lbl_1_bss_8ED90 {
    u32 unk_0;
    u32 unk_4;
};

void fn_1_1549B8(void) {
    void * v0;
    u32 v1;
    u32 v2;
    v0 = ((u8 *)&lbl_1_bss_8ED90 + 20);
    v1 = lbl_1_bss_8ED90.unk_0;
    v2 = lbl_1_bss_8ED90.unk_4;
    *(u32 *)((u8 *)v0 + 8) = v1;
    *(u32 *)((u8 *)v0 + 12) = v2;
    *(u32 *)((u8 *)v0 + 16) = 5;
    fn_1_C0510(v0, v2, v1);
    lbl_1_data_49AB4 = 3;
}
/* fzgx:end fn_1_1549B8 */
