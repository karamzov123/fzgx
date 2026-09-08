#include "types.h"

typedef struct {
    u8 unk[0x38];
    void *field38;
} FZeroObject;

extern u8 lbl_1_data_1D62C[148];
extern u32 lbl_801A6410;
extern void fn_1_46B4(u32 value, void *field, u8 *data, s32 code);

void fn_1_60170(FZeroObject *object) {
    void *field;

    field = object->field38;
    if (field != 0) {
        fn_1_46B4(lbl_801A6410, field, lbl_1_data_1D62C, 0x1261);
    }
}
