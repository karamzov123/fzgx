#include "types.h"

extern struct fn_16_6CD8_lbl_16_rodata_58 lbl_16_rodata_58;
extern u32 fn_1_133DBC(u32);
extern u32 fn_1_49410(void);
extern u32 fn_1_4955C(f32, f32);
extern u32 fn_1_495B0(u32);
extern u32 fn_1_4965C(u32);
extern u32 fn_1_496FC(f32, f32);
extern u32 fn_1_4AE0C(u32, ...);
extern u32 lbl_16_data_83524;

struct fn_16_6CD8_lbl_16_rodata_58 {
    u8 pad_0[0x20];
    f32 unk_20;
    u8 pad_24[0x11C];
    f32 unk_140;
    u8 pad_144[0x5C];
    f32 unk_1A0;
};

void fn_16_6CD8(void) {
    struct fn_16_6CD8_lbl_16_rodata_58 *p_lbl_16_rodata_58;
    f32 v0;
    p_lbl_16_rodata_58 = (struct fn_16_6CD8_lbl_16_rodata_58 *)&lbl_16_rodata_58;
    fn_1_49410();
    fn_1_495B0(0x80000000);
    fn_1_4965C(2);
    v0 = p_lbl_16_rodata_58->unk_20;
    fn_1_4955C(v0, v0);
    fn_1_496FC(p_lbl_16_rodata_58->unk_140, p_lbl_16_rodata_58->unk_1A0);
    fn_1_4AE0C((u32)&lbl_16_data_83524);
    fn_1_133DBC(0);
}
