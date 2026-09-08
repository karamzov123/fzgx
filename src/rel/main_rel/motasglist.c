#include "types.h"

/* fzgx:begin fn_1_4300C */
typedef struct Fn14300CObject {
    u8 pad0[2];
    u16 flags;
    f32 value;
    u8 pad1[0x38];
} Fn14300CObject;

extern void fn_1_43264(void *arg0, f32 arg1);

void fn_1_4300C(Fn14300CObject *object) {
    fn_1_43264((u8 *)object + 0x40, object->value);
    if (object->flags & 1) {
        fn_1_43264((u8 *)object + 0x58, object->value);
    }
}
/* fzgx:end fn_1_4300C */

/* fzgx:begin fn_1_43058 */
typedef struct Fn143058Entry {
    u8 pad0[0x94];
    f32 value0;
    u8 pad98[0x0C];
    f32 value1;
    u8 padA8[0x0C];
    f32 value2;
} Fn143058Entry;

typedef struct Fn143058Object {
    u16 count;
    u16 flags;
    u8 pad4[4];
    u8 *entries;
    u8 padC[0xC];
    f32 value0;
    f32 value1;
    f32 value2;
} Fn143058Object;

extern void lbl_8006DAEC(void);
extern void lbl_8006DB30(void);
extern void fn_1_43E08(Fn143058Object *object, u8 *entry, void *arg2, void *arg3, f32 value);
extern void fn_1_433E0(Fn143058Object *object);

void fn_1_43058(Fn143058Object *object, void *arg2, void *arg3, f32 value) {
    u32 offset;
    s32 i;

    lbl_8006DAEC();
    i = 0;
    offset = 0;
    for (; i < object->count; i++) {
        fn_1_43E08(object, object->entries + offset, arg2, arg3, value);
        offset += 0x18c;
    }
    object->value0 = ((Fn143058Entry *)object->entries)->value0;
    object->value1 = ((Fn143058Entry *)object->entries)->value1;
    object->value2 = ((Fn143058Entry *)object->entries)->value2;
    fn_1_433E0(object);
    lbl_8006DB30();
    object->flags &= ~0x100;
}
/* fzgx:end fn_1_43058 */

/* fzgx:begin fn_1_431B8 */
typedef enum Fn1431B8Index {
    FN1431B8_INVALID = -1
} Fn1431B8Index;

typedef struct Fn1431B8Object {
    u8 pad0[4];
    u16 flags;
    u16 index;
    u8 pad8[0xD8];
    u16 value0;
    u16 value1;
    u16 value2;
} Fn1431B8Object;

s32 fn_1_431B8(Fn1431B8Object *base, Fn1431B8Object *object) {
    Fn1431B8Object *entry;
    s32 count;

    entry = (Fn1431B8Object *)((u8 *)base + object->index * 0x18c);
    count = 0;
    while (!(entry->flags & 2)) {
        if (count == 2) {
            // The retail loop shares one exit for both termination checks.
            goto done;
        }
        if (entry->index == FN1431B8_INVALID) {
            // The retail loop shares one exit for both termination checks.
            goto done;
        }
        count++;
        if (entry->value0 != 0 || entry->value1 != 0 || entry->value2 != 0) {
            return 1;
        }
        entry = (Fn1431B8Object *)((u8 *)base + entry->index * 0x18c);
    }
done:
    return 0;
}
/* fzgx:end fn_1_431B8 */

/* fzgx:begin fn_1_451D4 */
extern u32 lbl_1_bss_384CC;

void fn_1_451D4(void) {
    lbl_1_bss_384CC = 0;
}
/* fzgx:end fn_1_451D4 */
