#include "types.h"


struct fn_8004A544_Arg0 {
    u8 pad_0[0x28];
    u32 unk_28;
};

s32 fn_8004A544(struct fn_8004A544_Arg0 *arg0, u32 arg1) {
    arg0->unk_28 = arg1;
    return 1;
}
