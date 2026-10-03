#include "types.h"

extern u32 fn_800744F8(void *, u32);
extern void fn_80008204(void *);
extern u32 lbl_801A6408;
extern u32 lbl_801A6728;

static inline int byte_equal(u8 *p) {
    u32 a;
    u32 b;
    /* Volatile byte reads preserve the retail input-before-cache load order. */
    b = *(volatile u8 *)p;
    a = ((volatile u8 *)&lbl_801A6408)[0]; /* volatile preserves the second ordered byte read. */
    return a == b;
}

void fn_80007AB4(u8 *arg0) {
    u32 loc_8[2];
    u8 b0;

    if (byte_equal(arg0)) {
        /* Goto shares the final flag store with the unchanged-value path. */
        if (((u8 *)&lbl_801A6408)[1] == arg0[1] &&
            ((u8 *)&lbl_801A6408)[2] == arg0[2] &&
            ((u8 *)&lbl_801A6408)[3] == arg0[3]) goto done; /* goto shares the final flag store. */
    }
    {
        loc_8[1] = *(u32 *)arg0;
        ((u8 *)&lbl_801A6408)[0] = ((u8 *)&loc_8[1])[0];
        ((u8 *)&lbl_801A6408)[1] = ((u8 *)&loc_8[1])[1];
        ((u8 *)&lbl_801A6408)[2] = ((u8 *)&loc_8[1])[2];
        ((u8 *)&lbl_801A6408)[3] = ((u8 *)&loc_8[1])[3];
        loc_8[0] = lbl_801A6408;
        fn_800744F8(loc_8, 0x1000000 - 1);
        fn_80008204(&lbl_801A6408);
    }
done:
    lbl_801A6728 = 1;
}
