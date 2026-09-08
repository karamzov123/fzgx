#include "types.h"
extern u8 lbl_3_bss_A17A0[44];
extern s16 lbl_3_bss_A17CC;
extern u32 lbl_3_bss_A176C;
extern u32 *lbl_3_bss_A1770;
extern u32 *lbl_3_bss_A1774;
extern u8 lbl_3_data_3574[60];
extern u32 lbl_801A6410;
extern void fn_1_46B4(u32, u32, u8 *, s32);

/* fzgx:begin fn_3_14008 */
void fn_3_14008(void) {
    u32 *value = (u32 *)lbl_3_bss_A17A0;
    *value |= 0x40000000;
}
/* fzgx:end fn_3_14008 */

/* fzgx:begin fn_3_1401C */
u32 fn_3_1401C(void) {
    return *(u32 *)(lbl_3_bss_A17A0 + 0x20);
}
/* fzgx:end fn_3_1401C */

/* fzgx:begin fn_3_1402C */
u32 fn_3_1402C(u8 index) {
    if (lbl_3_bss_A17CC <= index) {
        return 0;
    }
    return ((u32 *)lbl_3_bss_A176C)[index];
}
/* fzgx:end fn_3_1402C */

/* fzgx:begin fn_3_14064 */
u32 fn_3_14064(void) {
    return lbl_3_bss_A17CC & 0xff;
}
/* fzgx:end fn_3_14064 */

/* fzgx:begin fn_3_14074 */
u8 *fn_3_14074(void) {
    return lbl_3_bss_A17A0;
}
/* fzgx:end fn_3_14074 */

/* fzgx:begin fn_3_142C4 */
void fn_3_142C4(void) {
    u8 i;

    for (i = 0; i < 0x80; i++) {
        if (lbl_3_bss_A1770[i] != 0) {
            fn_1_46B4(lbl_801A6410, lbl_3_bss_A1770[i],
                       lbl_3_data_3574, 0x321);
            lbl_3_bss_A1770[i] = 0;
        }

        if (lbl_3_bss_A1774[i] != 0) {
            fn_1_46B4(lbl_801A6410, lbl_3_bss_A1774[i],
                       lbl_3_data_3574, 0x326);
            lbl_3_bss_A1774[i] = 0;
        }
    }

    if (lbl_3_bss_A1774 != 0) {
        fn_1_46B4(lbl_801A6410, (u32)lbl_3_bss_A1774,
                   lbl_3_data_3574, 0x32e);
        lbl_3_bss_A1774 = 0;
    }

    if (lbl_3_bss_A1770 != 0) {
        fn_1_46B4(lbl_801A6410, (u32)lbl_3_bss_A1770,
                   lbl_3_data_3574, 0x334);
        lbl_3_bss_A1770 = 0;
    }
}
/* fzgx:end fn_3_142C4 */

/* fzgx:begin fn_3_1441C */
typedef struct fn_3_1441C_EmblemState {
    u8 pad[0x18];
    s16 field_18;
    s16 field_1a;
} fn_3_1441C_EmblemState;

u8 fn_3_1441C(void) {
    fn_3_1441C_EmblemState *state = (fn_3_1441C_EmblemState *)lbl_3_bss_A17A0;
    return state->field_1a == state->field_18;
}
/* fzgx:end fn_3_1441C */

/* fzgx:begin fn_3_1443C */
typedef struct {
    u8 pad16[0x16];
    s16 value16;
    u8 pad18[2];
    s16 value1a;
} fn_3_1443C_EmblemState;

u8 fn_3_1443C(void) {
    fn_3_1443C_EmblemState *state = (fn_3_1443C_EmblemState *)lbl_3_bss_A17A0;
    return state->value1a == state->value16;
}
/* fzgx:end fn_3_1443C */

/* fzgx:begin fn_3_146BC */
// fn_3_146BC: empty in retail (single blr).
void fn_3_146BC(void) {
}
/* fzgx:end fn_3_146BC */
