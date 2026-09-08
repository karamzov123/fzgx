#include "types.h"

extern u32 lbl_80190B70[];
extern u32 lbl_80190B74[];

void fn_8005A614(u32 arg0, u32 arg1) {
    lbl_80190B70[0] = arg0;
    lbl_80190B74[0] = arg1;
}
