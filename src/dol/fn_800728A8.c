#include "types.h"
#include "dol/globals.h"

extern void fn_80037B14(s32, s32, s32, s32);

void fn_800728A8(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    if (2 == arg0) {
        if (*(s32 *)(lbl_801A6D38 + 0x1c) == 2 &&
            *(s32 *)(lbl_801A6D38 + 0x28) == arg3) {
            return;
        }

        fn_80037B14(2, arg1, arg2, arg3);
        *(s32 *)(lbl_801A6D38 + 0x1c) = 2;
        *(s32 *)(lbl_801A6D38 + 0x20) = arg1;
        *(s32 *)(lbl_801A6D38 + 0x24) = arg2;
        *(s32 *)(lbl_801A6D38 + 0x28) = arg3;
        return;
    }

    if (*(s32 *)(lbl_801A6D38 + 0x1c) == arg0 &&
        *(s32 *)(lbl_801A6D38 + 0x20) == arg1 &&
        *(s32 *)(lbl_801A6D38 + 0x24) == arg2) {
        return;
    }

    fn_80037B14(arg0, arg1, arg2, arg3);
    *(s32 *)(lbl_801A6D38 + 0x1c) = arg0;
    *(s32 *)(lbl_801A6D38 + 0x20) = arg1;
    *(s32 *)(lbl_801A6D38 + 0x24) = arg2;
    *(s32 *)(lbl_801A6D38 + 0x28) = arg3;
}
