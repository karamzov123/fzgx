#include "types.h"

extern u32 lbl_1_data_2A7E0[62];
extern void fn_1_9AD88(void);
extern u32 fn_1_9D260(void);
extern void fn_1_150ED0(u32 arg0, u32 arg1);

// fn_1_150608: Load data value, call two setup functions, then call main function with value and result
void fn_1_150608(void) {
    u32 value;
    u32 result;
    
    value = *(u32*)((u8*)lbl_1_data_2A7E0 + 0x3c);
    fn_1_9AD88();
    result = fn_1_9D260();
    fn_1_150ED0(value, result);
}
