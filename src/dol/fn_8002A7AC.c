#include "types.h"

extern struct fn_8002A7AC_lbl_80177960 lbl_80177960;

struct fn_8002A7AC_lbl_80177960_0_E272 {
    u8 pad_0[0x10C];
    u32 unk_10C;
};
struct fn_8002A7AC_lbl_80177960 {
    struct fn_8002A7AC_lbl_80177960_0_E272 unk_0[1];
};

u32 fn_8002A7AC(u32 arg0) {
    return lbl_80177960.unk_0[arg0].unk_10C;
}
