#include "types.h"

extern u32 lbl_801A6D08;
extern u32 lbl_801A6D0C;
extern void fn_80071C64(void);
extern void fn_80071C8C(void);

void fn_80070E00(void) {
    lbl_801A6D0C = (u32)fn_80071C64;
    lbl_801A6D08 = (u32)fn_80071C8C;
}
