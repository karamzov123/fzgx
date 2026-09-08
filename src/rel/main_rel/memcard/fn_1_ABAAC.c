#include "types.h"

extern void fn_80083DB0(void *dest, void *src);
extern void fn_80083D40(void *arg, void *data);
extern s32 fn_8002EBD4(u8 byte_val, void *arg1, void *arg2);
extern u8 lbl_1_data_3C7C4[216];

typedef struct {
    u8 pad[0x2];
    u16 field_0x2;
    s32 field_0x4;
} ResultStruct;

typedef struct {
    u8 byte_0;
    u8 pad_0x1[0xf];
    u8 data_0x10[0x14];
    ResultStruct *ptr_0x24;
} ArgStruct;

void fn_1_ABAAC(ArgStruct *arg) {
    u8 local[0x40];

    fn_80083DB0(local, (u8 *)arg->ptr_0x24 + 0x98);
    fn_80083D40(local, lbl_1_data_3C7C4);
    arg->ptr_0x24->field_0x4 =
        fn_8002EBD4(arg->byte_0, local, arg->data_0x10);
    if (arg->ptr_0x24->field_0x4 != -1) {
        arg->ptr_0x24->field_0x2 = 0;
    }
}
