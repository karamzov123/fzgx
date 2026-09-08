#include "types.h"

extern s16 camera_get_mode(void);
extern u32 fn_1_5910(void);
extern s32 fn_1_6EC0(u8 value);
extern u32 fn_1_864E8(s32 value);
extern s32 fn_1_7000(void);
extern void fn_1_681C(u8 value, void *out);
extern s32 fn_1_1FB80(void *value, s32 mode);

s32 fn_1_D3E90(void) {
    s16 value;
    u8 data[8];

    value = camera_get_mode();
    if (value != -1 && fn_1_6EC0((u8)fn_1_5910()) != 0 &&
        (fn_1_864E8((s16)value) & 0x20) != 0) {
        return 1;
    }

    if (fn_1_7000() != 0) {
        return 1;
    }

    fn_1_681C((u8)fn_1_5910(), data);
    return fn_1_1FB80(data, 1);
}
