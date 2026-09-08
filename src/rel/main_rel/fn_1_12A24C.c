#include "types.h"

extern s32 fn_1_86678(s32 arg);

s8 fn_1_12A24C(s8 arg) {
    if ((s8)fn_1_86678((s32)arg) == -1) {
        return -1;
    }
    return arg;
}
