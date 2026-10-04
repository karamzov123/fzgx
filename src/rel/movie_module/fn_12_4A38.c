#include "types.h"

void fn_12_4A38(f64 *arg0, f64 *arg1, f64 (*arg2)[8]) {
    f64 tmp[64];
    s32 i;
    s32 j;
    s32 k;
    f64 s;

    {
    s32 fzgx_loop_i_142;
for (fzgx_loop_i_142 = 0; fzgx_loop_i_142 < 8; fzgx_loop_i_142++) {
        for (j = 0; j < 8; j++) {
            s = 0.0;
            for (k = 0; k < 8; k++) {
                s += arg2[k][j] * arg0[fzgx_loop_i_142 * 8 + k];
            }
            tmp[fzgx_loop_i_142 * 8 + j] = s;
        }
    }
    i = fzgx_loop_i_142;
}

    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            s = 0.0;
            for (k = 0; k < 8; k++) {
                s += arg2[k][j] * tmp[i + k * 8];
            }
            arg1[j * 8 + i] = s;
        }
    }
}
