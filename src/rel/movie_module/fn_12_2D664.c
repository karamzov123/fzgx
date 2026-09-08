#include "types.h"

extern int fn_12_24990(u8 *base, u8 *out);
extern int fn_12_24A88(int value, u32 code);

typedef struct MovieData {
    u32 values[16];
} MovieData;

int fn_12_2D664(u8 *base, u8 *out) {
    if (fn_12_24990(base, out) != 0) {
        return fn_12_24A88(0, 0xff000114);
    } else {
        *(MovieData *)out = *(MovieData *)(base + 0x90c);
        return 0;
    }
}
