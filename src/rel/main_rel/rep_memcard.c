#include "types.h"

/* fzgx:begin fn_1_154D84 */
extern u32 lbl_1_bss_8ED90;
extern u32 lbl_1_bss_8ED94[3];
extern u32 lbl_801A6410;
extern char lbl_1_data_49AC8[14];

extern void fn_80008BA8(u32 arg0, u32 arg1, u32 arg2);
extern void fn_1_46B4(u32 arg0, u32 arg1, char *arg2, s32 arg3);

u32 fn_1_154D84(u32 arg0) {
    if (lbl_1_bss_8ED90 == 0) {
        return 0;
    }
    if (arg0 == 0) {
        return 0;
    }

    fn_80008BA8(arg0, lbl_1_bss_8ED90, lbl_1_bss_8ED94[0]);
    fn_1_46B4(lbl_801A6410, lbl_1_bss_8ED90, lbl_1_data_49AC8, 0x13f);
    lbl_1_bss_8ED90 = 0;
    lbl_1_bss_8ED94[0] = 0;
    return 1;
}
/* fzgx:end fn_1_154D84 */

/* fzgx:begin fn_1_154E24 */
extern u8 lbl_1_bss_8EDA0;

u8 fn_1_154E24(void) {
    return lbl_1_bss_8EDA0;
}
/* fzgx:end fn_1_154E24 */

/* fzgx:begin fn_1_154F1C */
extern u32 lbl_1_bss_8EDF0[76];
extern void *memset(void *dest, int value, u32 size);

void fn_1_154F1C(void) {
    u32 *ptr = &lbl_1_bss_8EDF0[13];

    lbl_1_bss_8EDF0[7] = 0;
    lbl_1_bss_8EDF0[5] = 0;
    lbl_1_bss_8EDF0[6] = 0;
    memset(ptr, 0, 250);
    *(u8 *)ptr = 0;
}
/* fzgx:end fn_1_154F1C */

/* fzgx:begin fn_1_154F74 */
extern u32 lbl_1_bss_8EDF0[76];

void fn_1_154F74(u32 arg0, u32 arg1) {
    lbl_1_bss_8EDF0[8] = arg0;
    lbl_1_bss_8EDF0[9] = arg1;
}
/* fzgx:end fn_1_154F74 */

/* fzgx:begin fn_1_154F88 */
extern f32 lbl_1_bss_8EDF0[76];

void fn_1_154F88(f32 value) {
    lbl_1_bss_8EDF0[11] = value;
}
/* fzgx:end fn_1_154F88 */

/* fzgx:begin fn_1_154FC0 */
#include "rel/main_rel/rep_memcard.h"

void fn_1_154FC0(void) {
    lbl_1_bss_8EDF0.unk_8 &= ~4;
}
/* fzgx:end fn_1_154FC0 */
