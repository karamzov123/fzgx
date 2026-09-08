#include "rel/main_rel/globals.h"

__typeof__(lbl_1_bss_F68) camera_get_state_object(void);
extern void fn_1_BD54(void);
extern void fn_1_B870(void);

// Dispatches to the camera update routine selected by the returned camera state.
void camera_update(void) {
    __typeof__(lbl_1_bss_F68) state = camera_get_state_object();

    if (((state->unk_0 >> 30) & 1) != 0) {
        fn_1_BD54();
    } else {
        fn_1_B870();
    }
}
