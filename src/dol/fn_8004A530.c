#include "types.h"


struct fn_8004A530_Arg0 {
    u8 pad_0[0x2];
    u8 unk_2;
};

s32 fn_8004A530(struct fn_8004A530_Arg0 *arg0) {
    return (s8)arg0->unk_2;
}
