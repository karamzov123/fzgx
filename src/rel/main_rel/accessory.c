#include "types.h"

/* fzgx:begin fn_1_1154D0 */
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
/* fzgx:end fn_1_1154D0 */

/* fzgx:begin fn_1_115B58 */
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
} Fn115B58Entry;

typedef struct {
    u8 _pad00[0x18];
    u32 count;
    u8 _pad1c[8];
    Fn115B58Entry *entries;
} Fn115B58Object;

void fn_1_115B58(Fn115B58Object *obj) {
    f32 inactive_value;
    f32 active_value;
    u32 i;
    Fn115B58Entry *entry;

    if (obj != 0) {
        fn_1_10846C();
        active_value = lbl_1_rodata_7B1C;
        inactive_value = lbl_1_rodata_7B20;
        entry = obj->entries;
        for (i = 0; i < obj->count; i++, entry++) {
            if ((entry->active & 1) != 0) {
                entry->active = 1;
                entry->field40 = active_value;
            } else {
                entry->active = 0;
                entry->field40 = inactive_value;
                entry->field34 = inactive_value;
                entry->field3c = inactive_value;
            }
        }
    }
}
/* fzgx:end fn_1_115B58 */

/* fzgx:begin fn_1_1166EC */
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
} Fn1166ECEntry;

typedef struct {
    u8 _pad00[0x18];
    u32 count;
    u8 _pad1c[8];
    Fn1166ECEntry *entries;
} Fn1166ECObject;

void fn_1_1166EC(Fn1166ECObject *obj) {
    f32 inactive_value;
    f32 active_value;
    u32 i;
    Fn1166ECEntry *entry;

    if (obj != 0) {
        fn_1_10846C();
        active_value = lbl_1_rodata_7B1C;
        inactive_value = lbl_1_rodata_7B20;
        entry = obj->entries;
        for (i = 0; i < obj->count; i++, entry++) {
            if ((entry->active & 1) != 0) {
                entry->active = 1;
                entry->field40 = active_value;
            } else {
                entry->active = 0;
                entry->field40 = inactive_value;
                entry->field34 = inactive_value;
                entry->field3c = inactive_value;
            }
        }
    }
}
/* fzgx:end fn_1_1166EC */

/* fzgx:begin fn_1_128DD8 */
extern u8 lbl_1_data_405C0[8];

u8 fn_1_128DD8(u8 value) {
    u8 i;

    i = 0;
    while (i < 6) {
        if (lbl_1_data_405C0[i] == value) {
            return i;
        }
        i++;
    }
    return i;
}
/* fzgx:end fn_1_128DD8 */

/* fzgx:begin fn_1_128E8C */
extern u32 lbl_801A66A0;

typedef struct {
    u8 field_0;
    u8 field_1;
    u16 field_2;
    u8 field_4;
    u8 pad_5[3];
    u8 field_8;
    u8 field_9;
    u16 field_a;
    u8 field_c[3];
    u8 field_f;
    u8 field_10;
    u8 field_11;
    u8 field_12;
    u8 pad_13;
    u16 field_14;
    u8 pad_16[2];
    u16 field_18;
} FnData;

void fn_1_128E8C(u32 unused, FnData *data) {
    data->field_0 = 1;
    data->field_1 = 2;
    data->field_2 = 3;
    data->field_4 = 0;
    data->field_8 = lbl_801A66A0 % 255;
    data->field_9 = 1;
    data->field_f = 2;
    data->field_a = 1234;
    data->field_11 = 5;
    data->field_12 = 6;
    data->field_14 = 7;
    data->field_10 = 4;
    data->field_18 = 982;
}
/* fzgx:end fn_1_128E8C */

/* fzgx:begin fn_1_12A24C */
extern s32 fn_1_86678(s32 arg);

s8 fn_1_12A24C(s8 arg) {
    if ((s8)fn_1_86678((s32)arg) == -1) {
        return -1;
    }
    return arg;
}
/* fzgx:end fn_1_12A24C */

/* fzgx:begin fn_1_12A2B8 */
extern u32 lbl_1_bss_897A4;

void fn_1_12A2B8(u32 value) {
    lbl_1_bss_897A4 = value;
}
/* fzgx:end fn_1_12A2B8 */
