#include "types.h"

extern void fn_1_105744(void);
extern u32 lbl_1_bss_3BE0;
extern u8 lbl_1_data_3F244[64];
extern void fn_1_106DB4(void);
extern void fn_1_9D0EC(u32, u32, void*, void*);
extern void fn_1_103AA8(void);
extern u32 lbl_1_bss_85288[2];

// fn_1_102FD0: Initialize some game state
void fn_1_102FD0(void) {
    u32 base;
    u32 field1;
    u32 field2;
    
    fn_1_105744();
    
    base = lbl_1_bss_3BE0;
    field1 = *(u32*)(base + 0x54);
    field2 = *(u32*)(base + 0x48);
    
    fn_1_9D0EC(field1, field2, &lbl_1_data_3F244[0], &fn_1_106DB4);
    fn_1_103AA8();
    
    lbl_1_bss_85288[0] = -1;
    lbl_1_bss_85288[1] = -1;
}
