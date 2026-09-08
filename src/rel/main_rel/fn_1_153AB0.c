#include "types.h"

extern u32 lbl_1_data_2A7E0[62];
extern void fn_1_153C60(u32 value);
extern void fn_1_1540B0(u32 value);
extern void fn_1_9AD88(void);

void fn_1_153AB0(void) {
    u32 value = lbl_1_data_2A7E0[15];

    fn_1_153C60(value);
    fn_1_1540B0(value);
    fn_1_9AD88();
}
