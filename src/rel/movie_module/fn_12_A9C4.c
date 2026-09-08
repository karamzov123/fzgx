#include "types.h"

extern int fn_12_CAB4(void *movie, void *out_a, void *out_b);
extern int fn_12_A660(int arg0, u32 arg1);

int fn_12_A9C4(void *movie, void *out_a, void *out_b) {
    if (fn_12_CAB4(movie, out_a, out_b)) {
        return fn_12_A660(0, 0xff03020e);
    }

    *(u32 *)out_a = *(u32 *)((u8 *)movie + 0x284);
    *(u32 *)out_b = *(u32 *)((u8 *)movie + 0x288);
    return 0;
}
