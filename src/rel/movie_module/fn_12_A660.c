#include "types.h"

extern u32 lbl_12_bss_4E60[6];

typedef void (*MovieCallback)(void *);

typedef struct MovieState {
    u8 pad_0[0x220];
    MovieCallback callback;
    void *callback_arg;
    s32 field_228;
} MovieState;

s32 fn_12_A660(MovieState *movie, s32 state) {
    if (movie == 0) {
        lbl_12_bss_4E60[2] = state;
        if (state != 0 && lbl_12_bss_4E60[0] != 0) {
            ((MovieCallback)lbl_12_bss_4E60[0])((void *)lbl_12_bss_4E60[1]);
        }
    } else {
        movie->field_228 = state;
        if (state != 0 && movie->callback != 0) {
            movie->callback(movie->callback_arg);
        }
    }
    return state;
}
