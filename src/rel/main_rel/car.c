#include "types.h"
#include "rel/main_rel/globals.h"

/* fzgx:begin fn_1_7F428 */
extern u32 lbl_1_bss_6D828;
extern s32 fn_1_3F854(u32 arg0);

u32 fn_1_7F428(u32 arg0) {
    s32 index;

    if (lbl_1_bss_6D828 == 0) {
        return 0;
    }

    index = fn_1_3F854(arg0);
    if ((u8)arg0 > index) {
        return 0;
    }

    return lbl_1_bss_6D828 + (u8)arg0 * (0x10000 + 0x440);
}
/* fzgx:end fn_1_7F428 */

/* fzgx:begin fn_1_7F934 */
extern void fn_1_7F658(void);

void fn_1_7F934(void) {
    fn_1_7F658();
}
/* fzgx:end fn_1_7F934 */

/* fzgx:begin fn_1_7FFF0 */
extern char lbl_1_data_2057C[22];
extern void fn_1_465D0(char *arg0, s32 arg1);

void fn_1_7FFF0(void) {
    fn_1_465D0(lbl_1_data_2057C, 1);
}
/* fzgx:end fn_1_7FFF0 */

/* fzgx:begin fn_1_8001C */
extern char lbl_1_data_20594[18];
extern u8 lbl_1_data_205A8[180];
extern void fn_1_465D0(void *arg0, s32 arg1);

void fn_1_8001C(void) {
    fn_1_465D0(lbl_1_data_20594, 1);
    fn_1_465D0(lbl_1_data_205A8, 1);
}
/* fzgx:end fn_1_8001C */

/* fzgx:begin fn_1_80058 */
extern void fn_80008BEC(void *dst, s32 value, u32 size);
extern void fn_80008BA8(void *dst, const void *src, u32 size);

void fn_1_80058(s8 arg0, u8 *arg1) {
    u8 data[0x20];

    fn_80008BEC(data, 0, 0x20);
    *(u32 *)(data + 4) = 0xF;
    data[8] = 1;
    fn_80008BA8(arg1, data, 0x20);
    *(u16 *)(arg1 + 2) = arg0;
}
/* fzgx:end fn_1_80058 */

/* fzgx:begin fn_1_800C4 */
extern void fn_80008BEC(void *dst, s32 value, u32 size);
extern u32 fn_1_8171C(s32 arg0);
extern void fn_80008BA8(void *dst, const void *src, u32 size);

void fn_1_800C4(u16 arg0, u8 *arg1) {
    u8 data[0x20];

    fn_80008BEC(data, 0, 0x20);
    *(u32 *)(data + 4) = fn_1_8171C(1);
    data[8] = 0x10;
    fn_80008BA8(arg1, data, 0x20);
    *(u16 *)(arg1 + 2) = arg0;
}
/* fzgx:end fn_1_800C4 */

/* fzgx:begin fn_1_80130 */
extern void fn_80008BEC(void *dst, s32 value, u32 size);
extern u32 fn_1_8171C(s32 arg0);
extern void fn_80008BA8(void *dst, const void *src, u32 size);

void fn_1_80130(u16 arg0, u8 *arg1) {
    u8 data[0x20];

    fn_80008BEC(data, 0, 0x20);
    *(u32 *)(data + 4) = fn_1_8171C(0);
    data[8] = 1;
    fn_80008BA8(arg1, data, 0x20);
    *(u16 *)(arg1 + 2) = arg0;
}
/* fzgx:end fn_1_80130 */

/* fzgx:begin fn_1_8019C */
#include "rel/main_rel/car.h"

extern u8 lbl_1_rodata_3968[232];
extern void fn_800712E4(void *arg0, u32 arg1, void *arg2);

typedef struct {
    u32 words[58];
} InitData;

void fn_1_8019C(void *arg0) {
    InitData data;

    data = *(InitData *)lbl_1_rodata_3968;
    fn_800712E4(arg0, lbl_1_bss_6D82C.unk_0, &data);
}
/* fzgx:end fn_1_8019C */

/* fzgx:begin fn_1_80270 */
extern void fn_1_80294(void *arg0, void *arg1, s32 arg2);

void fn_1_80270(void *arg0, void *arg1) {
    fn_1_80294(arg0, arg1, 0);
}
/* fzgx:end fn_1_80270 */

/* fzgx:begin fn_1_80F1C */
#include "rel/main_rel/car.h"

extern void fn_1_7F658(void *arg0);
extern void fn_1_12A734(s16 arg0);
extern void fn_80071718(void *arg0);

void fn_1_80F1C(s32 arg0, void *arg1) {
    fn_1_7F658(arg1);
    if ((s16)arg0 != -1) {
        fn_1_12A734((s16)arg0);
    }
    if (lbl_1_bss_6D82C.unk_0 != 0) {
        fn_80071718((void *)lbl_1_bss_6D82C.unk_0);
        lbl_1_bss_6D82C.unk_0 = 0;
    }
}
/* fzgx:end fn_1_80F1C */

/* fzgx:begin fn_1_8171C */
s32 fn_1_8171C(s32 value) {
    switch (value) {
    case 0:
        return 0xD;
    case 1:
        return 0xE;
    case 2:
        return 0xE;
    case 3:
        return 0xF;
    default:
        return -1;
    }
}
/* fzgx:end fn_1_8171C */

/* fzgx:begin fn_1_8176C */
extern u32 lbl_1_bss_6D850;

void fn_1_8176C(u32 value) {
    lbl_1_bss_6D850 = value;
}
/* fzgx:end fn_1_8176C */

/* fzgx:begin fn_1_81778 */
#include "rel/main_rel/car.h"

void fn_1_81778(void) {
    s32 offset;
    s32 i;
    i = 0;
    offset = 0;
    while (i < *(s8 *)&lbl_1_bss_6D84A) {
        *((u32 *)((u8 *)lbl_1_bss_6D838 + offset)) |= 0x8000;
        i++;
        offset += 0x620;
    }
}
/* fzgx:end fn_1_81778 */

/* fzgx:begin fn_1_817C0 */
#include "rel/main_rel/car.h"

extern Obj_1_bss_6D838_Target *lbl_1_bss_6D838;
extern u8 lbl_1_bss_6D84A;

void fn_1_817C0(void) {
    s32 offset;
    s32 i;

    i = 0;
    offset = 0;
    while (i < (s8)lbl_1_bss_6D84A) {
        u32 *value = (u32 *)((u8 *)lbl_1_bss_6D838 + offset);
        *value &= 0xFFFF7FFF;
        i++;
        offset += 0x620;
    }
}
/* fzgx:end fn_1_817C0 */

/* fzgx:begin fn_1_81808 */
#include "rel/main_rel/car.h"

extern void fn_80008BA8(void *arg0, void *arg1, s32 arg2);

void fn_1_81808(void *arg0) {
    fn_80008BA8(arg0, &lbl_1_data_2065C, 4);
}
/* fzgx:end fn_1_81808 */

/* fzgx:begin fn_1_81834 */
#include "rel/main_rel/car.h"

void fn_1_81834(u8 value, s8 index_a, s8 index_b) {
    if (index_a != -1) {
        *((u8 *)&lbl_1_data_2065C + index_a) = value;
    }
    *((u8 *)&lbl_1_data_20660 + index_b) = value;
}
/* fzgx:end fn_1_81834 */

/* fzgx:begin fn_1_81860 */
typedef struct {
    u8 pad0[2];
    s16 id;
    u8 pad4[8];
    u32 flags;
} Obj;

s32 fn_1_81860(Obj *a, Obj *b) {
    u32 flags_a = a->flags & 0xF0000000;
    if (flags_a != 0 && (b->flags & 0xF0000000) == 0) {
        return 1;
    }
    if ((b->flags & 0xF0000000) != 0 && flags_a == 0) {
        return -1;
    }
    return a->id - b->id;
}
/* fzgx:end fn_1_81860 */

/* fzgx:begin fn_1_818AC */
typedef struct {
    u8 pad0[0x320];
    s16 value;
} Obj;

s32 fn_1_818AC(Obj *a, Obj *b) {
    return a->value - b->value;
}
/* fzgx:end fn_1_818AC */

/* fzgx:begin fn_1_835E0 */
extern void fn_1_435C(void *);
extern u32 lbl_1_bss_6D970;
extern void fn_1_43E8(s32);
extern char lbl_1_data_20714[10];
extern void fn_1_850C4(void);
extern void fn_1_3F8C(char *, void (*)(void), void *, s32);
extern void fn_1_8B8A4(void);
extern u32 lbl_1_bss_6D974;
extern void fn_1_43F4(void);

void fn_1_835E0(void *arg0, void *arg1, void *arg2) {
    fn_1_435C(arg1);
    lbl_1_bss_6D970 = (u32)arg2;
    fn_1_43E8(1);
    fn_1_3F8C(lbl_1_data_20714, fn_1_850C4, arg0, 9);
    lbl_1_bss_6D974 = (u32)fn_1_8B8A4;
    fn_1_43F4();
}
/* fzgx:end fn_1_835E0 */

/* fzgx:begin fn_1_8365C */
extern u32 lbl_1_bss_6D97C;
extern u32 lbl_1_bss_6D978;

void fn_1_8365C(s32 enabled) {
    u32 value;

    if (enabled != 0) {
        lbl_1_bss_6D97C = 0;
        value = 0;
    } else {
        lbl_1_bss_6D97C = 1;
        value = 1;
    }
    lbl_1_bss_6D978 = value;
}
/* fzgx:end fn_1_8365C */

/* fzgx:begin fn_1_84124 */
typedef struct {
    u8 _pad[0x38c];
    u8 flags;
} Fn184124Object;

s8 fn_1_84124(Fn184124Object *obj, s8 direction, s8 index, u8 *count) {
    *count = 0;

    if (index == 5) {
        return index;
    }

    if (index < 0 || index > 5) {
        return index < 0 ? 0 : (index > 5 ? 5 : index);
    }

    if (direction == 1) {
        while (index < 5) {
            if (obj->flags & (1 << index)) {
                return index;
            }
            index++;
            (*count)++;
        }
    } else if (direction == -1) {
        while (index > -1) {
            if (obj->flags & (1 << index)) {
                return index;
            }
            index--;
            (*count)++;
        }
    }

    return -1;
}
/* fzgx:end fn_1_84124 */

/* fzgx:begin fn_1_843BC */
extern f32 lbl_1_rodata_3530[22];
extern f32 fn_1_86A2C(void *);

typedef struct {
    u8 _pad[0x32c];
    void *field_32c;
} Fn1843BCObject;

f32 fn_1_843BC(Fn1843BCObject *obj) {
    if (obj == NULL) {
        return lbl_1_rodata_3530[0];
    }

    if (obj->field_32c == NULL) {
        return lbl_1_rodata_3530[0];
    }

    return fn_1_86A2C(obj->field_32c);
}
/* fzgx:end fn_1_843BC */

/* fzgx:begin fn_1_8472C */
extern void *fn_1_36AD0(void);

typedef struct {
    u8 _pad[0x6];
    s16 field_6;
} Fn18472CObject;

void *fn_1_8472C(Fn18472CObject *obj) {
    s16 index;

    index = -1;
    if (obj->field_6 >= 0x29) {
        index = obj->field_6 - 0x32;
    }

    if (index == -1) {
        return NULL;
    }

    return (u8 *)fn_1_36AD0() + (s32)index * 0x81c0;
}
/* fzgx:end fn_1_8472C */

/* fzgx:begin fn_1_84794 */
extern u8 lbl_1_bss_6D868;

u8 fn_1_84794(s32 index) {
    return (&lbl_1_bss_6D868)[(s16)index];
}
/* fzgx:end fn_1_84794 */

/* fzgx:begin fn_1_85878 */
extern void fn_1_85688(void);
extern void fn_1_854D4(void);

void fn_1_85878(void) {
    fn_1_85688();
    fn_1_854D4();
}
/* fzgx:end fn_1_85878 */

/* fzgx:begin fn_1_8589C */
extern u32 lbl_1_bss_6D864;

void fn_1_8589C(u32 value) {
    lbl_1_bss_6D864 = value;
}
/* fzgx:end fn_1_8589C */

/* fzgx:begin fn_1_85F70 */
extern void fn_1_23500(void);

void fn_1_85F70(void) {
    fn_1_23500();
}
/* fzgx:end fn_1_85F70 */

/* fzgx:begin fn_1_8616C */
extern f32 lbl_1_rodata_3518[6];
extern void fn_1_85934(f32);

void fn_1_8616C(void) {
    fn_1_85934(lbl_1_rodata_3518[0]);
}
/* fzgx:end fn_1_8616C */

/* fzgx:begin fn_1_86194 */
extern void fn_1_85934(void);

// fn_1_86194: main_rel .text:0x00086194 size 0x20
// Wrapper that calls fn_1_85934.

void fn_1_86194(void) {
    fn_1_85934();
}
/* fzgx:end fn_1_86194 */

/* fzgx:begin fn_1_861B4 */
extern u8 lbl_1_bss_6D84A[6];
extern void fn_1_C489C(void *);
extern void fn_1_C40A0(void *);

// Processes each active car entry, then finalizes the car table.
void fn_1_861B4(void) {
    u32 i;
    u8 *entry;
    u8 *count;

    entry = (u8 *)lbl_1_bss_6D838;
    count = lbl_1_bss_6D84A;
    i = 0;
    while (i < (s8)*count) {
        if (*(u32 *)entry & 0x04000000) {
            fn_1_C489C(entry);
        }
        i++;
        entry += 0x620;
    }
    fn_1_C40A0((void *)lbl_1_bss_6D838);
}
/* fzgx:end fn_1_861B4 */

/* fzgx:begin fn_1_8623C */
// fn_1_8623C: returns a constant.
int fn_1_8623C(void) {
    return 0;
}
/* fzgx:end fn_1_8623C */

/* fzgx:begin fn_1_86244 */
#include "rel/main_rel/car.h"

u32 fn_1_86244(void) {
    return *(u32 *)(void *)&lbl_1_bss_6D838;
}
/* fzgx:end fn_1_86244 */

/* fzgx:begin fn_1_86254 */
#include "rel/main_rel/car.h"

extern Obj_1_bss_6D838_Target *lbl_1_bss_6D838;

void *fn_1_86254(int index) {
    if (lbl_1_bss_6D838 != 0) {
        return (u8 *)lbl_1_bss_6D838 + index * 0x620;
    }
    return 0;
}
/* fzgx:end fn_1_86254 */

/* fzgx:begin fn_1_8627C */
#include "rel/main_rel/car.h"

typedef struct {
    u8 pad_0[0x49C];
    u32 value;
    u8 pad_4A0[0x180];
} CarEntry_8627C;

u32 fn_1_8627C(s32 index) {
    CarEntry_8627C *cars;

    cars = (CarEntry_8627C *)lbl_1_bss_6D838;
    if (cars != 0) {
        return cars[index].value;
    }
    return 0;
}
/* fzgx:end fn_1_8627C */

/* fzgx:begin fn_1_8636C noprologue */
#include "types.h"
extern u8 *lbl_1_bss_6D838;
extern int fn_80007D58(void *);
extern void lbl_8006D784(void *);
extern void lbl_8006DD14(void *, void *);

void fn_1_8636C(int index, void *arg) {
    int offset = index * 0x620;

    if (fn_80007D58(lbl_1_bss_6D838 + offset + 0xec) != 0) {
        lbl_8006D784(arg);
    } else {
        lbl_8006DD14(lbl_1_bss_6D838 + offset + 0xec, arg);
    }
}
/* fzgx:end fn_1_8636C */

/* fzgx:begin fn_1_863E4 noprologue */
#include "types.h"

extern u8 *lbl_1_bss_6D838;
extern int fn_80007D58(void *);
extern void lbl_8006D784(void *);
extern void lbl_8006DD14(void *, void *);

void fn_1_863E4(int index, void *arg) {
    int offset = index * 0x620;

    if (fn_80007D58(lbl_1_bss_6D838 + offset + 0x5e0) != 0) {
        lbl_8006D784(arg);
    } else {
        lbl_8006DD14(lbl_1_bss_6D838 + offset + 0x5e0, arg);
    }
}
/* fzgx:end fn_1_863E4 */

/* fzgx:begin fn_1_864E8 */
// Returns the indexed car record from the shared car table.
u32 fn_1_864E8(int index) {
    return ((u32 *)lbl_1_bss_6D838)[index * (0x620 / sizeof(u32))];
}
/* fzgx:end fn_1_864E8 */

/* fzgx:begin fn_1_867F8 noprologue */
#include "types.h"
typedef struct {
    u8 pad[0x184];
    f32 value;
    u8 tail[0x498];
} Entry;

extern Entry *lbl_1_bss_6D838;

f32 fn_1_867F8(int index) {
    return lbl_1_bss_6D838[index].value;
}
/* fzgx:end fn_1_867F8 */

/* fzgx:begin fn_1_86810 noprologue */
#include "types.h"
typedef struct {
    u8 pad_0[0x49c];
    void *object;
    u8 pad_4a0[0x180];
} Entry;

typedef struct {
    u8 pad_0[0x115];
    u8 value;
} Object;

extern Entry *lbl_1_bss_6D838;

u8 fn_1_86810(int index) {
    return ((Object *)(lbl_1_bss_6D838[index].object))->value;
}
/* fzgx:end fn_1_86810 */

/* fzgx:begin fn_1_8682C */
extern u8 lbl_1_bss_6DA5C[30];

u8 fn_1_8682C(int index) {
    return lbl_1_bss_6DA5C[index];
}
/* fzgx:end fn_1_8682C */

/* fzgx:begin fn_1_8683C noprologue */
#include "types.h"
typedef struct {
    u8 pad_0[0x188];
    s16 value;
    u8 pad_18a[0x496];
} Entry;

extern Entry *lbl_1_bss_6D838;

s16 fn_1_8683C(int index) {
    return lbl_1_bss_6D838[index].value;
}
/* fzgx:end fn_1_8683C */

/* fzgx:begin fn_1_87880 */
extern f32 lbl_1_rodata_3C2C[33];

extern void fn_80008BA8(void *dst, const void *src, int size);
extern void lbl_8006D7DC(void *value);
extern void fn_8006E424(int value);
extern void lbl_8006DB74(void *value);

typedef struct Fn187880Inner {
    u8 pad_000[0x1FC];
    f32 value_1FC;
} Fn187880Inner;

typedef struct Fn187880Object {
    u8 pad_000[2];
    s8 field_002;
    u8 pad_003[0x208 - 3];
    u8 field_208[0x124];
    Fn187880Inner *field_32C;
    void *field_330;
    u8 pad_334[0x86];
    s16 field_3BA;
} Fn187880Object;

void fn_1_87880(Fn187880Object *object) {
    u8 value[12];

    if (object == 0) {
        return;
    }
    if (object->field_3BA != 0) {
        return;
    }
    if (object->field_002 - 1 < 0) {
        return;
    }

    fn_80008BA8(value, object->field_208, 12);
    lbl_8006D7DC(value);

    if (object->field_32C != 0) {
        fn_8006E424((int)(lbl_1_rodata_3C2C[0] * -object->field_32C->value_1FC));
    }

    lbl_8006DB74(object->field_330);
}
/* fzgx:end fn_1_87880 */

/* fzgx:begin fn_1_87918 */
extern f32 lbl_1_rodata_3C2C[33];

extern void fn_80008BA8(void *dst, const void *src, int size);
extern void lbl_8006D7DC(void *value);
extern void fn_8006E424(int value);
extern void lbl_8006DB74(void *value);

typedef struct Fn187918Inner {
    u8 pad_000[0x1FC];
    f32 value_1FC;
} Fn187918Inner;

typedef struct Fn187918Object {
    u8 pad_000[2];
    s8 field_002;
    u8 pad_003[0x208 - 3];
    u8 field_208[0x124];
    Fn187918Inner *field_32C;
    void *field_330;
    u8 pad_334[0x86];
    s16 field_3BA;
} Fn187918Object;

void fn_1_87918(Fn187918Object *object) {
    u8 value[12];

    if (object == 0) {
        return;
    }
    if (object->field_3BA != 0) {
        return;
    }
    if (object->field_002 - 1 < 0) {
        return;
    }

    fn_80008BA8(value, object->field_208, 12);
    lbl_8006D7DC(value);

    if (object->field_32C != 0) {
        fn_8006E424((int)(lbl_1_rodata_3C2C[0] * -object->field_32C->value_1FC));
    }

    lbl_8006DB74(object->field_330);
}
/* fzgx:end fn_1_87918 */

/* fzgx:begin fn_1_87AD8 */
extern f32 lbl_1_rodata_3C2C[33];

extern void fn_80008BA8(void *dst, const void *src, int size);
extern void lbl_8006D7DC(void *value);
extern void fn_8006E424(int value);
extern void lbl_8006DB74(void *value);

typedef struct Fn187AD8Inner {
    u8 pad_000[0x1FC];
    f32 value_1FC;
} Fn187AD8Inner;

typedef struct Fn187AD8Object {
    u8 pad_000[2];
    s8 field_002;
    u8 pad_003[0x208 - 3];
    u8 field_208[0x124];
    Fn187AD8Inner *field_32C;
    void *field_330;
    u8 pad_334[0x86];
    s16 field_3BA;
} Fn187AD8Object;

void fn_1_87AD8(Fn187AD8Object *object) {
    u8 value[12];

    if (object == 0) {
        return;
    }
    if (object->field_3BA != 0) {
        return;
    }
    if (object->field_002 - 1 < 0) {
        return;
    }

    fn_80008BA8(value, object->field_208, 12);
    lbl_8006D7DC(value);

    if (object->field_32C != 0) {
        fn_8006E424((int)(lbl_1_rodata_3C2C[0] * -object->field_32C->value_1FC));
    }

    lbl_8006DB74(object->field_330);
}
/* fzgx:end fn_1_87AD8 */

/* fzgx:begin fn_1_87F98 */
extern f32 lbl_1_rodata_3C2C[33];

extern void fn_80008BA8(void *dst, const void *src, int size);
extern void lbl_8006D7DC(void *value);
extern void fn_8006E424(int value);
extern void lbl_8006DB74(void *value);

typedef struct Fn187F98Inner {
    u8 pad_000[0x1FC];
    f32 value_1FC;
} Fn187F98Inner;

typedef struct Fn187F98Object {
    u8 pad_000[2];
    s8 field_002;
    u8 pad_003[0x208 - 3];
    u8 field_208[0x124];
    Fn187F98Inner *field_32C;
    void *field_330;
    u8 pad_334[0x86];
    s16 field_3BA;
} Fn187F98Object;

void fn_1_87F98(Fn187F98Object *object) {
    u8 value[12];

    if (object == 0) {
        return;
    }
    if (object->field_3BA != 0) {
        return;
    }
    if (object->field_002 - 1 < 0) {
        return;
    }

    fn_80008BA8(value, object->field_208, 12);
    lbl_8006D7DC(value);

    if (object->field_32C != 0) {
        fn_8006E424((int)(lbl_1_rodata_3C2C[0] * -object->field_32C->value_1FC));
    }

    lbl_8006DB74(object->field_330);
}
/* fzgx:end fn_1_87F98 */

/* fzgx:begin fn_1_88328 */
extern f32 lbl_1_rodata_3C2C[33];

extern void fn_80008BA8(void *dst, const void *src, int size);
extern void lbl_8006D7DC(void *value);
extern void fn_8006E424(int value);
extern void lbl_8006DB74(void *value);

typedef struct Fn88328Inner {
    u8 pad_000[0x1FC];
    f32 value_1FC;
} Fn88328Inner;

typedef struct Fn88328Object {
    u8 pad_000[2];
    s8 field_002;
    u8 pad_003[0x208 - 3];
    u8 field_208[0x124];
    Fn88328Inner *field_32C;
    void *field_330;
    u8 pad_334[0x86];
    s16 field_3BA;
} Fn88328Object;

void fn_1_88328(Fn88328Object *object) {
    u8 value[12];

    if (object == 0) {
        return;
    }
    if (object->field_3BA != 0) {
        return;
    }
    if (object->field_002 - 1 < 0) {
        return;
    }

    fn_80008BA8(value, object->field_208, 12);
    lbl_8006D7DC(value);

    if (object->field_32C != 0) {
        fn_8006E424((int)(lbl_1_rodata_3C2C[0] * -object->field_32C->value_1FC));
    }

    lbl_8006DB74(object->field_330);
}
/* fzgx:end fn_1_88328 */

/* fzgx:begin fn_1_883C0 */
extern f32 lbl_1_rodata_3C2C[33];

extern void fn_80008BA8(void *dst, const void *src, int size);
extern void lbl_8006D7DC(void *value);
extern void fn_8006E424(int value);
extern void lbl_8006DB74(void *value);

typedef struct Fn883C0Inner {
    u8 pad_000[0x1FC];
    f32 value_1FC;
} Fn883C0Inner;

typedef struct Fn883C0Object {
    u8 pad_000[2];
    s8 field_002;
    u8 pad_003[0x208 - 3];
    u8 field_208[0x124];
    Fn883C0Inner *field_32C;
    void *field_330;
    u8 pad_334[0x86];
    s16 field_3BA;
} Fn883C0Object;

void fn_1_883C0(Fn883C0Object *object) {
    u8 value[12];

    if (object == 0) {
        return;
    }
    if (object->field_3BA != 0) {
        return;
    }
    if (object->field_002 - 1 < 0) {
        return;
    }

    fn_80008BA8(value, object->field_208, 12);
    lbl_8006D7DC(value);

    if (object->field_32C != 0) {
        fn_8006E424((int)(lbl_1_rodata_3C2C[0] * -object->field_32C->value_1FC));
    }

    lbl_8006DB74(object->field_330);
}
/* fzgx:end fn_1_883C0 */

/* fzgx:begin fn_1_8856C */
extern f32 lbl_1_rodata_3C2C[33];

extern void fn_80008BA8(void *dst, const void *src, int size);
extern void lbl_8006D7DC(void *value);
extern void fn_8006E424(int value);
extern void lbl_8006DB74(void *value);

typedef struct Fn8856CInner {
    u8 pad_000[0x1FC];
    f32 value_1FC;
} Fn8856CInner;

typedef struct Fn8856CObject {
    u8 pad_000[2];
    s8 field_002;
    u8 pad_003[0x208 - 3];
    u8 field_208[0x124];
    Fn8856CInner *field_32C;
    void *field_330;
    u8 pad_334[0x86];
    s16 field_3BA;
} Fn8856CObject;

void fn_1_8856C(Fn8856CObject *object) {
    u8 value[12];

    if (object == 0) {
        return;
    }
    if (object->field_3BA != 0) {
        return;
    }
    if (object->field_002 - 1 < 0) {
        return;
    }

    fn_80008BA8(value, object->field_208, 12);
    lbl_8006D7DC(value);

    if (object->field_32C != 0) {
        fn_8006E424((int)(lbl_1_rodata_3C2C[0] * -object->field_32C->value_1FC));
    }

    lbl_8006DB74(object->field_330);
}
/* fzgx:end fn_1_8856C */

/* fzgx:begin fn_1_88A2C */
extern f32 lbl_1_rodata_3C2C[33];

extern void fn_80008BA8(void *dst, const void *src, int size);
extern void lbl_8006D7DC(void *value);
extern void fn_8006E424(int value);
extern void lbl_8006DB74(void *value);

typedef struct Fn1_88A2CInner {
    u8 pad_000[0x1FC];
    f32 value_1FC;
} Fn1_88A2CInner;

typedef struct Fn1_88A2CObject {
    u8 pad_000[2];
    s8 field_002;
    u8 pad_003[0x208 - 3];
    u8 field_208[0x124];
    Fn1_88A2CInner *field_32C;
    void *field_330;
    u8 pad_334[0x86];
    s16 field_3BA;
} Fn1_88A2CObject;

void fn_1_88A2C(Fn1_88A2CObject *object) {
    u8 value[12];

    if (object == 0) {
        return;
    }
    if (object->field_3BA != 0) {
        return;
    }
    if (object->field_002 - 1 < 0) {
        return;
    }

    fn_80008BA8(value, object->field_208, 12);
    lbl_8006D7DC(value);

    if (object->field_32C != 0) {
        fn_8006E424((int)(lbl_1_rodata_3C2C[0] * -object->field_32C->value_1FC));
    }

    lbl_8006DB74(object->field_330);
}
/* fzgx:end fn_1_88A2C */

/* fzgx:begin fn_1_88DBC */
extern f32 lbl_1_rodata_3C2C[33];

extern void fn_80008BA8(void *dst, const void *src, int size);
extern void lbl_8006D7DC(void *value);
extern void fn_8006E424(int value);
extern void lbl_8006DB74(void *value);

typedef struct Fn1_88DBCInner {
    u8 pad_000[0x1FC];
    f32 value_1FC;
} Fn1_88DBCInner;

typedef struct Fn1_88DBCObject {
    u8 pad_000[2];
    s8 field_002;
    u8 pad_003[0x208 - 3];
    u8 field_208[0x124];
    Fn1_88DBCInner *field_32C;
    void *field_330;
    u8 pad_334[0x86];
    s16 field_3BA;
} Fn1_88DBCObject;

void fn_1_88DBC(Fn1_88DBCObject *object) {
    u8 value[12];

    if (object == 0) {
        return;
    }
    if (object->field_3BA != 0) {
        return;
    }
    if (object->field_002 - 1 < 0) {
        return;
    }

    fn_80008BA8(value, object->field_208, 12);
    lbl_8006D7DC(value);

    if (object->field_32C != 0) {
        fn_8006E424((int)(lbl_1_rodata_3C2C[0] * -object->field_32C->value_1FC));
    }

    lbl_8006DB74(object->field_330);
}
/* fzgx:end fn_1_88DBC */

/* fzgx:begin fn_1_88E54 */
extern f32 lbl_1_rodata_3C2C[33];

extern void fn_80008BA8(void *dst, const void *src, int size);
extern void lbl_8006D7DC(void *value);
extern void fn_8006E424(int value);
extern void lbl_8006DB74(void *value);

typedef struct Fn1_88E54Inner {
    u8 pad_000[0x1FC];
    f32 value_1FC;
} Fn1_88E54Inner;

typedef struct Fn1_88E54Object {
    u8 pad_000[2];
    s8 field_002;
    u8 pad_003[0x208 - 3];
    u8 field_208[0x124];
    Fn1_88E54Inner *field_32C;
    void *field_330;
    u8 pad_334[0x86];
    s16 field_3BA;
} Fn1_88E54Object;

void fn_1_88E54(Fn1_88E54Object *object) {
    u8 value[12];

    if (object == 0) {
        return;
    }
    if (object->field_3BA != 0) {
        return;
    }
    if (object->field_002 - 1 < 0) {
        return;
    }

    fn_80008BA8(value, object->field_208, 12);
    lbl_8006D7DC(value);

    if (object->field_32C != 0) {
        fn_8006E424((int)(lbl_1_rodata_3C2C[0] * -object->field_32C->value_1FC));
    }

    lbl_8006DB74(object->field_330);
}
/* fzgx:end fn_1_88E54 */

/* fzgx:begin fn_1_88EEC */
extern f32 lbl_1_rodata_3C2C[33];

extern void fn_80008BA8(void *dst, const void *src, int size);
extern void lbl_8006D7DC(void *value);
extern void fn_8006E424(int value);
extern void lbl_8006DB74(void *value);

typedef struct Fn88EECInner {
    u8 pad_000[0x1FC];
    f32 value_1FC;
} Fn88EECInner;

typedef struct Fn88EECObject {
    u8 pad_000[2];
    s8 field_002;
    u8 pad_003[0x208 - 3];
    u8 field_208[0x124];
    Fn88EECInner *field_32C;
    void *field_330;
    u8 pad_334[0x86];
    s16 field_3BA;
} Fn88EECObject;

void fn_1_88EEC(Fn88EECObject *object) {
    u8 value[12];

    if (object == 0) {
        return;
    }
    if (object->field_3BA != 0) {
        return;
    }
    if (object->field_002 - 1 < 0) {
        return;
    }

    fn_80008BA8(value, object->field_208, 12);
    lbl_8006D7DC(value);

    if (object->field_32C != 0) {
        fn_8006E424((int)(lbl_1_rodata_3C2C[0] * -object->field_32C->value_1FC));
    }

    lbl_8006DB74(object->field_330);
}
/* fzgx:end fn_1_88EEC */

/* fzgx:begin fn_1_8A168 */
extern void fn_1_892FC(void *arg0, int arg1, int arg2);

void fn_1_8A168(void *arg0) {
    fn_1_892FC(arg0, 0, 0);
}
/* fzgx:end fn_1_8A168 */

/* fzgx:begin fn_1_8C3EC */
extern u8 lbl_1_bss_6DA7A;

u8 fn_1_8C3EC(void) {
    return lbl_1_bss_6DA7A;
}
/* fzgx:end fn_1_8C3EC */

/* fzgx:begin fn_1_8C3FC */
// fn_1_8C3FC: Load byte from BSS and return

extern u8 lbl_1_bss_6DA7B[5];

u8 fn_1_8C3FC(void) {
    return lbl_1_bss_6DA7B[0];
}
/* fzgx:end fn_1_8C3FC */

/* fzgx:begin fn_1_8C51C */
 // Return the indexed car's float field, or zero-base fallback when the car table is absent.
f32 fn_1_8C51C(u32 index) {
    u32 base;
    u32 addr;
    u32 ptr;

    base = (u32)lbl_1_bss_6D838;
    if (base != 0) {
        addr = base + index * 0x620;
    } else {
        addr = 0;
    }

    ptr = *(u32 *)(addr + 0x49c);
    return *(f32 *)(ptr + 0x5c);
}
/* fzgx:end fn_1_8C51C */

/* fzgx:begin fn_1_8C5AC */
// Return the indexed car's float value.
f32 fn_1_8C5AC(u32 index) {
    u32 base;
    u32 addr;
    u32 ptr;

    base = (u32)lbl_1_bss_6D838;
    if (base != 0) {
        addr = base + index * 0x620;
    } else {
        addr = 0;
    }

    ptr = *(u32 *)(addr + 0x49c);
    return *(f32 *)(ptr + 0x68);
}
/* fzgx:end fn_1_8C5AC */

/* fzgx:begin fn_1_8C66C */
// Return the value stored in the indexed car's nested state object.
u32 fn_1_8C66C(u32 index) {
    u32 base;
    u32 addr;
    u32 ptr;

    base = (u32)lbl_1_bss_6D838;
    if (base != 0) {
        addr = base + index * 0x620;
    } else {
        addr = 0;
    }

    ptr = *(u32 *)(addr + 0x49c);
    ptr = *(u32 *)(ptr + 0x90);
    return *(u32 *)ptr;
}
/* fzgx:end fn_1_8C66C */

/* fzgx:begin fn_1_8C6D4 */
u32 fn_1_8C6D4(u32 index) {
    u32 base;
    u32 car;
    u32 state;

    // Return the requested car's value from its state block.
    base = (u32)lbl_1_bss_6D838;
    if (base != 0) {
        car = base + index * 0x620;
    } else {
        car = 0;
    }

    state = *(u32 *)(car + 0x49c);
    return *(u32 *)(state + 0xb8);
}
/* fzgx:end fn_1_8C6D4 */

/* fzgx:begin fn_1_8C704 */
// Returns the selected car's value at offset 0xd8.
u32 fn_1_8C704(u32 index) {
    u32 base;
    u32 addr;
    u32 ptr;

    base = (u32)lbl_1_bss_6D838;
    if (base != 0) {
        addr = base + index * 0x620;
    } else {
        addr = 0;
    }

    ptr = *(u32 *)(addr + 0x49c);
    return *(u32 *)(ptr + 0xd8);
}
/* fzgx:end fn_1_8C704 */

/* fzgx:begin fn_1_8C734 */
// Return the selected car's float field, or the null object's field when unavailable.
f32 fn_1_8C734(u32 index) {
    u32 base;
    u32 addr;
    u32 ptr;

    base = (u32)lbl_1_bss_6D838;
    if (base != 0) {
        addr = base + index * 0x620;
    } else {
        addr = 0;
    }

    ptr = *(u32 *)(addr + 0x49c);
    return *(f32 *)(ptr + 0xf8);
}
/* fzgx:end fn_1_8C734 */

/* fzgx:begin fn_1_8C7C4 */
// Return the selected car's status byte, or the null-entry status when unavailable.
u8 fn_1_8C7C4(u32 index) {
    u32 base;
    u32 addr;
    u32 ptr;

    base = (u32)lbl_1_bss_6D838;
    if (base != 0) {
        addr = base + index * 0x620;
    } else {
        addr = 0;
    }

    ptr = *(u32 *)(addr + 0x49c);
    return *(u8 *)(ptr + 0x115);
}
/* fzgx:end fn_1_8C7C4 */

/* fzgx:begin fn_1_8C9A4 noprologue */
#include "types.h"
extern u32 lbl_1_bss_6D838;

u32 fn_1_8C9A4(void) {
    return lbl_1_bss_6D838 + 0x6;
}
/* fzgx:end fn_1_8C9A4 */

/* fzgx:begin fn_1_8C9B8 noprologue */
#include "types.h"
extern u32 lbl_1_bss_6D838;

u32 fn_1_8C9B8(void) {
    return lbl_1_bss_6D838 + 0x7c;
}
/* fzgx:end fn_1_8C9B8 */

/* fzgx:begin fn_1_8CA20 */
// fn_1_8CA20: Load value from BSS and return with offset applied

extern u32 lbl_1_bss_6D83C;

u32 fn_1_8CA20(void) {
    return lbl_1_bss_6D83C + 0x3a8;
}
/* fzgx:end fn_1_8CA20 */

/* fzgx:begin fn_1_8D1EC */
extern void fn_1_8D210(u32 arg0, u32 arg1, u32 arg2, u32 arg3);

void fn_1_8D1EC(u32 arg0, u32 arg1, u32 arg2) {
    fn_1_8D210(arg0, arg1, arg2, 0);
}
/* fzgx:end fn_1_8D1EC */

/* fzgx:begin fn_1_8DBA4 */
extern u8 lbl_1_bss_6D820;

void fn_1_8DBA4(u8 value) {
    lbl_1_bss_6D820 = value;
}
/* fzgx:end fn_1_8DBA4 */

/* fzgx:begin fn_1_8E1E8 */
extern u32 lbl_1_bss_6E958[2];
extern void fn_80071718(u32 value);
extern void fn_800711A8(u32 value);

void fn_1_8E1E8(void) {
    if (lbl_1_bss_6E958[1] != 0) {
        fn_80071718(lbl_1_bss_6E958[1]);
        fn_800711A8(lbl_1_bss_6E958[0]);
        lbl_1_bss_6E958[1] = 0;
        lbl_1_bss_6E958[0] = 0;
    }
}
/* fzgx:end fn_1_8E1E8 */

/* fzgx:begin fn_1_8E448 */
typedef struct {
    u8 pad[0x3bc];
    u32 value;
} Object;

void fn_1_8E448(Object *obj, u32 value) {
    obj->value = value;
}
/* fzgx:end fn_1_8E448 */

/* fzgx:begin fn_1_8F5A4 */
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
/* fzgx:end fn_1_8F5A4 */

/* fzgx:begin fn_1_924CC */
extern s32 lbl_1_bss_6E984;
extern u32 lbl_1_rodata_3FA0[28];
extern void fn_80007AB4(void *arg0);

void fn_1_924CC(void) {
    u32 value;

    if (lbl_1_bss_6E984 != 0) {
        value = lbl_1_rodata_3FA0[0];
        fn_80007AB4(&value);
    }
}
/* fzgx:end fn_1_924CC */

/* fzgx:begin fn_1_95210 */
extern void fn_80008BEC(void *arg0, s32 arg1, s32 arg2);

void fn_1_95210(void *arg0) {
    fn_80008BEC(arg0, 0, 0x70);
}
/* fzgx:end fn_1_95210 */
