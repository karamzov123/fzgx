#include "types.h"

struct fn_80045FA4_lbl_8017A288 {
    u32 unk_0;    u8 pad_4[0x10];
};

extern struct fn_80045FA4_lbl_8017A288 lbl_8017A288[];
extern u32 fn_8004E4AC(void);
extern u32 lbl_8017A29C[];
extern u32 memset(u32, u32, u32);

void fn_80045FA4(void) {
    u32 v0;
    fn_8004E4AC();
    v0 = lbl_8017A288[0].unk_0;
    lbl_8017A288[0].unk_0 = (v0 + 1);
    memset((u32)&lbl_8017A29C, 0, 3456);
}
