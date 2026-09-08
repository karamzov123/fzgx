#include "types.h"

typedef struct MovieObject {
    u8 pad_0[0x278];
    u32 value_278;
} MovieObject;

extern s32 fn_12_CAB4(MovieObject *movie, u32 *out);
extern s32 fn_12_A660(u32 arg0, u32 arg1);

s32 fn_12_AB00(MovieObject *movie, u32 *out) {
    if (fn_12_CAB4(movie, out)) {
        return fn_12_A660(0, 0xff03020d);
    }
    *out = movie->value_278;
    return 0;
}
