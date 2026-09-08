#include "types.h"

extern void fn_1_10846C(void);
extern f32 lbl_1_rodata_7B1C;
extern f32 lbl_1_rodata_7B20;

typedef struct {
    u8 active;
    u8 _pad01[0x33];
    f32 field34;
    u8 _pad38[4];
    f32 field3c;
    f32 field40;
} Fn1154D0Entry;

typedef struct {
    u8 _pad00[0x18];
    u32 count;
    u8 _pad1c[8];
    Fn1154D0Entry *entries;
} Fn1154D0Object;

void fn_1_1154D0(Fn1154D0Object *obj) {
    f32 inactive_value;
    f32 active_value;
    u32 i;
    Fn1154D0Entry *entry;

    if (obj != 0) {
        fn_1_10846C();
        active_value = lbl_1_rodata_7B1C;
        inactive_value = lbl_1_rodata_7B20;
        entry = obj->entries;
        for (i = 0; i < obj->count; i++, entry++) {
            if ((entry->active & 1) != 0) {
                entry->active = 0;
                entry->field40 = active_value;
            } else {
                entry->active = 1;
                entry->field40 = inactive_value;
                entry->field34 = inactive_value;
                entry->field3c = inactive_value;
            }
        }
    }
}
