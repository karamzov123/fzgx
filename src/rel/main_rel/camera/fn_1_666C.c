#include "types.h"

typedef struct {
    u32 flags;
    u8 pad[0x1C];
    s16 value;
} CameraState;

extern void fn_1_D9D8(CameraState *);
extern void lbl_8006DBAC(void);
extern void fn_8006F038(void *, void *, s16);
extern void lbl_8006DCDC(void);

void fn_1_666C(CameraState *state) {
    if ((state->flags >> 30) & 1) {
        fn_1_D9D8(state);
        lbl_8006DBAC();
    } else {
        fn_8006F038((u8 *)state + 4, (u8 *)state + 0x10, state->value);
    }

    lbl_8006DCDC();
}
