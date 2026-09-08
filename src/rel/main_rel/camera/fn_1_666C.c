#include "rel/main_rel/globals.h"

typedef struct {
    u32 unk_0;
    u8 unk_4[0x1C];
    s16 unk_20;
} CameraState;

extern void fn_1_D9D8(CameraState *);
extern void lbl_8006DBAC(void);
extern void fn_8006F038(void *, void *, s16);
extern void lbl_8006DCDC(void);

// Updates the camera state through the active or standard camera path.
void fn_1_666C(CameraState *state) {
    if ((state->unk_0 >> 30) & 1) {
        fn_1_D9D8(state);
        lbl_8006DBAC();
    } else {
        fn_8006F038((u8 *)state + 4, (u8 *)state + 0x10, state->unk_20);
    }

    lbl_8006DCDC();
}
