#include "types.h"

extern s32 lbl_80187110[];
extern u32 lbl_80187330[];
extern u32 memset(u32, u32, u32);
extern u32 fn_80053BB4(void);

void fn_8005190C(void) {
    if (--lbl_80187110[0] == 0) {
        memset((u32)&lbl_80187330, 0, 0x40);
        fn_80053BB4();
    }
}
