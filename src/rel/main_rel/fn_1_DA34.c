#include "types.h"

extern char lbl_1_data_4420[14];
extern char lbl_1_data_4430[15];

extern void fn_8000C49C(char *, s32, ...);

void fn_1_DA34(void) {
    fn_8000C49C(lbl_1_data_4420, 0x34, lbl_1_data_4430);
}
