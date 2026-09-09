#include "types.h"

extern u8 lbl_801A36E8[];

void* TRKGetBuffer(s32 index) {
    void* result = 0;
    if (index >= 0 && index < 3) {
        result = (void*)(lbl_801A36E8 + index * 0x890);
    }
    return result;
}
