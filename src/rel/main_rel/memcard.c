#include "types.h"

/* fzgx:begin fn_1_AB598 */
extern void fn_8002D634(u8 byte_val, int value);

typedef struct {
    u8 pad[0x2];
    u16 field_0x2;
} OffsetStruct;

typedef struct {
    u8 byte_0;
    u8 pad[0x23];
    OffsetStruct *ptr_0x24;
} ArgStruct;

void fn_1_AB598(ArgStruct *arg) {
    arg->ptr_0x24->field_0x2 = 300;
    fn_8002D634(arg->byte_0, 0);
}
/* fzgx:end fn_1_AB598 */

/* fzgx:begin fn_1_AB5CC */
extern int fn_8002A958(u8 byte);

typedef struct {
    u8 pad[0x2];
    u16 field_0x2;
    s32 field_0x4;
} OffsetStruct;

typedef struct {
    u8 byte_0;
    u8 pad[0x23];
    OffsetStruct *ptr_0x24;
} ArgStruct;

// fn_1_AB5CC: main_rel .text:0x000AB5CC size 0x50
// Load byte from arg, call fn_8002A958, store result at struct offset 0x4, clear offset 0x2 if not -1.

void fn_1_AB5CC(ArgStruct *arg) {
    u8 byte_val = arg->byte_0;
    int result = fn_8002A958(byte_val);
    
    OffsetStruct *s = arg->ptr_0x24;
    s->field_0x4 = result;
    
    s = arg->ptr_0x24;
    if (s->field_0x4 != -1) {
        s->field_0x2 = 0;
    }
}
/* fzgx:end fn_1_AB5CC */

/* fzgx:begin fn_1_AB75C */
extern int fn_8002C0A0(u8 byte);
extern void fn_8002E90C(u8 byte, int value);
extern f32 lbl_1_rodata_4CA8;

typedef struct {
    u8 pad[0x2];
    u16 field_0x2;
} OffsetStruct;

typedef struct {
    u8 byte_0;
    u8 pad_0x1[0x23];
    OffsetStruct *ptr_0x24;
    u8 pad_0x28[0x2];
    u8 flags_0x2a;
    u8 pad_0x2b[0x5];
    s32 field_0x30;
    u8 pad_0x34[0x4];
    f32 field_0x38;
} ArgStruct;

// fn_1_AB75C: main_rel .text:0x000AB75C size 0x68
void fn_1_AB75C(ArgStruct *arg) {
    arg->ptr_0x24->field_0x2 = 0x12c;
    arg->field_0x30 = fn_8002C0A0(arg->byte_0);
    arg->field_0x38 = lbl_1_rodata_4CA8;
    arg->flags_0x2a |= 2;
    fn_8002E90C(arg->byte_0, 0);
}
/* fzgx:end fn_1_AB75C */

/* fzgx:begin fn_1_AB93C */
extern int fn_8002A958(u8 byte);

typedef struct {
    u8 pad[0x2];
    u16 field_0x2;
    s32 field_0x4;
} OffsetStruct;

typedef struct {
    u8 byte_0;
    u8 pad[0x23];
    OffsetStruct *ptr_0x24;
} ArgStruct;

// fn_1_AB93C: main_rel .text:0x000AB93C size 0x50
void fn_1_AB93C(ArgStruct *arg) {
    u8 byte_val = arg->byte_0;
    int result = fn_8002A958(byte_val);

    OffsetStruct *s = arg->ptr_0x24;
    s->field_0x4 = result;

    s = arg->ptr_0x24;
    if (s->field_0x4 != -1) {
        s->field_0x2 = 0;
    }
}
/* fzgx:end fn_1_AB93C */

/* fzgx:begin fn_1_ABA24 */
extern s32 fn_8002A988(u8 byte_val, void *arg1, void *arg2);

typedef struct {
    u8 pad[0x2];
    u16 field_0x2;
    s32 field_0x4;
    s32 field_0x8;
    u8 pad_0xc[0x4];
    u8 data[0x8];
} OffsetStruct;

typedef struct {
    u8 byte_0;
    u8 pad[0x23];
    OffsetStruct *ptr_0x24;
} ArgStruct;

void fn_1_ABA24(ArgStruct *arg) {
    s32 result = fn_8002A988(arg->byte_0, arg->ptr_0x24->data,
        &arg->ptr_0x24->data[0x4]);
    arg->ptr_0x24->field_0x4 = result;
    if (arg->ptr_0x24->field_0x4 == 0) {
        arg->ptr_0x24->field_0x8 |= 0x80;
    }
    if (arg->ptr_0x24->field_0x4 != -1) {
        arg->ptr_0x24->field_0x2 = 0;
    }
}
/* fzgx:end fn_1_ABA24 */

/* fzgx:begin fn_1_ABA9C */
typedef struct {
    u8 pad[0x2];
    u16 field_0x2;
} OffsetStruct;

typedef struct {
    u8 pad[0x24];
    OffsetStruct *ptr_0x24;
} ArgStruct;

void fn_1_ABA9C(ArgStruct *arg) {
    arg->ptr_0x24->field_0x2 = 0x12c;
}
/* fzgx:end fn_1_ABA9C */

/* fzgx:begin fn_1_ABAAC */
extern void fn_80083DB0(void *dest, void *src);
extern void fn_80083D40(void *arg, void *data);
extern s32 fn_8002EBD4(u8 byte_val, void *arg1, void *arg2);
extern u8 lbl_1_data_3C7C4[216];

typedef struct {
    u8 pad[0x2];
    u16 field_0x2;
    s32 field_0x4;
} ResultStruct;

typedef struct {
    u8 byte_0;
    u8 pad_0x1[0xf];
    u8 data_0x10[0x14];
    ResultStruct *ptr_0x24;
} ArgStruct;

void fn_1_ABAAC(ArgStruct *arg) {
    u8 local[0x40];

    fn_80083DB0(local, (u8 *)arg->ptr_0x24 + 0x98);
    fn_80083D40(local, lbl_1_data_3C7C4);
    arg->ptr_0x24->field_0x4 =
        fn_8002EBD4(arg->byte_0, local, arg->data_0x10);
    if (arg->ptr_0x24->field_0x4 != -1) {
        arg->ptr_0x24->field_0x2 = 0;
    }
}
/* fzgx:end fn_1_ABAAC */

/* fzgx:begin fn_1_ABDA8 */
typedef struct {
    u8 pad[0x2];
    u16 field_0x2;
} OffsetStruct;

typedef struct {
    u8 pad[0x24];
    OffsetStruct *ptr_0x24;
} ArgStruct;

void fn_1_ABDA8(ArgStruct *arg) {
    arg->ptr_0x24->field_0x2 = 0x12c;
}
/* fzgx:end fn_1_ABDA8 */

/* fzgx:begin fn_1_AC294 */
typedef struct Fn1AC294State {
    u8 pad_00[0x2];
    u16 value;
} Fn1AC294State;

typedef struct Fn1AC294Target {
    u8 id;
    u8 pad_01[0x13];
    void *arg;
    u8 pad_18[0xC];
    Fn1AC294State *state;
} Fn1AC294Target;

extern void fn_8002FF80(u8 id, void *arg, void *data, int zero);

void fn_1_AC294(Fn1AC294Target *target) {
    target->state->value = 300;
    fn_8002FF80(target->id, target->arg, (u8 *)target->state + 0x20, 0);
}
/* fzgx:end fn_1_AC294 */

/* fzgx:begin fn_1_AC328 */
typedef struct Fn1AC328State {
    u8 pad_00[0x2];
    u16 value;
    u8 pad_04[0x94];
} Fn1AC328State;

typedef struct Fn1AC328Target {
    u8 id;
    u8 pad_01[0x23];
    Fn1AC328State *state;
} Fn1AC328Target;

extern u8 lbl_1_data_3C7C4[216];

extern void fn_80083DB0(void *dst, void *src);
extern void fn_80083D40(void *dst, void *src);
extern void fn_8002FB04(u8 id, void *data, int zero);

void fn_1_AC328(Fn1AC328Target *target) {
    u8 data[0x40];

    fn_80083DB0(data, (u8 *)target->state + 0x98);
    fn_80083D40(data, lbl_1_data_3C7C4);
    target->state->value = 300;
    fn_8002FB04(target->id, data, 0);
}
/* fzgx:end fn_1_AC328 */

/* fzgx:begin fn_1_AC4B4 */
typedef struct Fn1AC4B4Target {
    u8 pad_00[0x24];
    struct Fn1AC4B4State *state;
} Fn1AC4B4Target;

typedef struct Fn1AC4B4State {
    u8 pad_00[0x2];
    u16 value;
} Fn1AC4B4State;

void fn_1_AC4B4(Fn1AC4B4Target *target) {
    target->state->value = 300;
}
/* fzgx:end fn_1_AC4B4 */

/* fzgx:begin fn_1_AC4C4 */
typedef struct Fn1AC4C4State {
    u16 pad_00;
    u16 value_02;
    s32 result;
    u32 flags;
    u8 pad_0c[0x4];
    u8 data_10[0x4];
    u8 data_14[0x4];
} Fn1AC4C4State;

typedef struct Fn1AC4C4Target {
    u8 value;
    u8 pad_01[0x23];
    Fn1AC4C4State *state;
} Fn1AC4C4Target;

extern s32 fn_8002A988(u8 value, void *arg1, void *arg2);

void fn_1_AC4C4(Fn1AC4C4Target *target) {
    target->state->result =
        fn_8002A988(target->value, &target->state->data_10, &target->state->data_14);
    if (target->state->result == 0) {
        target->state->flags |= 0x80;
    }
    if (target->state->result != -1) {
        target->state->value_02 = 0;
    }
}
/* fzgx:end fn_1_AC4C4 */

/* fzgx:begin fn_1_AC6A4 */
// fn_1_AC6A4: empty in retail (single blr).
void fn_1_AC6A4(void) {
}
/* fzgx:end fn_1_AC6A4 */

/* fzgx:begin fn_1_AC6C8 */
// fn_1_AC6C8: empty in retail (single blr).
void fn_1_AC6C8(void) {
}
/* fzgx:end fn_1_AC6C8 */

/* fzgx:begin fn_1_AC6DC */
// fn_1_AC6DC: empty in retail (single blr).
void fn_1_AC6DC(void) {
}
/* fzgx:end fn_1_AC6DC */

/* fzgx:begin fn_1_AC6E0 */
// fn_1_AC6E0: Write struct fields at offsets 0x8, 0xa, and 0x2a (with OR).
void fn_1_AC6E0(void *r3) {
    *(u16 *)((u8 *)r3 + 0x8) = 0x34;
    *(u16 *)((u8 *)r3 + 0xa) = 0xe;
    *(((u8 *)r3) + 0x2a) |= 0x4;
}
/* fzgx:end fn_1_AC6E0 */

/* fzgx:begin fn_1_AC700 */
// fn_1_AC700: empty in retail (single blr).
void fn_1_AC700(void) {
}
/* fzgx:end fn_1_AC700 */

/* fzgx:begin fn_1_AC904 */
// fn_1_AC904: empty in retail (single blr).
void fn_1_AC904(void) {
}
/* fzgx:end fn_1_AC904 */

/* fzgx:begin fn_1_AC928 */
// fn_1_AC928: empty in retail (single blr).
void fn_1_AC928(void) {
}
/* fzgx:end fn_1_AC928 */

/* fzgx:begin fn_1_ACD00 */
// fn_1_ACD00: empty in retail (single blr).
void fn_1_ACD00(void) {
}
/* fzgx:end fn_1_ACD00 */

/* fzgx:begin fn_1_ACE7C */
// fn_1_ACE7C: empty in retail (single blr).
void fn_1_ACE7C(void) {
}
/* fzgx:end fn_1_ACE7C */

/* fzgx:begin fn_1_ACE80 */
extern void fn_8002FB04(u8, void*, u32);

typedef struct {
    u8 field_0;
    u8 pad_1[0x23];
    void* field_24;
} SomeObject;

// fn_1_ACE80: writes 0x12c to pointed structure, then calls fn_8002FB04
void fn_1_ACE80(SomeObject* obj) {
    *(u16*)((u8*)obj->field_24 + 0x2) = 0x12c;
    fn_8002FB04(obj->field_0, (u8*)obj->field_24 + 0x98, 0);
}
/* fzgx:end fn_1_ACE80 */

/* fzgx:begin fn_1_ACF0C */
typedef struct {
    u8 pad_0[0x8];
    u16 field_8;
    u16 field_A;
    u8 pad_C[0x1E];
    u8 flags_2A;
} Object;

void fn_1_ACF0C(Object* obj) {
    obj->field_8 = 0x40;
    obj->field_A = 0xE;
    obj->flags_2A |= 0x4;
}
/* fzgx:end fn_1_ACF0C */

/* fzgx:begin fn_1_ACF2C */
// fn_1_ACF2C: empty in retail (single blr).
void fn_1_ACF2C(void) {
}
/* fzgx:end fn_1_ACF2C */

/* fzgx:begin fn_1_AD098 */
// fn_1_AD098: empty in retail (single blr).
void fn_1_AD098(void) {
}
/* fzgx:end fn_1_AD098 */

/* fzgx:begin fn_1_AD09C */
// fn_1_AD09C: empty in retail (single blr).
void fn_1_AD09C(void) {
}
/* fzgx:end fn_1_AD09C */

/* fzgx:begin fn_1_AD140 */
extern int fn_1_B7FDC(u8 value);

int fn_1_AD140(const u8 *value) {
    return fn_1_B7FDC(value[0]);
}
/* fzgx:end fn_1_AD140 */

/* fzgx:begin fn_1_AD164 */
// fn_1_AD164: empty in retail (single blr).
void fn_1_AD164(void) {
}
/* fzgx:end fn_1_AD164 */

/* fzgx:begin fn_1_AD1E4 */
typedef struct Fn1AD1E4A {
    u8 pad0[8];
    void *field8;
    void *fieldC;
} Fn1AD1E4A;

typedef struct Fn1AD1E4P {
    u8 pad0[8];
    u32 flags;
    u8 padC[0x88];
    void *field94;
    u8 pad98[0x20];
    void *fieldB8;
} Fn1AD1E4P;

typedef struct Fn1AD1E4B {
    u8 pad0[3];
    u8 state;
    u8 pad4[0x20];
    Fn1AD1E4P *field24;
} Fn1AD1E4B;

extern void fn_80083D6C(void *arg0, void *arg1, int arg2);
extern void fn_80008BA8(void *arg0, void *arg1, void *arg2);

void fn_1_AD1E4(Fn1AD1E4A *arg0, Fn1AD1E4B *arg1) {
    if ((arg1->field24->flags & 0x20) == 0) {
        if (arg0->field8 != 0) {
            fn_80083D6C((u8 *)arg1->field24->field94 + 0x24,
                        arg0->field8, 0x20);
        }
        fn_80008BA8((u8 *)arg1->field24->field94 + 0x2060,
                    arg0->fieldC, arg1->field24->fieldB8);
    }
    arg1->state = 3;
}
/* fzgx:end fn_1_AD1E4 */

/* fzgx:begin fn_1_B03A8 */
void fn_1_B03A8(void *arg0, void *arg1) {
    void *child = *(void **)((u8 *)arg1 + 0x24);
    if ((*(u32 *)((u8 *)child + 0x8) & 0x20) == 0) {
        *(u32 *)((u8 *)child + 0x90) = *(u32 *)((u8 *)arg0 + 0xc);
    }
    *(u8 *)((u8 *)arg1 + 0x3) = 0x3;
}
/* fzgx:end fn_1_B03A8 */

/* fzgx:begin fn_1_B2770 */
// fn_1_B2770: main_rel .text:0x000B2770 size 0xC
// Store byte 0x3 at offset 3 of the pointer argument

void fn_1_B2770(int unused, void *ptr) {
    *(u8 *)((u8 *)ptr + 3) = 0x3;
}
/* fzgx:end fn_1_B2770 */

/* fzgx:begin fn_1_B38F4 */
void fn_1_B38F4(int unused, void *ptr) {
    *(u8 *)((u8 *)ptr + 3) = 0x3;
}
/* fzgx:end fn_1_B38F4 */

/* fzgx:begin fn_1_B40B4 */
void fn_1_B40B4(int unused, void *ptr) {
    *(u8 *)((u8 *)ptr + 3) = 0x3;
}
/* fzgx:end fn_1_B40B4 */

/* fzgx:begin fn_1_B7E48 */
extern u8 lbl_1_bss_716C8[320];

typedef struct {
    u8 pad0[8];
    u32 flags;
} FlagObject;

int fn_1_B7E48(void) {
    int result = 0;
    FlagObject *object = *(FlagObject **)(lbl_1_bss_716C8 + 0x24);

    if (object != 0 && (object->flags & 0x200) != 0) {
        result = 1;
    }

    object = *(FlagObject **)(lbl_1_bss_716C8 + 0xc4);
    if (object != 0 && (object->flags & 0x200) != 0) {
        result = 1;
    }

    return result;
}
/* fzgx:end fn_1_B7E48 */

/* fzgx:begin fn_1_B7EF8 */
extern u8 lbl_1_bss_718B9[7];

// fn_1_B7EF8: Load and return first byte from lbl_1_bss_718B9
u8 fn_1_B7EF8(void) {
    return lbl_1_bss_718B9[0];
}
/* fzgx:end fn_1_B7EF8 */

/* fzgx:begin fn_1_C17CC */
// fn_1_C17CC: empty in retail (single blr).
void fn_1_C17CC(void) {
}
/* fzgx:end fn_1_C17CC */
