#include "types.h"

typedef struct MovieModuleState {
    u8 pad_0c[0x0c];
    s32 status;
    u8 pad_10[0x30];
    s32 value_40;
    u8 pad_44[0xa4];
    void *movie;
} MovieModuleState;

extern s32 fn_12_2FA90(s32 arg0, s32 arg1, void *arg2, s32 arg3);

s32 fn_12_35418(MovieModuleState *self) {
    s32 status_eq;

    status_eq = self->status == 2;
    if (status_eq == 1) {
        return 0;
    }

    if (self->movie == 0) {
        return 0;
    }

    return fn_12_2FA90(self->value_40, 2, self->movie, 0) ? -1 : 0;
}
