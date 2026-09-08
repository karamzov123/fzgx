#include "types.h"
#include "rel/main_rel/globals.h"
#include "rel/main_rel/rob.h"

extern void fn_1_12ABB4(void *arg0);
extern void fn_1_96230(void *arg0, void *arg1, void *arg2, void *arg3, int arg4);
extern void fn_1_41850(void *arg0);
extern u32 lbl_1_bss_6E98C;
extern u16 fn_1_96B14(void *base, void *arg1, s32 index);
extern s32 fn_80083BCC(void *arg0, void *arg1);
extern void fn_8004C698(u32 arg0);
extern u32 lbl_801A6410;
extern void fn_1_46B4(u32, u32, u8 *, u32);
extern void fn_80008BEC(void *, u32, u32);

extern const char *lbl_1_data_20D1C[];
extern void fn_8008069C(char *buffer, u8 *format, ...);
extern s32 fn_1_12A32C(char *buffer);
extern s16 fn_1_98104(void);

/* fzgx:begin fn_1_961F0 */
typedef struct Fn1961F0Owner Fn1961F0Owner;

struct Fn1961F0Owner {
    u8 unk_00[0x0C];
    void *resource;
};

void fn_1_961F0(Fn1961F0Owner *owner) {
    if (owner->resource != 0) {
        fn_1_12ABB4(owner->resource);
        owner->resource = 0;
    }
}
/* fzgx:end fn_1_961F0 */

/* fzgx:begin fn_1_96630 */
typedef struct Fn196630Object Fn196630Object;

struct Fn196630Object {
    u8 unk_00[0x88];
    void *value_88;
};

void fn_1_96630(Fn196630Object *object, void *arg1, void *arg2) {
    fn_1_96230(object, object->value_88, arg1, arg2, 0);
}
/* fzgx:end fn_1_96630 */

/* fzgx:begin fn_1_96664 */
void fn_1_96664(void *arg0, void *arg1, void *arg2) {
    fn_1_96230(0, arg0, arg1, arg2, 1);
}
/* fzgx:end fn_1_96664 */

/* fzgx:begin fn_1_968FC */
typedef struct Fn1968FCChild Fn1968FCChild;
typedef struct Fn1968FCObject Fn1968FCObject;

struct Fn1968FCChild {
    u8 unk_00[0x08];
    u32 unk_08;
    u8 unk_0c[0x10];
    u32 unk_1c;
};

struct Fn1968FCObject {
    u8 unk_00[0x1c];
    Fn1968FCChild *unk_1c;
    void *unk_20;
};

void fn_1_968FC(Fn1968FCObject *object) {
    if (object->unk_20 != 0) {
        fn_1_12ABB4(object->unk_20);
        object->unk_20 = 0;
    }

    if (object->unk_1c != 0) {
        object->unk_1c->unk_1c = 0;
        object->unk_1c->unk_08 = 0;
        fn_1_41850(object->unk_1c);
        object->unk_1c = 0;
    }
}
/* fzgx:end fn_1_968FC */

/* fzgx:begin fn_1_96AE8 */
void fn_1_96AE8(void *base, void *arg1) {
    fn_1_96B14(base, arg1, lbl_1_bss_6E98C);
}
/* fzgx:end fn_1_96AE8 */

/* fzgx:begin fn_1_97174 */
typedef struct Fn197174Owner Fn197174Owner;
typedef struct Fn197174Root Fn197174Root;
typedef struct Fn197174Entry Fn197174Entry;

struct Fn197174Owner {
    u8 unk_00[0x14];
    u8 *base;
};

struct Fn197174Root {
    u8 unk_00[0x08];
    u16 count;
};

struct Fn197174Entry {
    u8 unk_00[0x1C];
    u32 offset;
};

extern Fn197174Root *fn_1_41BDC(void);
extern Fn197174Entry *fn_1_41B18(Fn197174Owner *owner, void *arg1, s32 index);

s32 fn_1_97174(Fn197174Owner *owner, void *arg1, void *arg2) {
    Fn197174Root *root = fn_1_41BDC();
    s32 index = 0;

    while (index < (s32)root->count) {
        Fn197174Entry *entry = fn_1_41B18(owner, arg1, index);
        if (fn_80083BCC(owner->base + entry->offset, arg2) == 0) {
            return index;
        }
        index++;
    }
    return -1;
}
/* fzgx:end fn_1_97174 */

/* fzgx:begin fn_1_9724C */
// Loads the active resource value when the initialization check has not completed.
void fn_1_9724C(void) {
    if (fn_1_98104() == 0) {
        fn_8004C698(lbl_1_bss_6EAD0.unk_0->unk_4);
    }
}
/* fzgx:end fn_1_9724C */

/* fzgx:begin fn_1_97F1C */
typedef struct {
    u8 unk_000[0x4A4];
    s32 unk_4A4;
} Fn197F1CObject;

/* Formats the indexed message and stores its parsed value in the object. */
s16 fn_1_97F1C(Fn197F1CObject *object, s16 index) {
    char buffer[0x80];

    fn_8008069C(buffer, lbl_1_data_27D90, lbl_1_data_20D1C[index]);
    object->unk_4A4 = fn_1_12A32C(buffer);
    return (s16)object->unk_4A4;
}
/* fzgx:end fn_1_97F1C */

/* fzgx:begin fn_1_98104 noprologue */
#include "types.h"
#include "rel/main_rel/globals.h"
#include "rel/main_rel/rob.h"

extern u8 lbl_1_data_27C00[0xE0];
extern u32 lbl_801A6410;
extern void fn_1_46B4(u32, u32, u8 *, u32);
extern void fn_80008BEC(void *, u32, u32);

typedef struct Fn198104Obj {
    u8 unk_00[0x1C];
    u32 value_1C;
    u8 unk_20[0x0C];
    u32 value_2C;
} Fn198104Obj;

s32 fn_1_98104(Fn198104Obj *obj) {
    Fn198104Obj *ptr = (Fn198104Obj *)((u8 *)obj + 0x488);
    u32 saved;

    if (ptr == 0) {
        return 0;
    }

    saved = ptr->value_1C;
    if (ptr->value_2C != 0) {
        fn_1_46B4(lbl_801A6410, ptr->value_2C, lbl_1_data_27C00, 0xfc0);
        ptr->value_2C = 0;
    }

    fn_80008BEC(ptr, 0, 0x30);
    ptr->value_1C = saved;
    return 1;
}
/* fzgx:end fn_1_98104 */

/* fzgx:begin fn_1_9818C */
extern u32 fn_1_12A734(u32);

s32 fn_1_9818C(u32 arg0) {
    s16 v0;
    s32 v1;
    u32 t0;
    v0 = (s16)arg0;
    v1 = v0;
    if (v0 != -1) {
    t0 = fn_1_12A734(v1);
    v1 = t0;
    v1 = 1;
    } else {
    v1 = 0;
    }
    return v1;
}
/* fzgx:end fn_1_9818C */
