#include "types.h"

extern volatile u32 lbl_8009232C[]; /* Volatile preserves the retail table read. */
extern u32 fn_80057728(void);
extern s32 lbl_8018B2A0[];
extern u32 lbl_8018B2A4[];
extern u32 memset(u32, u32, u32);
extern u32 fn_800576DC(void);

void fn_80058680(void) {
    lbl_8009232C[0];
    fn_80057728();
    if (lbl_8018B2A0[0] == 0) {
        memset((u32)&lbl_8018B2A4, 0, 0x4000);
    }
    lbl_8018B2A0[0]++;
    fn_800576DC();
}
