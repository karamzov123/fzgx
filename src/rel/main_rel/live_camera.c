#include "types.h"
#include "rel/main_rel/globals.h"
#include "rel/main_rel/live_camera.h"

extern void OSPanic(u8 *file, int line, u8 *message, ...);
extern u32 fn_80008E30(u32);
extern u8 lbl_1_bss_17B4[12];
extern void fn_80083DB0(Obj_1_bss_3B30 *, void *);
extern u32 *fn_1_1304C(Obj_1_bss_3B30 *, u32);
extern u8 lbl_1_bss_1810[8984];
extern void fn_8000659C(void);
extern u32 OSIsThreadTerminated(u8 *);

extern void fn_1_6DD0(void *);
extern u32 *fn_1_1289C(u32 *);

/* fzgx:begin fn_1_DA34 */
// Reports the camera source location and message used by this routine.
void fn_1_DA34(void) {
    OSPanic(lbl_1_data_4420, 0x34, lbl_1_data_4430);
}
/* fzgx:end fn_1_DA34 */

/* fzgx:begin fn_1_EE04 */
typedef struct {
    u8 pad[2];
    s16 state;
} Fn1EE04Context;

typedef void (*Fn1EE04Handler)(Fn1EE04Context *, void *);

// Dispatch the active live-camera handler, then advance camera processing.
void fn_1_EE04(Fn1EE04Context *context) {
    ((Fn1EE04Handler *)lbl_1_data_4440)[context->state](
        context, lbl_1_data_4440);
    fn_1_6DD0(0);
}
/* fzgx:end fn_1_EE04 */

/* fzgx:begin fn_1_10138 */
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
u32 fn_1_101C0(void) {
    return lbl_1_bss_17B0;
}
/* fzgx:end fn_1_101C0 */

/* fzgx:begin fn_1_12850 */
u8 fn_1_12850(void) {
    return lbl_1_bss_17B4[0];
}
/* fzgx:end fn_1_12850 */

/* fzgx:begin fn_1_12860 */
// Initializes the live-camera state before returning the active camera result.
u32 fn_1_12860(u32 arg0, u32 arg1) {
    u32 *state = (u32 *)lbl_1_bss_1800;

    state[0] = arg0;
    state[1] = arg1;
    state[2] = 0;
    return fn_1_1289C(state)[0];
}
/* fzgx:end fn_1_12860 */

/* fzgx:begin fn_1_12F78 */
u32 fn_1_12F78(void *arg0, u32 arg1) {
    fn_80083DB0(&lbl_1_bss_3B30, arg0);
    lbl_1_bss_3B30.unk_80 = arg1;
    lbl_1_bss_3B30.unk_84 = 0;
    return *fn_1_1304C(&lbl_1_bss_3B30, 0);
}
/* fzgx:end fn_1_12F78 */

/* fzgx:begin fn_1_13018 */
u32 fn_1_13018(void) {
    fn_8000659C();
    return OSIsThreadTerminated(lbl_1_bss_1810) == 0;
}
/* fzgx:end fn_1_13018 */
