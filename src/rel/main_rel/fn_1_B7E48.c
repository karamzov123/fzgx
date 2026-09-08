#include "types.h"

extern u8 lbl_1_bss_716C8[320];

typedef struct {
    u8 pad0[8];
    u32 flags;
} FlagObject;

int fn_1_B7E48(void) {
    int result = 0;
    FlagObject *object = *(FlagObject **)(lbl_1_bss_716C8 + 0x24);

    if (object != 0 && (object->flags & 0x200) != 0) {
        result = 1;
    }

    object = *(FlagObject **)(lbl_1_bss_716C8 + 0xc4);
    if (object != 0 && (object->flags & 0x200) != 0) {
        result = 1;
    }

    return result;
}
