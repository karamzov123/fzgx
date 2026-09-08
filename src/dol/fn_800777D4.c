#include "types.h"

extern f32 lbl_801A74A8;
extern f32 lbl_801A74AC;
extern struct fn_800777D4_lbl_8019F130 lbl_8019F130;
extern u32 fn_80070D94(void);
extern u32 fn_80070E00(void);
extern u32 fn_80072778(void);
extern u32 fn_80074A8C(f32, f32, f32, f32);
extern u32 fn_80074BC4(f32, f32, f32, f32);
extern u32 fn_80075240(void);
extern u32 fn_80077848(void);
extern u32 lbl_801A6D90;
extern u32 lbl_801A6D94;
extern u32 lbl_801A6D98;

struct fn_800777D4_lbl_8019F130 {
    u8 pad_0[0xC];
    f32 unk_C;
};

void fn_800777D4(void) {
    fn_80072778();
    lbl_8019F130.unk_C = lbl_801A74A8;
    fn_80074A8C(lbl_801A74A8, lbl_801A74A8, lbl_801A74A8, lbl_801A74A8);
    fn_80074BC4(lbl_801A74AC, lbl_801A74AC, lbl_801A74AC, lbl_801A74AC);
    lbl_801A6D90 = 0;
    lbl_801A6D98 = 0;
    lbl_801A6D94 = 0;
    fn_80075240();
    fn_80077848();
    fn_80070E00();
    fn_80070D94();
}
