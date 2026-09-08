#include "rel/main_rel/globals.h"

extern void fn_1_D9D8(__typeof__(lbl_1_bss_F68));
extern void lbl_8006DBAC(void);
extern void fn_8006F038(void *, void *, s16);
extern void lbl_8006DCDC(void);

// Updates the camera state through the active or standard camera path.
void fn_1_666C(__typeof__(lbl_1_bss_F68) state) {
    if ((state->unk_0 >> 30) & 1) {
        fn_1_D9D8(state);
        lbl_8006DBAC();
    } else {
        fn_8006F038(&state->unk_4, &state->unk_4 + 6,
                     *((s16 *)&state->unk_4 + 14));
    }

    lbl_8006DCDC();
}
