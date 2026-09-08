#include "rel/main_rel/globals.h"

extern s32 fn_1_5910(void);
extern void *fn_1_9D260(void);
extern void fn_1_DB198(void *arg0, void *arg1);
extern s16 fn_1_9AD88(void);
extern s16 fn_1_7B054(void);
extern s16 fn_1_3F0C8(void);
extern f32 lbl_1_rodata_663C;
extern void fn_1_1067A8(void *arg0, f32 arg1, f32 arg2);

// Initializes the current background object and updates it for the active state.
void fn_1_DAEFC(void) {
    s32 index;
    Obj_1_data_2A7E0_At3C *object;

    index = fn_1_5910();
    object = lbl_1_data_2A7E0.unk_3C;
    fn_1_DB198(object, fn_1_9D260());
    fn_1_9AD88();
    if (fn_1_7B054() == 42) {
        if (fn_1_3F0C8() != 39) {
            fn_1_1067A8((u8 *)object + 0x142a0, ((f32 *)object)[index], lbl_1_rodata_663C);
        }
    }
}
