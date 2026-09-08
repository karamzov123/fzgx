#include "types.h"

extern u8 lbl_1_data_1D62C[148];
extern void *lbl_801A6410;
extern void fn_1_46B4(void *, void *, u8 *, s32);

struct Fn1_61760Object {
    u8 _pad38[0x38];
    void *field_38;
};

void fn_1_61760(struct Fn1_61760Object *object) {
    if (object->field_38 != 0) {
        fn_1_46B4(lbl_801A6410, object->field_38, lbl_1_data_1D62C, 0x16c1);
    }
}
