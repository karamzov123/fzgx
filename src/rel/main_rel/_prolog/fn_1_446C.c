#include "types.h"

extern u32 lbl_1_data_2C70;
extern void fn_80008E84(u32 value);

void fn_1_446C(void) {
    fn_80008E84(lbl_1_data_2C70);
    lbl_1_data_2C70 = (u32)-1;
}
