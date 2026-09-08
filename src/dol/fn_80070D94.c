#include "types.h"

extern u32 lbl_801A6D10;
extern u32 lbl_801A6D14;
extern u32 lbl_801A6D18;
extern u32 lbl_801A6D1C;
extern u32 lbl_801A6D20;
extern u32 lbl_801A6D24;
extern void fn_80071CB4(void);
extern void fn_80071CC0(void);
extern void fn_80071CE0(void);
extern void fn_80071CE8(void);
extern void fn_80071D0C(void);
extern void fn_80071D2C(void);

void fn_80070D94(void) {
    lbl_801A6D24 = (u32)fn_80071CB4;
    lbl_801A6D20 = (u32)fn_80071CC0;
    lbl_801A6D1C = (u32)fn_80071CE0;
    lbl_801A6D18 = (u32)fn_80071CE8;
    lbl_801A6D14 = (u32)fn_80071D0C;
    lbl_801A6D10 = (u32)fn_80071D2C;
}
