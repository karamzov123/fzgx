#include "types.h"

typedef struct {
    u8 _pad[0x4b];
    u8 field_4b;
} Fn15300State;

extern Fn15300State *lbl_1_bss_F68;

u8 fn_1_5300(void) {
    u32 value;

    if (lbl_1_bss_F68 != 0) {
        value = lbl_1_bss_F68->field_4b;
    } else {
        value = 0;
    }
    return (u8)value;
}
