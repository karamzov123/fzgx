#include "types.h"

typedef struct {
    u8 _pad[0x4c];
    u8 field_4c;
} Fn15328State;

extern Fn15328State *lbl_1_bss_F68;
extern u8 lbl_1_data_3314;

void fn_1_5328(u8 value) {
    lbl_1_data_3314 = value;
    if (lbl_1_bss_F68 != 0) {
        lbl_1_bss_F68->field_4c = value;
    }
}
