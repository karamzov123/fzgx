#include "types.h"

typedef struct Fn8006A9B4Entry {
    u32 unk_0;
    u32 unk_4;
    u32 unk_8;
} Fn8006A9B4Entry;

typedef struct Fn8006A9B4Data {
    u32 unk_0;
    Fn8006A9B4Entry *unk_4;
    u32 unk_8;
    u32 unk_C;
    u32 unk_10;
    u32 unk_14;
    u32 unk_18;
} Fn8006A9B4Data;

s32 fn_8006A554(void);

s32 fn_8006A9B4(Fn8006A9B4Data *data) {
    s32 idx = fn_8006A554();
    Fn8006A9B4Entry *tbl = data->unk_4;

    if (idx < 0 || ((tbl[idx].unk_0 & 0xFF000000) == 0 ? 0 : 1) == 0) {
        return 0;
    }
    data->unk_18 = idx;
    return 1;
}
