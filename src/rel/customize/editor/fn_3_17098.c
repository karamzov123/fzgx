#include "types.h"
#include "rel/customize/editor.h"
#pragma opt_propagation off
/* file-scope objects of the retail TU, in retail order: MWCC addresses them off one section base */
u32 lbl_3_bss_A2410_fill_A2410;
u8 lbl_3_bss_A2410_4;
u8 lbl_3_bss_A2410_fill_A2415;
u16 lbl_3_bss_A2410_fill_A2416;
u32 lbl_3_bss_A2410_8;
u8 lbl_3_bss_A2410_C;
u8 lbl_3_bss_A2410_fill_A241D;
u16 lbl_3_bss_A2410_fill_A241E;
u16 lbl_3_bss_A2410_10;
u16 lbl_3_bss_A2410_12;
u16 lbl_3_bss_A2410_14;
s16 lbl_3_bss_A2410_16;
u32 lbl_3_bss_A2410_fill_A2428[2];
s16 lbl_3_bss_A2410_20;
u16 lbl_3_bss_A2410_fill_A2432;
u32 lbl_3_bss_A2410_fill_A2434;
u32 fzgx_obj_lbl_3_bss_A2438[7];
u32 fzgx_obj_lbl_3_bss_A2454;
u32 fzgx_obj_lbl_3_bss_A2460[2];
u32 fzgx_obj_lbl_3_bss_A2468[9];

#pragma opt_common_subs off
#pragma peephole off
void fn_3_17098(void) {
    u32 *b = (u32 *)&lbl_3_bss_A2410;
    s32 c = 0x40000000;
    b[2] = c;
{
    s8 i = 0;
    ((u8 *)b)[0xC] = i;
    b[1] = *(u32 *)((u8 *)&lbl_3_data_35C0 + (i * 4));
    ((u16 *)b)[8] = i;
    ((u16 *)b)[9] = i;
    ((u16 *)b)[10] = i;
    ((u16 *)b)[11] = 0xFF;
    ((u16 *)b)[16] = i;
}
}
#pragma peephole reset

#pragma opt_common_subs reset
