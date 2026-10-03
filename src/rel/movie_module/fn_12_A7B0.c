#include "types.h"
#include "sofdec/sj.h"

extern s32 fn_12_CAB4(void *movie);
extern s32 fn_12_A660(void *movie, s32 code);
extern int MPV_GoNextDelimSj(SJ *sj);
extern int MPV_MoveChunk(SJ *sj, int count, int size);

#pragma opt_propagation off
void fn_12_A7B0(void *movie, SJ *sj) {
    s32 tmp_fn_12_A660;
    void *movie_local = movie;
    u32 state;
    s32 lab_t1;

    if (fn_12_CAB4(movie_local)) {
        lab_t1 = 0xFF03020A;
        tmp_fn_12_A660 = fn_12_A660(0, lab_t1);
        tmp_fn_12_A660;
        return;
    }

    state = 0xFF030305;
    for (;;) {
        int r = MPV_GoNextDelimSj(sj);

        if (r == 0) {
            break;
        }
        if (r & 0xcc) {
            state = 0;
            break;
        }
        if (MPV_MoveChunk(sj, 1, 4) != 4) {
            break;
        }
    }

    lab_t1 = state;
    tmp_fn_12_A660 = fn_12_A660(movie_local, lab_t1);
    tmp_fn_12_A660;
}
#pragma opt_propagation reset
