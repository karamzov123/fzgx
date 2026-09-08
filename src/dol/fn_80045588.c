#include "types.h"


struct fn_80045588_Arg0 {
    u8 pad_0[0xE];
    u8 unk_E;
};

s32 fn_80045588(struct fn_80045588_Arg0 *arg0) {
    return (s8)arg0->unk_E;
}
