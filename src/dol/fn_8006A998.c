#include "types.h"


struct fn_8006A998_Arg0 {
    u32 unk_0;
    u32 unk_4;
};

u32 fn_8006A998(struct fn_8006A998_Arg0 *arg0) {
    return (*(u32 *)((u8 *)arg0->unk_0 + 0) + arg0->unk_4);
}
