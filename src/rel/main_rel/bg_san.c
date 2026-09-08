#include "types.h"
#include "rel/main_rel/globals.h"

/* fzgx:begin fn_1_DA7B8 */
#include "rel/main_rel/bg_san.h"

extern void fn_1_DAAFC(Obj_1_data_2A7E0_At3C *obj);

// Pass the active background object to the next processing stage.
void fn_1_DA7B8(void) {
    fn_1_DAAFC(lbl_1_data_2A7E0.unk_3C);
}
/* fzgx:end fn_1_DA7B8 */

/* fzgx:begin fn_1_DA9F0 */
#include "rel/main_rel/bg_san.h"

extern u32 fn_1_58C4(void);
extern void fn_1_DAB1C(Obj_1_data_2A7E0_At3C *obj);

void fn_1_DA9F0(void) {
    Obj_1_data_2A7E0_At3C *obj = lbl_1_data_2A7E0.unk_3C;

    if (fn_1_58C4() == 1) {
        fn_1_DAB1C(obj);
    }
}
/* fzgx:end fn_1_DA9F0 */

/* fzgx:begin fn_1_DAA34 */
extern void fn_1_9AD54(void);
extern void fn_1_FBEA8(void);

void fn_1_DAA34(void) {
    fn_1_9AD54();
    fn_1_FBEA8();
}
/* fzgx:end fn_1_DAA34 */

/* fzgx:begin fn_1_DAA58 */
#include "rel/main_rel/bg_san.h"

extern void fn_1_FB9C0(u32 value);
extern void fn_1_103F10(void *state);
extern void fn_1_DAB3C(Obj_1_data_2A7E0_At3C *state);

// Initializes the background state and advances its active substate.
void fn_1_DAA58(void) {
    Obj_1_data_2A7E0_At3C *state = lbl_1_data_2A7E0.unk_3C;

    if (state->unk_BD4 != 0) {
        fn_1_FB9C0(0);
        fn_1_FB9C0(1);
        fn_1_FB9C0(2);
        fn_1_FB9C0(4);
    }
    fn_1_103F10(&state->unk_BD8);
    fn_1_DAB3C(state);
}
/* fzgx:end fn_1_DAA58 */

/* fzgx:begin fn_1_DAAC4 */
#include "rel/main_rel/bg_san.h"

extern void fn_1_103F58(void *arg);
extern void fn_1_9AD88(void);

void fn_1_DAAC4(void) {
    fn_1_103F58(&lbl_1_data_2A7E0.unk_3C->unk_BD8);
    fn_1_9AD88();
}
/* fzgx:end fn_1_DAAC4 */

/* fzgx:begin fn_1_DAAF8 */
// fn_1_DAAF8: empty in retail (single blr).
void fn_1_DAAF8(void) {
}
/* fzgx:end fn_1_DAAF8 */

/* fzgx:begin fn_1_DAAFC */
extern void fn_1_58C4(void);

void fn_1_DAAFC(void) {
    fn_1_58C4();
}
/* fzgx:end fn_1_DAAFC */

/* fzgx:begin fn_1_DAB1C */
extern void fn_1_58C4(void);

void fn_1_DAB1C(void) {
    fn_1_58C4();
}
/* fzgx:end fn_1_DAB1C */

/* fzgx:begin fn_1_DAB3C */
extern void fn_1_58C4(void);

void fn_1_DAB3C(void) {
    fn_1_58C4();
}
/* fzgx:end fn_1_DAB3C */

/* fzgx:begin fn_1_DAD68 */
// fn_1_DAD68: empty in retail (single blr).
void fn_1_DAD68(void) {
}
/* fzgx:end fn_1_DAD68 */

/* fzgx:begin fn_1_DAD6C */
#include "rel/main_rel/bg_san.h"

extern void fn_1_9A508(void);
extern void fn_1_DAF94(Obj_1_data_2A7E0_At3C *);

void fn_1_DAD6C(void) {
    Obj_1_data_2A7E0_At3C *obj;

    obj = lbl_1_data_2A7E0.unk_3C;
    fn_1_9A508();
    fn_1_DAF94(obj);
}
/* fzgx:end fn_1_DAD6C */

/* fzgx:begin fn_1_DAEF8 */
// fn_1_DAEF8: empty in retail (single blr).
void fn_1_DAEF8(void) {
}
/* fzgx:end fn_1_DAEF8 */

/* fzgx:begin fn_1_DAEFC */
extern s32 fn_1_5910(void);
extern void *fn_1_9D260(void);
extern void fn_1_DB198(void *arg0, void *arg1);
extern s16 fn_1_9AD88(void);
extern s16 fn_1_7B054(void);
extern s16 fn_1_3F0C8(void);
extern f32 lbl_1_rodata_663C;
extern void fn_1_1067A8(void *arg0, f32 arg1, f32 arg2);

// Initializes the current background object and updates it for the active state.
void fn_1_DAEFC(void) {
    s32 index;
    Obj_1_data_2A7E0_At3C *object;

    index = fn_1_5910();
    object = lbl_1_data_2A7E0.unk_3C;
    fn_1_DB198(object, fn_1_9D260());
    fn_1_9AD88();
    if (fn_1_7B054() == 42) {
        if (fn_1_3F0C8() != 39) {
            fn_1_1067A8((u8 *)object + 0x142a0, ((f32 *)object)[index], lbl_1_rodata_663C);
        }
    }
}
/* fzgx:end fn_1_DAEFC */

/* fzgx:begin fn_1_DAF90 */
// fn_1_DAF90: empty in retail (single blr).
void fn_1_DAF90(void) {
}
/* fzgx:end fn_1_DAF90 */

/* fzgx:begin fn_1_DB138 */
typedef struct {
    u8 unk_00[0x10];
    s32 count;
} BgSanContext;

extern u32 fn_1_58C4(BgSanContext *arg0);
extern void fn_1_DB268(void *arg0);

void fn_1_DB138(BgSanContext *arg0) {
    u8 *entry;
    s32 count;

    if (fn_1_58C4(arg0) < 2) {
        count = arg0->count;
        entry = (u8 *)arg0 + 0x14;
        while (count > 0) {
            fn_1_DB268(entry);
            count -= 1;
            entry += 0x1020;
        }
    }
}
/* fzgx:end fn_1_DB138 */

/* fzgx:begin fn_1_DB198 */
typedef struct BgSanObject {
    u8 pad_0000[0x10];
    s32 active;
    void *data;
    u8 pad_0018[0x80c];
    f32 offset_x;
    f32 offset_y;
    f32 offset_z;
} BgSanObject;

extern u32 fn_1_58C4(BgSanObject *object);
extern f32 lbl_1_rodata_6644[35];
extern void lbl_8006DCA4(void *data);
extern void *fn_1_5448C(f32 *position);
extern void *fn_1_548AC(u32 size);
extern void fn_1_DB53C(void);
extern void fn_1_5489C(void *result, void *allocation);

void fn_1_DB198(BgSanObject *object, void *arg1) {
    f32 position[3];
    void *result;
    void *allocation;

    if (fn_1_58C4(object) < 2 && object->active != 0) {
        position[0] = (((f32 *)object->data)[3] + object->offset_x) * lbl_1_rodata_6644[0];
        position[1] = (((f32 *)object->data)[4] + object->offset_y) * lbl_1_rodata_6644[0];
        position[2] = (((f32 *)object->data)[5] + object->offset_z) * lbl_1_rodata_6644[0];
        lbl_8006DCA4(object->data);
        result = fn_1_5448C(position);
        allocation = fn_1_548AC(12);
        if (allocation != 0) {
            ((void **)allocation)[1] = (void *)fn_1_DB53C;
            ((void **)allocation)[2] = arg1;
            fn_1_5489C(result, allocation);
        }
    }
}
/* fzgx:end fn_1_DB198 */

/* fzgx:begin fn_1_DC1B8 */
// fn_1_DC1B8: returns a constant.
int fn_1_DC1B8(void) {
    return 0;
}
/* fzgx:end fn_1_DC1B8 */

/* fzgx:begin fn_1_DC204 */
// fn_1_DC204: empty in retail (single blr).
void fn_1_DC204(void) {
}
/* fzgx:end fn_1_DC204 */

/* fzgx:begin fn_1_DC208 */
#include "rel/main_rel/bg_san.h"

extern void fn_1_9A508(void);
extern void fn_1_103D28(void *arg0, int arg1, f32 arg2);
extern void fn_1_DC3A4(Obj_1_data_2A7E0_At3C *arg0);
extern f32 lbl_1_rodata_66D0[30];

void fn_1_DC208(void) {
    Obj_1_data_2A7E0_At3C *obj = lbl_1_data_2A7E0.unk_3C;

    obj->unk_0 = 0;
    fn_1_9A508();
    fn_1_103D28((u8 *)obj + 0x1588, 0x14, lbl_1_rodata_66D0[0]);
    fn_1_DC3A4(obj);
}
/* fzgx:end fn_1_DC208 */

/* fzgx:begin fn_1_DC264 */
// fn_1_DC264: empty in retail (single blr).
void fn_1_DC264(void) {
}
/* fzgx:end fn_1_DC264 */

/* fzgx:begin fn_1_DC268 */
#include "rel/main_rel/bg_san.h"

void fn_1_9AD54(void);
s32 fn_1_9D260(void);
void fn_1_DCB10(void);
void fn_1_DC454(Obj_1_data_2A7E0_At3C *, s32);
s32 fn_1_58C4(void);
void fn_1_5948(s32);
void fn_1_DC648(Obj_1_data_2A7E0_At3C *);
void fn_1_627C(s32);

void fn_1_DC268(void) {
    Obj_1_data_2A7E0_At3C *obj;
    s32 i;
    s32 count;

    obj = lbl_1_data_2A7E0.unk_3C;
    fn_1_9AD54();
    count = fn_1_9D260();
    fn_1_DCB10();
    fn_1_DC454(obj, count);
    count = fn_1_58C4();
    for (i = 0; i < count; i++) {
        fn_1_5948(i);
        fn_1_DC648(obj);
        fn_1_627C(i);
    }
}
/* fzgx:end fn_1_DC268 */

/* fzgx:begin fn_1_DC2F8 */
#include "rel/main_rel/bg_san.h"

extern void fn_1_72648(Obj_1_data_2A7E0 *);
extern void fn_1_DC404(Obj_1_data_2A7E0_At3C *);
extern void fn_1_103F10(void *);
extern Obj_1_data_2A7E0 lbl_1_data_2A7E0;

void fn_1_DC2F8(void) {
    Obj_1_data_2A7E0_At3C *obj;

    obj = lbl_1_data_2A7E0.unk_3C;
    fn_1_72648(&lbl_1_data_2A7E0);
    fn_1_DC404(obj);
    fn_1_103F10((u8 *)obj + 0x1588);
}
/* fzgx:end fn_1_DC2F8 */

/* fzgx:begin fn_1_DC33C */
#include "rel/main_rel/bg_san.h"

extern s32 fn_1_9D260(Obj_1_data_2A7E0 *);
extern void fn_1_103F58(void *);
extern void fn_1_DC6FC(Obj_1_data_2A7E0_At3C *);
extern void fn_1_DC5E8(Obj_1_data_2A7E0_At3C *, s32);
extern void fn_1_9AD88(void);

void fn_1_DC33C(void) {
    Obj_1_data_2A7E0_At3C *obj;
    s32 result;

    obj = lbl_1_data_2A7E0.unk_3C;
    result = fn_1_9D260(&lbl_1_data_2A7E0);
    fn_1_103F58(&obj->unk_1588);
    fn_1_DC6FC(obj);
    fn_1_DC5E8(obj, result);
    fn_1_9AD88();
}
/* fzgx:end fn_1_DC33C */

/* fzgx:begin fn_1_DC3A0 */
// fn_1_DC3A0: empty in retail (single blr).
void fn_1_DC3A0(void) {
}
/* fzgx:end fn_1_DC3A0 */

/* fzgx:begin fn_1_DC3A4 */
extern void fn_1_103090(void *);

typedef struct Entry {
    u8 unk00[0x68];
    s32 initialized;
    u8 unk6c[0x40];
} Entry;

typedef struct Container {
    s32 count;
    Entry entries[1];
} Container;

void fn_1_DC3A4(Container *container) {
    s32 count;
    Entry *entry;

    count = container->count;
    entry = container->entries;
    while (count > 0) {
        entry->initialized = 1;
        fn_1_103090(entry);
        count--;
        entry++;
    }
}
/* fzgx:end fn_1_DC3A4 */

/* fzgx:begin fn_1_DC404 */
extern void fn_1_1030A4(void *);

typedef struct Entry {
    u8 data[0xac];
} Entry;

typedef struct Container {
    s32 count;
    Entry entries[1];
} Container;

void fn_1_DC404(Container *container) {
    s32 count;
    Entry *entry;

    count = container->count;
    entry = container->entries;
    while (count > 0) {
        fn_1_1030A4(entry);
        count--;
        entry++;
    }
}
/* fzgx:end fn_1_DC404 */

/* fzgx:begin fn_1_DC5E8 */
extern void fn_1_103264(void *, void *);

typedef struct Entry {
    u8 unk00[0xac];
} Entry;

typedef struct Container {
    s32 count;
    Entry entries[1];
} Container;

void fn_1_DC5E8(Container *container, void *arg) {
    s32 count;
    Entry *entry;

    entry = container->entries;
    count = container->count;
    while (count > 0) {
        fn_1_103264(entry, arg);
        count--;
        entry++;
    }
}
/* fzgx:end fn_1_DC5E8 */

/* fzgx:begin fn_1_DC6FC */
extern void *fn_1_54448(s32);
extern void *fn_1_548AC(s32);
extern void fn_1_DC764(void);
extern void fn_1_5489C(void *, void *);

typedef struct Handler {
    u8 unk00[4];
    void (*callback)(void);
    void *context;
} Handler;

void fn_1_DC6FC(void *context) {
    void *value;
    Handler *handler;

    value = fn_1_54448(0);
    handler = fn_1_548AC(12);
    if (handler != 0) {
        handler->callback = fn_1_DC764;
        handler->context = context;
        fn_1_5489C(value, handler);
    }
}
/* fzgx:end fn_1_DC6FC */

/* fzgx:begin fn_1_DCB10 */
#include "rel/main_rel/bg_san.h"

extern const f64 lbl_1_rodata_6748;
extern void fn_1_D6C10(void *, f32);

typedef struct Entry {
    u8 pad_00[0x20];
    u32 flags;
} Entry;

typedef struct Container {
    u8 pad_00[0xa4];
    s32 count;
    Entry *entries;
} Container;

void fn_1_DCB10(void) {
    Container **global;
    s32 i;
    Entry *entry;

    global = (Container **)&lbl_1_bss_3BE0;
    i = 0;
    entry = (*global)->entries;
    while (i < (*global)->count) {
        if (entry->flags & 2) {
            fn_1_D6C10(entry, *(const f32 *)&lbl_1_rodata_6748);
        }
        i++;
        entry++;
    }
}
/* fzgx:end fn_1_DCB10 */

/* fzgx:begin fn_1_DCB9C */
#include "rel/main_rel/bg_san.h"

s32 fn_1_DCB9C(s32 value, u32 *data) {
    Obj_1_data_2A7E0_At3C *obj;

    obj = lbl_1_data_2A7E0.unk_3C;
    switch (value) {
    case 0:
        obj->unk_1584 = *data;
        break;
    case 1:
        obj->unk_225C = *data;
        break;
    case 2:
        obj->unk_2260 = *data;
        break;
    }
    return 1;
}
/* fzgx:end fn_1_DCB9C */
