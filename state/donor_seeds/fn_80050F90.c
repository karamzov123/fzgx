#include "types.h"

// Donor seed for fn_80050F90 (addr 0x80050F90)
// Extracted from adxt_80050F90.c (original name: ADXF_StreamTeardown)

int fn_80050F90(StreamObj* obj, char* arg4, char* arg5)
{
    int r28 = obj->unk388;
    int i;

    if (obj->unk348 == 12) {
        return 0;
    }

    lbl_80187118[0] = adxtNullCallback();
    ADXF_Stop(obj->unk34c, &obj->unk384, &obj->unk3b8, &obj->unk5b8, obj->unk348 >> 2, obj->unk0);

    lbl_80187118[3] = adxtNullCallback();
    for (i = 0; i < 3; i++) {
        fn_80053DB4(obj->unk350, (char*)obj->unk0 + i * 0x80, 0, arg4 + i * 0x40);
    }

    lbl_80187118[4] = adxtNullCallback();
    if (r28 >= 2) {
        for (i = 0; i < 3; i++) {
            fn_80053DB4(obj->unk350, (char*)obj->unk0 + 0x180 + i * 0x80, 1, arg5 + i * 0x40);
        }
    }

    lbl_80187118[5] = adxtNullCallback();
    obj->unk348++;
    return 0x60;
}
