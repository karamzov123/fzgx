#include "types.h"


struct fn_8004BE90_Arg0 {
    u8 pad_0[0x6D];
    u8 unk_6D;
};

void fn_8004BE90(struct fn_8004BE90_Arg0 *arg0, u32 arg1) {
    arg0->unk_6D = arg1;
}
