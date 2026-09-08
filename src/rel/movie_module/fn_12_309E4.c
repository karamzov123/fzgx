#include "types.h"


struct fn_12_309E4_Arg0 {
    u8 pad_0[0x158];
    f64 unk_158;
    f64 unk_160;
};
struct fn_12_309E4_Arg1 {
    f64 unk_0;
    f64 unk_8;
};

f64 fn_12_309E4(struct fn_12_309E4_Arg0 *arg0, struct fn_12_309E4_Arg1 *arg1) {
    f64 v0;
    f64 v1;
    v0 = arg1->unk_0;
    v1 = arg1->unk_8;
    arg0->unk_158 = v0;
    arg0->unk_160 = v1;
    return v0;
}
