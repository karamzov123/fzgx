#include "types.h"

extern u32 lbl_1_bss_7B188[2];
extern u32 lbl_1_data_3E358[3];

// fn_1_EB080: Check BSS flag, conditionally copy data field, store parameter.
void fn_1_EB080(int param) {
    u8* data_ptr = (u8*)lbl_1_data_3E358;
    
    if ((int)lbl_1_bss_7B188[0] == 0) {
        return;
    }
    
    if (param == 11) {
        *(u32*)(data_ptr + 0x1e0) = *(u32*)(data_ptr + 0x1d4);
    }
    
    *(u32*)(data_ptr + 0x1d8) = param;
}
