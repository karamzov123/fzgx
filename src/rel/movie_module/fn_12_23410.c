#include "types.h"

extern u8 lbl_12_bss_7250[2048];
extern u8 lbl_12_rodata_A18[56];
extern void * memcpy(void *, const void *, u32);

typedef struct MovieState {
    u8 unk_00[0x0c];
    u8 *unk_0c;
    u8 unk_10[0x18];
    u8 unk_28;
    u8 unk_29[3];
    u32 unk_2c;
} MovieState;

struct Sig_fn_12_215F4_Arg2 {
    u32 unk_0;
};

extern s32 fn_12_215F4(u32, u32, struct Sig_fn_12_215F4_Arg2 *);

static inline int find_entry(u8 *data, s32 size, MovieState *st) {
    s32 n;
    u8 *p;
    struct Sig_fn_12_215F4_Arg2 arg;

    n = (size < 0x800) ? size : 0x800;
    memcpy(lbl_12_bss_7250, data, (u32)n);
    p = lbl_12_bss_7250;
    while (n > 0) {
        if (fn_12_215F4((u32)p, (u32)n, &arg)) {
            st->unk_0c = lbl_12_rodata_A18;
            st->unk_28 = p[7];
            st->unk_2c = (p[8] << 24) | (p[9] << 16) | (p[10] << 8) | p[11];
            return 1;
        }
        p += 4;
        n -= 4;
    }
    return 0;
}

int fn_12_23410(u8 *data, s32 size, MovieState *st) {
    if (find_entry(data, size, st)) {
        return 1;
    }
    if (find_entry(data + 2, size - 2, st)) {
        return 1;
    }
    if (find_entry(data + 1, size - 1, st)) {
        return 1;
    }
    if (find_entry(data + 3, size - 3, st)) {
        return 1;
    }
    return 0;
}
