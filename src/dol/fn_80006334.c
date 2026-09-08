#include "types.h"


struct fn_80006334_Arg0 {
    u8 unk_0;
};

u32 fn_80006334(struct fn_80006334_Arg0 *arg0) {
    return ((arg0->unk_0 >> 7) & 0x1);
}
