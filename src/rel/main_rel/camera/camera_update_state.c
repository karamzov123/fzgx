#include "rel/main_rel/globals.h"

extern void camera_get_extended_state_storage(__typeof__(lbl_1_bss_F68));
extern void lbl_8006DBAC(void);
extern void fn_8006F038(void *, void *, s16);
extern void lbl_8006DCDC(void);

// Updates the camera state through the active or standard camera path.
void camera_update_state(__typeof__(lbl_1_bss_F68) state) {
    if ((state->unk_0 >> 30) & 1) {
        camera_get_extended_state_storage(state);
        lbl_8006DBAC();
    } else {
        fn_8006F038(&state->unk_4, &state->unk_4 + 6,
                     *((s16 *)&state->unk_4 + 14));
    }

    lbl_8006DCDC();
}
