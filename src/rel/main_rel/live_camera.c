#include "types.h"

/* fzgx:begin fn_1_DA34 */
extern char lbl_1_data_4420[14];
extern char lbl_1_data_4430[15];

extern void fn_8000C49C(char *, s32, ...);

void fn_1_DA34(void) {
    fn_8000C49C(lbl_1_data_4420, 0x34, lbl_1_data_4430);
}
/* fzgx:end fn_1_DA34 */

/* fzgx:begin fn_1_EE04 */
typedef struct {
    u8 pad[2];
    s16 state;
} Fn1EE04Context;

typedef void (*Fn1EE04Handler)(Fn1EE04Context *);

extern Fn1EE04Handler lbl_1_data_4440[17];
extern void fn_1_6DD0(void *);

void fn_1_EE04(Fn1EE04Context *context) {
    lbl_1_data_4440[context->state](context);
    fn_1_6DD0(0);
}
/* fzgx:end fn_1_EE04 */

/* fzgx:begin fn_1_10138 */
#include "rel/main_rel/live_camera.h"

extern u32 fn_80008E30(u32);

void fn_1_10138(void) {
    Obj_1_bss_17C4_At0 *obj;
    u32 i;
    u32 count;

    lbl_1_bss_17B0 = 0;
    count = lbl_1_data_44F4.unk_0;
    obj = lbl_1_bss_17C4.unk_0;
    i = 0;
    while (i < count) {
        lbl_1_bss_17B0 += fn_80008E30(obj->unk_0);
        i++;
        obj++;
    }
}
/* fzgx:end fn_1_10138 */

/* fzgx:begin fn_1_101C0 */
extern u32 lbl_1_bss_17B0;

u32 fn_1_101C0(void) {
    return lbl_1_bss_17B0;
}
/* fzgx:end fn_1_101C0 */

/* fzgx:begin fn_1_12850 */
extern u8 lbl_1_bss_17B4[12];

u8 fn_1_12850(void) {
    return lbl_1_bss_17B4[0];
}
/* fzgx:end fn_1_12850 */

/* fzgx:begin fn_1_12860 */
extern u32 lbl_1_bss_1800[4];
extern u32 *fn_1_1289C(u32 *);

u32 fn_1_12860(u32 arg0, u32 arg1) {
    lbl_1_bss_1800[0] = arg0;
    lbl_1_bss_1800[1] = arg1;
    lbl_1_bss_1800[2] = 0;
    return fn_1_1289C(lbl_1_bss_1800)[0];
}
/* fzgx:end fn_1_12860 */

/* fzgx:begin fn_1_12F78 */
#include "rel/main_rel/live_camera.h"

extern void fn_80083DB0(Obj_1_bss_3B30 *, void *);
extern u32 *fn_1_1304C(Obj_1_bss_3B30 *, u32);

u32 fn_1_12F78(void *arg0, u32 arg1) {
    fn_80083DB0(&lbl_1_bss_3B30, arg0);
    lbl_1_bss_3B30.unk_80 = arg1;
    lbl_1_bss_3B30.unk_84 = 0;
    return *fn_1_1304C(&lbl_1_bss_3B30, 0);
}
/* fzgx:end fn_1_12F78 */

/* fzgx:begin fn_1_13018 */
extern u8 lbl_1_bss_1810[8984];
extern void fn_8000659C(void);
extern u32 fn_800102B8(u8 *);

u32 fn_1_13018(void) {
    fn_8000659C();
    return fn_800102B8(lbl_1_bss_1810) == 0;
}
/* fzgx:end fn_1_13018 */
