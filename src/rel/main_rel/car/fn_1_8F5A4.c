#include "types.h"

typedef struct {
    u8 pad_0[0x148];
    u8 pad_148[0x20];
    u8 pad_168[0x40];
    u8 pad_1a8[0x20];
    u8 pad_1c8[0x20];
    u8 pad_1e8[0x20];
    u8 entries[0x14][0x20];
} Fn1_8F5A4_Object;

extern void fn_1_961F0(void *obj);
extern void fn_1_968FC(Fn1_8F5A4_Object *obj);

void fn_1_8F5A4(Fn1_8F5A4_Object *obj) {
    u32 i;

    fn_1_961F0(obj->pad_148);
    fn_1_961F0(obj->pad_168);
    fn_1_961F0(obj->pad_1a8);
    fn_1_961F0(obj->pad_1c8);
    fn_1_961F0(obj->pad_1e8);
    for (i = 0; i < 0x14; i++) {
        fn_1_961F0(obj->entries[i]);
    }
    fn_1_968FC(obj);
}
