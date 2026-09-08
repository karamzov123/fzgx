#include "types.h"


struct fn_12_30A0C_Arg0 {
    u8 pad_0[0x138];
    f64 unk_138;
    f64 unk_140;
};
struct fn_12_30A0C_Arg1 {
    f64 unk_0;
    f64 unk_8;
};

f64 fn_12_30A0C(struct fn_12_30A0C_Arg0 *arg0, struct fn_12_30A0C_Arg1 *arg1) {
    f64 v0;
    f64 v1;
    v0 = arg1->unk_0;
    v1 = arg1->unk_8;
    arg0->unk_138 = v0;
    arg0->unk_140 = v1;
    return v0;
}
