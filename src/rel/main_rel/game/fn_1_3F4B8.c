#include "types.h"
#include "rel/main_rel/globals.h"
#include "rel/main_rel/game.h"

extern u32 lbl_1_bss_26C5C;

typedef struct lbl_1_bss_3C30_t {
    u8 pad_0[0x13f8];
    u16 unk_13F8;
    u16 unk_13FA;
    u16 unk_13FC;
    u16 unk_13FE;
    u16 unk_1400;
    u8 unk_1402;
    u8 pad_1403[0xb9];
} lbl_1_bss_3C30_t;

/* file-scope objects of the retail TU, in retail order: MWCC addresses them off one section base */
lbl_1_bss_3C30_t fzgx_obj_lbl_1_bss_3C30;
u32 fzgx_obj_lbl_1_bss_50EC[5];
u32 fzgx_obj_lbl_1_bss_5100;
u32 fzgx_obj_lbl_1_bss_5104[13];
u32 fzgx_obj_lbl_1_bss_5138[65];
u32 lbl_1_bss_523C[8];

#pragma section code_type ".fzgxpool"
static void fzgx_bss_layout(void) {
    volatile u8 s;  /* fzgx-allow: S2 layout primer sink: MWCC emits .bss objects in first-access order */
    s = *(u8 *)&fzgx_obj_lbl_1_bss_3C30;
    s = *(u8 *)&fzgx_obj_lbl_1_bss_50EC;
    s = *(u8 *)&fzgx_obj_lbl_1_bss_5100;
    s = *(u8 *)&fzgx_obj_lbl_1_bss_5104;
    s = *(u8 *)&fzgx_obj_lbl_1_bss_5138;
    s = *(u8 *)&lbl_1_bss_523C;
}
#pragma section code_type ".text"

void fn_1_3F4B8(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 arg5)
{
    fzgx_obj_lbl_1_bss_3C30.unk_13F8 = arg0 * 60;
    fzgx_obj_lbl_1_bss_3C30.unk_1402 = arg5;
    fzgx_obj_lbl_1_bss_3C30.unk_13FA = arg1 * 60;
    fzgx_obj_lbl_1_bss_3C30.unk_13FC = arg2 * 60;
    fzgx_obj_lbl_1_bss_3C30.unk_13FE = arg3 * 60;
    fzgx_obj_lbl_1_bss_3C30.unk_1400 = arg4 * 60;
    lbl_1_bss_26C5C = 1;
}
