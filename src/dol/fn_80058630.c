#include "types.h"

extern u32 fn_80057728(void);
extern u32 lbl_8018B2A0[];
extern u32 lbl_8018B2A4[];
extern u32 memset(u32, u32, u32);
extern u32 fn_800576DC(void);

void fn_80058630(void) {
    fn_80057728();
    if (--lbl_8018B2A0[0] == 0) {
        memset((u32)&lbl_8018B2A4, 0, 0x4000);
    }
    fn_800576DC();
}
