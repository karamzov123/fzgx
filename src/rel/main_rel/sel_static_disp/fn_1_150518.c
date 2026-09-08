#include "types.h"

extern u32 lbl_1_data_2A7E0[62];
extern void fn_1_9A508(u32 arg);
extern void fn_1_150C8C(u32 arg);
extern void fn_1_150F30(u32 arg);

typedef struct {
    u32 field_00;
    u8 pad_04[0x80];
    u32 field_84;
    u8 pad_88[0x269c];
    u32 field_2724;
} MainObject;

void fn_1_150518(void) {
    MainObject *object = (MainObject *)*(u32 *)((u8 *)lbl_1_data_2A7E0 + 0x3c);

    object->field_84 = 0;
    object->field_2724 = 0;
    fn_1_9A508((u32)lbl_1_data_2A7E0);
    fn_1_150C8C((u32)object);
    fn_1_150F30((u32)object);
    object->field_00 = 1;
}
