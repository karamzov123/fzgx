#include "types.h"


struct fn_12_309D0_Arg0 {
    u8 pad_0[0x168];
    f64 unk_168;
    f64 unk_170;
};
struct fn_12_309D0_Arg1 {
    f64 unk_0;
    f64 unk_8;
};

f64 fn_12_309D0(struct fn_12_309D0_Arg0 *arg0, struct fn_12_309D0_Arg1 *arg1) {
    f64 v0;
    f64 v1;
    v0 = arg1->unk_0;
    v1 = arg1->unk_8;
    arg0->unk_168 = v0;
    arg0->unk_170 = v1;
    return v0;
}
