#include "types.h"
#include "rel/main_rel/globals.h"
#include "rel/main_rel/effect.h"

extern void fn_1_3BDC(s32 arg0);
extern u32 fn_1_3C18(s32 arg0);
extern void fn_80008BEC(void *, int, int);
extern void fn_80008BA8(void *, void *, int);
extern void fn_1_680F8(void);
extern void fn_1_68284(void);
extern void fn_1_68B68(void);
extern void fn_1_68248(void);
extern void fn_1_69BBC(void);
extern void fn_1_69BCC(void);
extern f32 lbl_1_rodata_29AC[5];
extern f32 lbl_1_rodata_2978[4];
extern void lbl_8006DCA4(void);
extern f32 lbl_1_rodata_2AF4[14];
extern s32 fn_1_54E34(void *object, f32 value);
extern void fn_1_5EB98(void);
extern void fn_1_5FEBC(void);
extern void *lbl_801A6410;
extern void fn_1_46B4(u32 value, void *field, u8 *data, s32 code);
extern f32 lbl_1_rodata_2A5C[5];
extern void fn_1_61D08(void);
extern void fn_1_61EF4(void);
extern void fn_1_620C4(void);
extern void fn_1_4730(void *arg0, void *arg1, int arg2, u8 *arg3, int arg4);
extern f32 lbl_1_rodata_2B2C[145];
extern void fn_1_638E8(void);
extern const f32 lbl_1_rodata_2A70[12];
extern void fn_1_64388(void);
extern void fn_1_652F4(void);
extern void fn_1_65748(void);
extern void fn_1_8636C(s16, void *);

extern void *lbl_801A6410;
extern void fn_1_46B4(u32 value, void *field, u8 *data, s32 code);
extern void fn_1_4730(void *arg0, void *arg1, int arg2, u8 *arg3, int arg4);
extern void lbl_8006DCA4(void);
extern s32 fn_1_54E34(void *object, f32 value);
extern void fn_1_65748(void);
extern const f32 lbl_1_rodata_2A70[12];
extern void fn_1_64388(void);
extern void *fn_1_5448C(void *);
extern void fn_1_61E60(void);
extern void fn_1_5489C(void *, void *);
extern void fn_1_862D4(s16 value, void *result);
extern void *fn_1_548AC(u32 size);

/* fzgx:begin fn_1_58D38 */
typedef struct fn_1_58D38_EffectEntry {
    s8 unk_00;
    u8 pad_01[7];
    u32 unk_08;
    s16 unk_0C;
    u8 pad_0E[0x0C];
    u16 unk_1A;
    u8 pad_1C[0xCC];
} fn_1_58D38_EffectEntry;

typedef struct fn_1_58D38_EffectState {
    fn_1_58D38_EffectEntry *unk_00;
    fn_1_58D38_EffectEntry *unk_04;
    u8 pad_08[8];
    u32 unk_10;
} fn_1_58D38_EffectState;

extern void fn_1_62360(fn_1_58D38_EffectEntry *arg0);

void fn_1_58D38(void) {
    fn_1_58D38_EffectState *state;
    s32 count;
    fn_1_58D38_EffectEntry *entry;
    u32 max_count;

    state = (fn_1_58D38_EffectState *)&lbl_1_bss_6C848;
    if (state->unk_00 != 0) {
        fn_1_3BDC(9);

        entry = state->unk_00;
        count = 190;
        do {
            if (entry->unk_0C == 25 &&
                (s32)entry->unk_00 != 0 &&
                entry->unk_1A != (s16)0 &&
                (entry->unk_08 & ~0x7fffffff) == 0) {
                fn_1_62360(entry);
            }
            count--;
            entry++;
        } while (count > 0);

        entry = state->unk_04;
        count = 200;
        do {
            if (entry->unk_0C == 25 &&
                (s32)entry->unk_00 != 0 &&
                entry->unk_1A != (s16)0 &&
                (entry->unk_08 & ~0x7fffffff) == 0) {
                fn_1_62360(entry);
            }
            count--;
            entry++;
        } while (count > 0);

        count = fn_1_3C18(9);
        max_count = state->unk_10;
        if (count > max_count) {
            max_count = count;
        }
        state->unk_10 = max_count;
    }
}
/* fzgx:end fn_1_58D38 */

/* fzgx:begin fn_1_59078 */
extern int fn_1_3FC8C(void);
extern int fn_1_3FC58(void);
extern void *memcpy(void *, const void *, u32);

s16 fn_1_59078(Obj_1_bss_6C84C_Target *arg0) {
    Obj_1_bss_6C84C_Target *obj;
    s32 i;

    if (fn_1_3FC8C() != 0) {
        return -1;
    }
    if (fn_1_3FC58() != 0) {
        return -1;
    }

    obj = lbl_1_bss_6C84C;
    for (i = 0; i < 0xbe; i++, obj++) {
        if ((s8)obj->unk_0 == 0) {
            obj->unk_0 = 1;
            // Skip the exhaustion path when a free slot is found.
            goto slot_found;
        }
    }
    i = -1;
slot_found:
    if (i < 0) {
        return -1;
    }

    obj = lbl_1_bss_6C84C + i;
    memcpy(obj, arg0, 0xe8);
    obj->unk_0 = 1;
    obj->unk_2 = i;
    ((void (**)(Obj_1_bss_6C84C_Target *))lbl_1_data_1D1D8)[obj->unk_C](obj);
    obj->unk_4 = lbl_1_bss_6C850.unk_0;
    lbl_1_bss_6C850.unk_0++;
    if (lbl_1_bss_6C850.unk_0 < 0) {
        lbl_1_bss_6C850.unk_0 = 0;
    }
    return obj->unk_4;
}
/* fzgx:end fn_1_59078 */

/* fzgx:begin fn_1_59290 */
void fn_1_59290(void) {
    u8 buffer1[0xe8];
    u8 buffer2[0xe8];
    int count;
    Obj_1_bss_6C84C_Target *effect;
    Obj_1_bss_6C84C_Target *effects;

    effects = *(Obj_1_bss_6C84C_Target **)(void *)&lbl_1_bss_6C848;
    effect = effects;
    count = 0xbe;
    for (; count > 0; count--, effect++) {
        if ((s8)effect->unk_0 != 0) {
            if (effect->unk_C == 1) {
                fn_80008BEC(buffer1, 0, 0xe8);
                *(s16 *)(buffer1 + 0x4) = effect->unk_4;
                buffer1[0] = effect->unk_0;
                *(s16 *)(buffer1 + 0xc) = effect->unk_C;
                *(u32 *)(buffer1 + 0x38) = effect->unk_38;
                *(u32 *)(buffer1 + 0x34) = effect->unk_34;
                *(u16 *)(buffer1 + 0x1a) = effect->unk_1A;
                *(s16 *)(buffer1 + 0x18) = effect->unk_18;
                fn_80008BEC(effect, 0, 0xe8);
                fn_80008BA8(effect, buffer1, 0xe8);
            } else if (effect->unk_C != 4) {
                effect->unk_0 = 3;
                effect->unk_8 |= (u32)1 << 31;
            }
        }
    }

    effects = lbl_1_bss_6C84C;
    effect = effects;
    count = 0xc8;
    for (; count > 0; count--, effect++) {
        if ((s8)effect->unk_0 != 0) {
            if (effect->unk_C == 1) {
                fn_80008BEC(buffer2, 0, 0xe8);
                *(s16 *)(buffer2 + 0x4) = effect->unk_4;
                buffer2[0] = effect->unk_0;
                *(s16 *)(buffer2 + 0xc) = effect->unk_C;
                *(u32 *)(buffer2 + 0x38) = effect->unk_38;
                *(u32 *)(buffer2 + 0x34) = effect->unk_34;
                *(u16 *)(buffer2 + 0x1a) = effect->unk_1A;
                *(s16 *)(buffer2 + 0x18) = effect->unk_18;
                fn_80008BEC(effect, 0, 0xe8);
                fn_80008BA8(effect, buffer2, 0xe8);
            } else if (effect->unk_C != 4) {
                effect->unk_0 = 3;
                effect->unk_8 |= (u32)1 << 31;
            }
        }
    }
}
/* fzgx:end fn_1_59290 */

/* fzgx:begin fn_1_5942C */
// fn_1_5942C: wrapper that calls fn_1_680F8
void fn_1_5942C(void) {
    fn_1_680F8();
}
/* fzgx:end fn_1_5942C */

/* fzgx:begin fn_1_5944C */
void fn_1_5944C(void) {
    fn_1_68284();
}
/* fzgx:end fn_1_5944C */

/* fzgx:begin fn_1_5946C */
void fn_1_5946C(void) {
    fn_1_68B68();
}
/* fzgx:end fn_1_5946C */

/* fzgx:begin fn_1_5948C */
void fn_1_5948C(void) {
    fn_1_68248();
}
/* fzgx:end fn_1_5948C */

/* fzgx:begin fn_1_594AC */
void fn_1_594AC(void) {
    fn_1_69BBC();
}
/* fzgx:end fn_1_594AC */

/* fzgx:begin fn_1_594CC */
void fn_1_594CC(void) {
    fn_1_69BCC();
}
/* fzgx:end fn_1_594CC */

/* fzgx:begin fn_1_594EC */
// fn_1_594EC: empty in retail (single blr).
void fn_1_594EC(void) {
}
/* fzgx:end fn_1_594EC */

/* fzgx:begin fn_1_594F0 */
// fn_1_594F0: empty in retail (single blr).
void fn_1_594F0(void) {
}
/* fzgx:end fn_1_594F0 */

/* fzgx:begin fn_1_594F4 */
// fn_1_594F4: empty in retail (single blr).
void fn_1_594F4(void) {
}
/* fzgx:end fn_1_594F4 */

/* fzgx:begin fn_1_594F8 */
// fn_1_594F8: empty in retail (single blr).
void fn_1_594F8(void) {
}
/* fzgx:end fn_1_594F8 */

/* fzgx:begin fn_1_594FC */
// fn_1_594FC: empty in retail (single blr).
void fn_1_594FC(void) {
}
/* fzgx:end fn_1_594FC */

/* fzgx:begin fn_1_59500 */
// fn_1_59500: empty in retail (single blr).
void fn_1_59500(void) {
}
/* fzgx:end fn_1_59500 */

/* fzgx:begin fn_1_59504 */
// fn_1_59504: empty in retail (single blr).
void fn_1_59504(void) {
}
/* fzgx:end fn_1_59504 */

/* fzgx:begin fn_1_59508 */
// fn_1_59508: empty in retail (single blr).
void fn_1_59508(void) {
}
/* fzgx:end fn_1_59508 */

/* fzgx:begin fn_1_5950C */
// fn_1_5950C: empty in retail (single blr).
void fn_1_5950C(void) {
}
/* fzgx:end fn_1_5950C */

/* fzgx:begin fn_1_59510 */
// fn_1_59510: empty in retail (single blr).
void fn_1_59510(void) {
}
/* fzgx:end fn_1_59510 */

/* fzgx:begin fn_1_59A70 */
// fn_1_59A70: empty in retail (single blr).
void fn_1_59A70(void) {
}
/* fzgx:end fn_1_59A70 */

/* fzgx:begin fn_1_59CC4 */
// fn_1_59CC4: empty in retail (single blr).
void fn_1_59CC4(void) {
}
/* fzgx:end fn_1_59CC4 */

/* fzgx:begin fn_1_5A8CC */
// fn_1_5A8CC: empty in retail (single blr).
void fn_1_5A8CC(void) {
}
/* fzgx:end fn_1_5A8CC */

/* fzgx:begin fn_1_5A8D0 */
typedef struct {
    u8 pad[0x10];
    s32 field_10;
} Fn1_5A8D0Data;

void fn_1_5A8D0(Fn1_5A8D0Data *data) {
    data->field_10 = 2;
}
/* fzgx:end fn_1_5A8D0 */

/* fzgx:begin fn_1_5ABC4 */
// fn_1_5ABC4: empty in retail (single blr).
void fn_1_5ABC4(void) {
}
/* fzgx:end fn_1_5ABC4 */

/* fzgx:begin fn_1_5ABC8 */
// fn_1_5ABC8: empty in retail (single blr).
void fn_1_5ABC8(void) {
}
/* fzgx:end fn_1_5ABC8 */

/* fzgx:begin fn_1_5B074 */
// fn_1_5B074: empty in retail (single blr).
void fn_1_5B074(void) {
}
/* fzgx:end fn_1_5B074 */

/* fzgx:begin fn_1_5B3CC */
// fn_1_5B3CC: empty in retail (single blr).
void fn_1_5B3CC(void) {
}
/* fzgx:end fn_1_5B3CC */

/* fzgx:begin fn_1_5B6F0 */
// fn_1_5B6F0: empty in retail (single blr).
void fn_1_5B6F0(void) {
}
/* fzgx:end fn_1_5B6F0 */

/* fzgx:begin fn_1_5B780 */
extern const f32 lbl_1_rodata_29A4;

typedef struct Fn15B780 {
    u8 state;
    u8 _pad01[0x0f];
    int current;
    int maximum;
    u8 _pad18[0x94];
    s16 step;
    u8 _padAE[0x04];
    f32 result;
} Fn15B780;

void fn_1_5B780(Fn15B780 *effect) {
    effect->state = 2;
    if (effect->current <= effect->maximum) {
        effect->step += 2;
        if (effect->step >= 14) {
            effect->step = 14;
        }
    }
    effect->result = lbl_1_rodata_29A4 * (f32)effect->current;
}
/* fzgx:end fn_1_5B780 */

/* fzgx:begin fn_1_5BF6C */
// fn_1_5BF6C: empty in retail (single blr).
void fn_1_5BF6C(void) {
}
/* fzgx:end fn_1_5BF6C */

/* fzgx:begin fn_1_5BFF0 */
extern f32 lbl_1_rodata_29A4;
extern f64 lbl_1_rodata_2988;

struct fn_1_5BFF0_Arg0 {
    u8 unk_0;
    u8 pad_1[0xF];
    u32 unk_10;
    u8 pad_14[0x98];
    s16 unk_AC;
    u8 pad_AE[0x6];
    f32 unk_B4;
};

void fn_1_5BFF0(struct fn_1_5BFF0_Arg0 *arg0) {
    arg0->unk_0 = 2;
    arg0->unk_AC += 2;
    if (arg0->unk_AC >= 15) {
    arg0->unk_AC = 15;
    }
    arg0->unk_B4 = (lbl_1_rodata_29A4 * (f32)(s32)arg0->unk_10);
}
/* fzgx:end fn_1_5BFF0 */

/* fzgx:begin fn_1_5C780 */
// fn_1_5C780: empty in retail (single blr).
void fn_1_5C780(void) {
}
/* fzgx:end fn_1_5C780 */

/* fzgx:begin fn_1_5D010 */
// fn_1_5D010: empty in retail (single blr).
void fn_1_5D010(void) {
}
/* fzgx:end fn_1_5D010 */

/* fzgx:begin fn_1_5D014 */
typedef struct Fn1_5D014Object {
    u8 pad[0xae];
    s16 field_ae;
} Fn1_5D014Object;

void fn_1_5D014(Fn1_5D014Object *obj) {
    obj->field_ae = 0;
}
/* fzgx:end fn_1_5D014 */

/* fzgx:begin fn_1_5D374 */
// fn_1_5D374: empty in retail (single blr).
void fn_1_5D374(void) {
}
/* fzgx:end fn_1_5D374 */

/* fzgx:begin fn_1_5D4FC */
// fn_1_5D4FC: empty in retail (single blr).
void fn_1_5D4FC(void) {
}
/* fzgx:end fn_1_5D4FC */

/* fzgx:begin fn_1_5D88C */
struct Fn15D88C {
    u8 _pad08[0x08];
    u32 flags;
    u8 _pad0c[0x1c];
    f32 value28;
    u8 _pad2c[0x08];
    u32 enabled;
    u8 _pad38[0x1c];
    u16 value54;
    u16 value56;
    u16 value58;
    u8 _pad5a[0x5a];
    f32 value_b4;
};

void fn_1_5D88C(struct Fn15D88C *self) {
    self->value_b4 = lbl_1_rodata_29AC[0];
    if (self->enabled != 0) {
        f32 value28;
        lbl_1_data_1D628 = lbl_1_data_1D628 * 1103515245 + 12345;
        value28 = lbl_1_rodata_2978[0];
        self->value54 = (u16)((lbl_1_data_1D628 >> 16) & 0x7fff);
        lbl_1_data_1D628 = lbl_1_data_1D628 * 1103515245 + 12345;
        self->value56 = (u16)((lbl_1_data_1D628 >> 16) & 0x7fff);
        lbl_1_data_1D628 = lbl_1_data_1D628 * 1103515245 + 12345;
        self->value58 = (u16)((lbl_1_data_1D628 >> 16) & 0x7fff);
        self->value28 = value28;
    }
    self->flags |= 0x40000000;
}
/* fzgx:end fn_1_5D88C */

/* fzgx:begin fn_1_5D918 */
// fn_1_5D918: empty in retail (single blr).
void fn_1_5D918(void) {
}
/* fzgx:end fn_1_5D918 */

/* fzgx:begin fn_1_5EB08 */
#include "types.h"
#include "rel/main_rel/globals.h"
#include "rel/main_rel/effect.h"

typedef struct FnObj {
    u8 pad18[0x18];
    s16 value;
    u8 pad3c[0x22];
    f32 field3c;
} FnObj;

typedef struct FnLocal {
    u8 data[0x10];
} FnLocal;

typedef struct FnNode {
    u8 pad4[4];
    void (*callback)(void);
    FnObj *object;
} FnNode;

void fn_1_5EB08(FnObj *object) {
    FnLocal local;
    FnNode *node;
    FnNode *allocated;

    fn_1_862D4(object->value, &local);
    lbl_8006DCA4();
    if (fn_1_54E34(&object->field3c, lbl_1_rodata_2AF4[0])) {
        node = fn_1_5448C(&local);
        allocated = fn_1_548AC(0xc);
        if (allocated != 0) {
            allocated->callback = fn_1_5EB98;
            allocated->object = object;
            fn_1_5489C(node, allocated);
        }
    }
}
/* fzgx:end fn_1_5EB08 */

/* fzgx:begin fn_1_5F5C4 */
// fn_1_5F5C4: empty in retail (single blr).
void fn_1_5F5C4(void) {
}
/* fzgx:end fn_1_5F5C4 */

/* fzgx:begin fn_1_5FE24 */
// fn_1_5FE24: empty in retail (single blr).
void fn_1_5FE24(void) {
}
/* fzgx:end fn_1_5FE24 */

/* fzgx:begin fn_1_5FE28 */
// fn_1_5FE28: empty in retail (single blr).
void fn_1_5FE28(void) {
}
/* fzgx:end fn_1_5FE28 */

/* fzgx:begin fn_1_5FE2C */
// fn_1_5FE2C: empty in retail (single blr).
void fn_1_5FE2C(void) {
}
/* fzgx:end fn_1_5FE2C */

/* fzgx:begin fn_1_5FE30 */
#include "types.h"
#include "rel/main_rel/globals.h"
#include "rel/main_rel/effect.h"

typedef struct {
    u8 unk[0x18];
    s16 value;
    u8 unk1A[0x0E];
    f32 rate;
    u8 unk2C[0x10];
    u8 field3C[1];
} fn_1_5FE30_FZeroObject;

typedef struct {
    u8 unk0[4];
    void (*callback)(void);
    fn_1_5FE30_FZeroObject *owner;
} FZeroEvent;

void fn_1_5FE30(fn_1_5FE30_FZeroObject *object) {
    u8 result[8];
    void *callback;
    FZeroEvent *event;

    fn_1_862D4(object->value, result);
    lbl_8006DCA4();
    if (fn_1_54E34(object->field3C, object->rate) != 0) {
        callback = fn_1_5448C(result);
        event = (FZeroEvent *)fn_1_548AC(12);
        if (event != 0) {
            event->callback = fn_1_5FEBC;
            event->owner = object;
            fn_1_5489C(callback, event);
        }
    }
}
/* fzgx:end fn_1_5FE30 */

/* fzgx:begin fn_1_5FFAC */
// fn_1_5FFAC: empty in retail (single blr).
void fn_1_5FFAC(void) {
}
/* fzgx:end fn_1_5FFAC */

/* fzgx:begin fn_1_60170 */
typedef struct {
    u8 unk[0x38];
    void *field38;
} fn_1_60170_FZeroObject;

// Submit the effect data when this object has an associated field.
void fn_1_60170(fn_1_60170_FZeroObject *object) {
    if (object->field38 != 0) {
        fn_1_46B4((u32)lbl_801A6410, object->field38, lbl_1_data_1D62C, 0x1261);
    }
}
/* fzgx:end fn_1_60170 */

/* fzgx:begin fn_1_60C70 */
// fn_1_60C70: empty in retail (single blr).
void fn_1_60C70(void) {
}
/* fzgx:end fn_1_60C70 */

/* fzgx:begin fn_1_60C74 */
struct Effect {
    u8 _pad[0x28];
    f32 field_28;
};

void fn_1_60C74(struct Effect *effect) {
    effect->field_28 = lbl_1_rodata_2A5C[0];
}
/* fzgx:end fn_1_60C74 */

/* fzgx:begin fn_1_60C84 */
// fn_1_60C84: empty in retail (single blr).
void fn_1_60C84(void) {
}
/* fzgx:end fn_1_60C84 */

/* fzgx:begin fn_1_60F7C */
// fn_1_60F7C: empty in retail (single blr).
void fn_1_60F7C(void) {
}
/* fzgx:end fn_1_60F7C */

/* fzgx:begin fn_1_60F80 */
// fn_1_60F80: empty in retail (single blr).
void fn_1_60F80(void) {
}
/* fzgx:end fn_1_60F80 */

/* fzgx:begin fn_1_61760 */
struct Fn1_61760Object {
    u8 _pad38[0x38];
    void *effect_resource;
};

// Dispatches the object's effect resource when one is available.
void fn_1_61760(struct Fn1_61760Object *object) {
    if (object->effect_resource != 0) {
        fn_1_46B4((u32)lbl_801A6410, object->effect_resource, lbl_1_data_1D62C, 0x16c1);
    }
}
/* fzgx:end fn_1_61760 */

/* fzgx:begin fn_1_61C84 */
// fn_1_61C84: empty in retail (single blr).
void fn_1_61C84(void) {
}
/* fzgx:end fn_1_61C84 */

/* fzgx:begin fn_1_61C88 */
void fn_1_61C88(void) {
    fn_1_61D08();
}
/* fzgx:end fn_1_61C88 */

/* fzgx:begin fn_1_61CA8 */
void fn_1_61CA8(void) {
    fn_1_61EF4();
}
/* fzgx:end fn_1_61CA8 */

/* fzgx:begin fn_1_61CC8 */
void fn_1_61CC8(void) {
    fn_1_620C4();
}
/* fzgx:end fn_1_61CC8 */

/* fzgx:begin fn_1_61CE8 */
void fn_1_61CE8(void) {
    fn_1_61E60();
}
/* fzgx:end fn_1_61CE8 */

/* fzgx:begin fn_1_61E60 noprologue */
#include "types.h"
#include "rel/main_rel/globals.h"
#include "rel/main_rel/effect.h"

extern u8 lbl_1_data_1D62C[0x94];
extern void *lbl_801A6410;
extern void fn_1_46B4(u32 value, void *field, u8 *data, s32 code);
extern void fn_1_4730(void *arg0, void *arg1, int arg2, u8 *arg3, int arg4);

typedef struct {
    u8 pad20[0x20];
    void *unk_20;
} Fn1_61E60Node;

typedef struct {
    u8 pad38[0x38];
    Fn1_61E60Node *unk_38;
} Fn1_61E60Object;

// Releases the effect resources and clears the active effect references.
int fn_1_61E60(Fn1_61E60Object *object) {
    Fn1_61E60Node *node = object->unk_38;

    if (node != 0) {
        fn_1_4730(lbl_801A6410, node->unk_20, 1, lbl_1_data_1D62C, 0x17D5);
        node->unk_20 = 0;
        fn_1_46B4((u32)lbl_801A6410, object->unk_38, lbl_1_data_1D62C, 0x17D8);
        object->unk_38 = 0;
    }

    return 1;
}
/* fzgx:end fn_1_61E60 */

/* fzgx:begin fn_1_620C4 */
// fn_1_620C4: empty in retail (single blr).
void fn_1_620C4(void) {
}
/* fzgx:end fn_1_620C4 */

/* fzgx:begin fn_1_6312C */
// fn_1_6312C: empty in retail (single blr).
void fn_1_6312C(void) {
}
/* fzgx:end fn_1_6312C */

/* fzgx:begin fn_1_63130 */
struct fn_1_63130_obj {
    u8 unk_00[0xAE];
    s16 unk_AE;
};

void fn_1_63130(struct fn_1_63130_obj *obj) {
    obj->unk_AE = 0;
}
/* fzgx:end fn_1_63130 */

/* fzgx:begin fn_1_633BC */
// fn_1_633BC: empty in retail (single blr).
void fn_1_633BC(void) {
}
/* fzgx:end fn_1_633BC */

/* fzgx:begin fn_1_63514 */
// fn_1_63514: empty in retail (single blr).
void fn_1_63514(void) {
}
/* fzgx:end fn_1_63514 */

/* fzgx:begin fn_1_63858 */
struct LocalData {
    u8 data[0x10];
};

struct Event {
    u8 data[4];
    void (*callback)(void);
    void *owner;
};

struct Object {
    u8 data[0x18];
    s16 value;
};

void fn_1_63858(struct Object *object) {
    struct LocalData local;
    struct Event *event;
    void *target;

    fn_1_862D4(object->value, &local);
    lbl_8006DCA4();
    if (fn_1_54E34(&local, lbl_1_rodata_2B2C[0]) != 0) {
        target = fn_1_5448C(&local);
        event = (struct Event *)fn_1_548AC(0xc);
        if (event != 0) {
            event->callback = fn_1_638E8;
            event->owner = object;
            fn_1_5489C(target, event);
        }
    }
}
/* fzgx:end fn_1_63858 */

/* fzgx:begin fn_1_64098 */
typedef struct Fn164098Object {
    u8 pad[0xae];
    u16 value;
} Fn164098Object;

void fn_1_64098(Fn164098Object *object) {
    object->value = 0;
}
/* fzgx:end fn_1_64098 */

/* fzgx:begin fn_1_642E8 */
typedef struct fn_1_642E8_EffectState {
    char bytes[0x14];
} fn_1_642E8_EffectState;

typedef struct fn_1_642E8_EffectObject {
    char pad0[0x28];
    f32 value;
    char pad2c[0x10];
    fn_1_642E8_EffectState state;
} fn_1_642E8_EffectObject;

typedef struct EffectNode {
    char pad0[4];
    void (*callback)(void);
    void *owner;
} EffectNode;

void fn_1_642E8(fn_1_642E8_EffectObject *self) {
    f32 value;
    void *state;
    EffectNode *node;

    value = self->value / lbl_1_rodata_2A70[0];
    lbl_8006DCA4();
    if (fn_1_54E34(&self->state, value)) {
        state = fn_1_5448C(&self->state);
        node = fn_1_548AC(0xc);
        if (node != 0) {
            node->callback = fn_1_64388;
            node->owner = self;
            fn_1_5489C(state, node);
        }
    }
}
/* fzgx:end fn_1_642E8 */

/* fzgx:begin fn_1_645C8 */
// fn_1_645C8: empty in retail (single blr).
void fn_1_645C8(void) {
}
/* fzgx:end fn_1_645C8 */

/* fzgx:begin fn_1_645CC */
// fn_1_645CC: empty in retail (single blr).
void fn_1_645CC(void) {
}
/* fzgx:end fn_1_645CC */

/* fzgx:begin fn_1_645D0 */
// fn_1_645D0: empty in retail (single blr).
void fn_1_645D0(void) {
}
/* fzgx:end fn_1_645D0 */

/* fzgx:begin fn_1_648D4 */
// fn_1_648D4: empty in retail (single blr).
void fn_1_648D4(void) {
}
/* fzgx:end fn_1_648D4 */

/* fzgx:begin fn_1_648D8 */
// fn_1_648D8: empty in retail (single blr).
void fn_1_648D8(void) {
}
/* fzgx:end fn_1_648D8 */

/* fzgx:begin fn_1_65268 */
typedef struct {
    u32 pad_00;
    void (*vtable)(void);
    void *owner;
} Event;

typedef struct {
    u8 pad_00[0x18];
    s16 value;
    u8 pad_1a[0xe];
    f32 amount;
    u8 pad_2c[0x10];
    u8 embedded[1];
} Object;

void fn_1_65268(Object *object) {
    u8 local[4];
    void *result;
    Event *event;

    fn_1_862D4(object->value, local);
    lbl_8006DCA4();
    if (fn_1_54E34(&object->embedded[0], object->amount) != 0) {
        result = fn_1_5448C(local);
        event = (Event *)fn_1_548AC(0xc);
        if (event != 0) {
            event->vtable = fn_1_652F4;
            event->owner = object;
            fn_1_5489C(result, event);
        }
    }
}
/* fzgx:end fn_1_65268 */

/* fzgx:begin fn_1_65420 */
// fn_1_65420: empty in retail (single blr).
void fn_1_65420(void) {
}
/* fzgx:end fn_1_65420 */

/* fzgx:begin fn_1_656C8 */
typedef struct {
    u32 unk_00;
    void (*unk_04)(void);
    void *unk_08;
} EffectEvent;

typedef struct {
    u8 unk_00[0x28];
    f32 unk_28;
    u8 unk_2c[0x10];
    u8 unk_3c[1];
} fn_1_656C8_EffectObject;

// Initializes the effect and queues an event when its embedded state is ready.
void fn_1_656C8(fn_1_656C8_EffectObject *object) {
    void *result;
    EffectEvent *event;

    lbl_8006DCA4();
    if (fn_1_54E34(&object->unk_3c, object->unk_28) != 0) {
        result = fn_1_5448C(&object->unk_3c);
        event = (EffectEvent *)fn_1_548AC(0xc);
        if (event != 0) {
            event->unk_04 = fn_1_65748;
            event->unk_08 = object;
            fn_1_5489C(result, event);
        }
    }
}
/* fzgx:end fn_1_656C8 */

/* fzgx:begin fn_1_65AAC */
// fn_1_65AAC: empty in retail (single blr).
void fn_1_65AAC(void) {
}
/* fzgx:end fn_1_65AAC */

/* fzgx:begin fn_1_65B58 */
// fn_1_65B58: empty in retail (single blr).
void fn_1_65B58(void) {
}
/* fzgx:end fn_1_65B58 */

/* fzgx:begin fn_1_65E58 */
typedef struct Effect {
    u8 _pad_00[0x10];
    int field_10;
    u8 _pad_14[0x4];
    s16 field_18;
    u8 _pad_1a[0x9e];
} Effect;

void fn_1_65E58(Effect *effect) {
    effect->field_10 = 10;
    fn_1_8636C(effect->field_18, (u8 *)effect + 0xb8);
}
/* fzgx:end fn_1_65E58 */

/* fzgx:begin fn_1_65E88 */
// fn_1_65E88: empty in retail (single blr).
void fn_1_65E88(void) {
}
/* fzgx:end fn_1_65E88 */

/* fzgx:begin fn_1_662D4 */
// fn_1_662D4: empty in retail (single blr).
void fn_1_662D4(void) {
}
/* fzgx:end fn_1_662D4 */

/* fzgx:begin fn_1_6742C */
// fn_1_6742C: empty in retail (single blr).
void fn_1_6742C(void) {
}
/* fzgx:end fn_1_6742C */

/* fzgx:begin fn_1_6766C */
typedef struct {
    u8 padding[0xb4];
    f32 value;
} Fn6766CObject;

void fn_1_6766C(Fn6766CObject *object) {
    object->value = lbl_1_rodata_2978[0];
}
/* fzgx:end fn_1_6766C */

/* fzgx:begin fn_1_6767C */
// fn_1_6767C: empty in retail (single blr).
void fn_1_6767C(void) {
}
/* fzgx:end fn_1_6767C */

/* fzgx:begin fn_1_68054 */
typedef struct fn_1_68054_EffectObject {
    u8 pad0[0x28];
    f32 value;
    u8 pad1[0x10];
    u8 subobject;
} fn_1_68054_EffectObject;

typedef struct fn_1_68054_EffectEntry {
    u8 pad0[4];
    void (*callback)(void);
    fn_1_68054_EffectObject *owner;
} fn_1_68054_EffectEntry;

// Advances the effect and queues its completion callback when it finishes.
void fn_1_68054(fn_1_68054_EffectObject *effect) {
    f32 progress;
    void *source;
    fn_1_68054_EffectEntry *completion;

    progress = effect->value / lbl_1_rodata_2A70[0];
    lbl_8006DCA4();

    if (fn_1_54E34(&effect->subobject, progress) == 0) {
        return;
    }

    source = fn_1_5448C(&effect->subobject);
    completion = (fn_1_68054_EffectEntry *)fn_1_548AC(0xc);
    if (completion == 0) {
        return;
    }

    completion->callback = fn_1_64388;
    completion->owner = effect;
    fn_1_5489C(source, completion);
}
/* fzgx:end fn_1_68054 */

/* fzgx:begin fn_1_680F4 */
// fn_1_680F4: empty in retail (single blr).
void fn_1_680F4(void) {
}
/* fzgx:end fn_1_680F4 */
