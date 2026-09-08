#include "types.h"

typedef struct {
    u8 _pad[0x4b];
    u8 field_4b;
} Fn152E0State;

extern Fn152E0State *lbl_1_bss_F68;
extern u8 lbl_1_bss_F76;

void fn_1_52E0(u8 value) {
    lbl_1_bss_F76 = value;
    if (lbl_1_bss_F68 != 0) {
        lbl_1_bss_F68->field_4b = value;
    }
}
