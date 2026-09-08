#include "types.h"

/* fzgx:begin fn_1_49F0 */
extern u32 lbl_1_data_2CD8;

void fn_1_49F0(u32 *value) {
    lbl_1_data_2CD8 = *value;
}
/* fzgx:end fn_1_49F0 */

/* fzgx:begin fn_1_4FEC */
extern u32 lbl_1_bss_F58;

void fn_1_4FEC(void) {
    lbl_1_bss_F58 = 0;
}
/* fzgx:end fn_1_4FEC */

/* fzgx:begin fn_1_509C */
extern u32 lbl_1_bss_F58;
extern u32 lbl_1_bss_7AC40[2];
extern u32 lbl_1_bss_7AC20[8];

s32 fn_8008023C(u32 *, u32 *, u32);

s32 fn_1_509C(void) {
    lbl_1_bss_7AC40[0] = lbl_1_bss_F58;
    if (fn_8008023C(lbl_1_bss_7AC20, lbl_1_bss_7AC40, 4) == 0) {
        return 0;
    }
    lbl_1_bss_7AC20[0] = lbl_1_bss_7AC40[0];
    return 1;
}
/* fzgx:end fn_1_509C */

/* fzgx:begin fn_1_52E0 */
typedef struct {
    u8 _pad[0x4b];
    u8 field_4b;
} Fn152E0State;

extern Fn152E0State *lbl_1_bss_F68;
extern u8 lbl_1_bss_F76;

void fn_1_52E0(u8 value) {
    lbl_1_bss_F76 = value;
    if (lbl_1_bss_F68 != 0) {
        lbl_1_bss_F68->field_4b = value;
    }
}
/* fzgx:end fn_1_52E0 */

/* fzgx:begin fn_1_5300 */
typedef struct {
    u8 _pad[0x4b];
    u8 field_4b;
} Fn15300State;

extern Fn15300State *lbl_1_bss_F68;

u8 fn_1_5300(void) {
    u32 value;

    if (lbl_1_bss_F68 != 0) {
        value = lbl_1_bss_F68->field_4b;
    } else {
        value = 0;
    }
    return (u8)value;
}
/* fzgx:end fn_1_5300 */

/* fzgx:begin fn_1_5328 */
typedef struct {
    u8 _pad[0x4c];
    u8 field_4c;
} Fn15328State;

extern Fn15328State *lbl_1_bss_F68;
extern u8 lbl_1_data_3314;

void fn_1_5328(u8 value) {
    lbl_1_data_3314 = value;
    if (lbl_1_bss_F68 != 0) {
        lbl_1_bss_F68->field_4c = value;
    }
}
/* fzgx:end fn_1_5328 */

/* fzgx:begin fn_1_63D4 */
extern u32 live_camera;
void fn_1_F3D0(u32);

void fn_1_63D4(void) {
    fn_1_F3D0(live_camera);
}
/* fzgx:end fn_1_63D4 */
