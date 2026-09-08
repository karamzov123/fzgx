#include "types.h"

extern u32 fn_800702E4(u32);
extern u32 fn_800791A4(void);
extern u32 lbl_801A66CC;

void fn_800057CC(void) {
    u32 t0;
    t0 = fn_800702E4(3);
    lbl_801A66CC = t0;
    fn_800791A4();
}
