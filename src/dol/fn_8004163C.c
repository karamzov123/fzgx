#include "types.h"

extern u32 fn_8004551C(u32);

struct fn_8004163C_Arg0 {
    u8 pad_0[0x4];
    u32 unk_4;
};

void fn_8004163C(struct fn_8004163C_Arg0 *arg0) {
    fn_8004551C(arg0->unk_4);
}
