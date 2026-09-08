#include "types.h"

typedef struct {
    u16 unk0;
    u32 unk4;
    u8 unk8;
    u8 pad9[3];
    u32 unkC;
    u32 unk10;
    u32 unk14;
    u32 unk18;
} FnData;

void fn_1_12C0EC(FnData *arg) {
    arg->unk18 = 0;
    arg->unk14 = 0;
    arg->unk10 = 0;
    arg->unkC = 0;
    arg->unk8 = 0;
    arg->unk4 = 0;
    arg->unk0 = 0;
}
