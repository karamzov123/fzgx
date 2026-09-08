#include "types.h"

extern u32 lbl_1_data_2A7E0[62];
extern u32 fn_1_9AD54(void);
extern u32 fn_1_9D260(void);
extern void fn_1_150D3C(u32 arg0, u32 arg1);
extern void fn_1_150654(void);
extern void fn_1_151054(u32 arg);

void fn_1_1505B4(void) {
    u32 value;
    u32 result;

    value = *(u32*)((u8*)lbl_1_data_2A7E0 + 0x3c);
    fn_1_9AD54();
    result = fn_1_9D260();
    fn_1_150D3C(value, result);
    fn_1_150654();
    fn_1_151054(value);
}
