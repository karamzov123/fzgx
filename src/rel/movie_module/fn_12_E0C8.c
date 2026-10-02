#include "types.h"

typedef struct {
    unsigned int field_0;
    unsigned int field_4;
    unsigned int field_8;
    unsigned int field_C;
} fn_12_E0C8_struct;

extern const fn_12_E0C8_struct lbl_12_rodata_928;

void fn_12_E0C8(fn_12_E0C8_struct *param) {
    *param = lbl_12_rodata_928;
}
