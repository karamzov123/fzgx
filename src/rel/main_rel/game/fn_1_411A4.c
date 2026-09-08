#include "types.h"

extern u32 lbl_1_data_66A0[8];
extern void fn_1_411D4(u32 index, u32 value);

// fn_1_411A4: Load array element by index and call fn_1_411D4 with both index and value.
void fn_1_411A4(u32 index) {
    fn_1_411D4(index, lbl_1_data_66A0[index]);
}
