#include "types.h"

typedef struct MovieState {
    u8 unk_00;
    u8 unk_01;
    u8 pad_02[0x06];
    void *unk_08;
    void *unk_0c;
    s32 unk_10;
    s32 unk_14;
    s32 unk_18;
} MovieState;

extern void fn_12_24970(void *state);
extern void fn_12_23BFC(void *arg0, void *arg1, MovieState *movie);
extern u8 lbl_12_rodata_BD8[56];
extern void fn_12_24950(void *state);

void fn_12_23E0C(void *arg0, void *arg1, MovieState *movie) {
    u8 state[8];

    fn_12_24970(state);
    fn_12_23BFC(arg0, arg1, movie);
    if (movie->unk_08 != 0 || movie->unk_0c != 0 || movie->unk_10 == -1) {
        movie->unk_00 = 1;
    }
    if (movie->unk_08 == lbl_12_rodata_BD8) {
        s32 width;
        s32 height;

        width = (movie->unk_14 + 15) / 16;
        height = (movie->unk_18 + 15) / 16;
        if (width * height > 0x654) {
            movie->unk_08 = 0;
        }
    }
    if (movie->unk_08 != 0 || movie->unk_0c != 0) {
        movie->unk_01 = 1;
    }
    fn_12_24950(state);
}
