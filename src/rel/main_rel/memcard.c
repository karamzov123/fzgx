#include "types.h"

/* fzgx:begin fn_1_AB478 */
#include "rel/main_rel/memcard.h"

typedef struct {
    u8 pad_0[2];
    u16 unk_2;
} Fn1AB478Sub;

typedef struct {
    u8 unk_0;
    u8 pad_1[6];
    u8 unk_7;
    u8 pad_8[4];
    u32 unk_C;
    u8 pad_10[0x14];
    Fn1AB478Sub *unk_24;
} Fn1AB478Obj;

extern u32 lbl_801A6410;
extern Obj_1_data_3C7B8 lbl_1_data_3C7B8;
extern u32 fn_1_45D0(void *, s32, void *, s32);
extern void fn_1_AB45C(void);
extern void fn_8002DE40(u8, u32, void (*)(void), s32);

void fn_1_AB478(Fn1AB478Obj *obj) {
    obj->unk_C = fn_1_45D0((void *)lbl_801A6410, 0xA000,
                           &lbl_1_data_3C7B8, 0x657);
    obj->unk_24->unk_2 = 0x12c;
    obj->unk_7 = 0;
    fn_8002DE40(obj->unk_0, obj->unk_C, fn_1_AB45C, 0);
}
/* fzgx:end fn_1_AB478 */

/* fzgx:begin fn_1_AB4F4 */
extern int fn_8002A958(u8 value);
extern int fn_80030690(u8 value, void *data);

typedef struct {
    u8 pad[0x2];
    s16 field_0x2;
    int field_0x4;
} StatusStruct;

typedef struct {
    u8 byte_0;
    s8 byte_1;
    u8 pad[0x22];
    StatusStruct *ptr_0x24;
    u8 pad2[0x70];
    int field_0x98;
    int field_0x9c;
} ArgStruct;

void fn_1_AB4F4(ArgStruct *arg) {
    int status;

    status = fn_8002A958(arg->byte_0);
    arg->ptr_0x24->field_0x4 = status;
    if (arg->ptr_0x24->field_0x4 == -3 && arg->byte_1 == -2) {
        arg->ptr_0x24->field_0x4 = arg->byte_1;
    }
    if (arg->ptr_0x24->field_0x4 != -1 &&
        fn_80030690(arg->byte_0, &arg->field_0x98)) {
        arg->field_0x9c = 0;
        arg->field_0x98 = 0;
    }
    if (arg->ptr_0x24->field_0x4 != -1) {
        arg->ptr_0x24->field_0x2 = 0;
    }
}
/* fzgx:end fn_1_AB4F4 */

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

/* fzgx:begin fn_1_AB61C */
typedef struct {
    u8 pad[1];
    u8 field_0x1;
} MemcardState;

typedef struct {
    u8 pad_0x24[0x24];
    MemcardState *state;
    u8 pad_0x2a[0x2];
    u8 field_0x2a;
} MemcardArg;

void fn_1_AB61C(MemcardArg *arg) {
    MemcardState *state = arg->state;
    state->field_0x1 = 1;
    *(s16 *)((u8 *)arg + 0x8) = -1;
    *(s16 *)((u8 *)arg + 0xa) = -1;
    arg->field_0x2a = arg->field_0x2a & 0xFE;
    arg->field_0x2a = 0;
}
/* fzgx:end fn_1_AB61C */

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

/* fzgx:begin fn_1_AB7C4 */
extern int fn_8002A958(u8 byte);
extern int fn_8002C0A0(u8 byte);
extern const f64 lbl_1_rodata_4CB0;
extern const f32 lbl_1_rodata_4CAC;

typedef struct {
    u8 pad[0x2];
    u16 field_0x2;
    s32 field_0x4;
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

void fn_1_AB7C4(ArgStruct *arg) {
    arg->ptr_0x24->field_0x4 = fn_8002A958(arg->byte_0);
    arg->field_0x38 = (fn_8002C0A0(arg->byte_0) - arg->field_0x30) / lbl_1_rodata_4CAC;
    if (arg->ptr_0x24->field_0x4 != -1) {
        arg->flags_0x2a &= ~2;
        arg->ptr_0x24->field_0x2 = 0;
        arg->flags_0x2a &= ~16;
    }
}
/* fzgx:end fn_1_AB7C4 */

/* fzgx:begin fn_1_AB870 */
typedef struct {
    u8 pad[0x2];
    u16 field_0x2;
} OffsetStruct;

typedef struct {
    u8 pad[0x24];
    OffsetStruct *ptr_0x24;
} ArgStruct;

// fn_1_AB870: main_rel .text:0x000AB870 size 0x10
void fn_1_AB870(ArgStruct *arg) {
    arg->ptr_0x24->field_0x2 = 0x12c;
}
/* fzgx:end fn_1_AB870 */

/* fzgx:begin fn_1_AB880 */
extern int fn_80030690(u8 byte, void *ptr);

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

// fn_1_AB880: main_rel .text:0x000AB880 size 0x58
void fn_1_AB880(ArgStruct *arg) {
    OffsetStruct *s = arg->ptr_0x24;
    int result = fn_80030690(arg->byte_0, (u8 *)s + 0x18);

    s = arg->ptr_0x24;
    s->field_0x4 = result;

    s = arg->ptr_0x24;
    if (s->field_0x4 != -1) {
        s->field_0x2 = 0;
    }
}
/* fzgx:end fn_1_AB880 */

/* fzgx:begin fn_1_AB8D8 */
extern u8 lbl_1_data_3C89C[68];

extern void fn_80083DB0(void *dest, const void *src);
extern void fn_80083D40(void *dest, const void *src);
extern void fn_8002FB04(u8 byte, void *data, int arg);

typedef struct {
    u8 byte_0;
    u8 pad_0x1[0x23];
    void *ptr_0x24;
} ArgStruct;

void fn_1_AB8D8(ArgStruct *arg) {
    u8 data[0x40];

    fn_80083DB0(data, (u8 *)arg->ptr_0x24 + 0x98);
    fn_80083D40(data, lbl_1_data_3C89C);
    ((u16 *)arg->ptr_0x24)[1] = 0x12c;
    fn_8002FB04(arg->byte_0, data, 0);
}
/* fzgx:end fn_1_AB8D8 */

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

/* fzgx:begin fn_1_AB98C */
typedef struct {
    u8 pad[0x2];
    u16 field_0x2;
} OffsetStruct;

typedef struct {
    u8 pad[0x24];
    OffsetStruct *ptr_0x24;
} ArgStruct;

void fn_1_AB98C(ArgStruct *arg) {
    arg->ptr_0x24->field_0x2 = 300;
}
/* fzgx:end fn_1_AB98C */

/* fzgx:begin fn_1_AB99C */
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

void fn_1_AB99C(ArgStruct *arg) {
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
/* fzgx:end fn_1_AB99C */

/* fzgx:begin fn_1_ABA14 */
typedef struct {
    u8 pad[0x2];
    u16 field_0x2;
} OffsetStruct;

typedef struct {
    u8 pad[0x24];
    OffsetStruct *ptr_0x24;
} ArgStruct;

void fn_1_ABA14(ArgStruct *arg) {
    arg->ptr_0x24->field_0x2 = 0x12c;
}
/* fzgx:end fn_1_ABA14 */

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

/* fzgx:begin fn_1_ABB24 */
typedef struct {
    u8 pad_0x0[0x1];
    u8 field_0x1;
    u8 pad_0x2[0x6];
    s16 field_0x8;
    s16 field_0xa;
} MemcardState;

typedef struct {
    u8 pad_0x0[0x8];
    s16 field_0x8;
    s16 field_0xa;
    u8 pad_0xc[0x18];
    MemcardState *ptr_0x24;
    u8 pad_0x28[0x2];
    u8 field_0x2a;
} MemcardArg;

void fn_1_ABB24(MemcardArg *arg) {
    arg->ptr_0x24->field_0x1 = 1;
    arg->field_0x8 = -1;
    arg->field_0xa = -1;
    arg->field_0x2a &= 0xfe;
}
/* fzgx:end fn_1_ABB24 */

/* fzgx:begin fn_1_ABC5C */
extern void fn_8002F9D8(u8 byte_val, void *arg, s32 zero);

typedef struct {
    u8 byte_0;
    u8 pad_0x1[0x13];
    void *field_0x14;
} ArgStruct;

typedef struct {
    u8 pad[0x2];
    u16 field_0x2;
} ResultStruct;

typedef struct {
    u8 pad[0x24];
    ResultStruct *ptr_0x24;
} StateStruct;

void fn_1_ABC5C(ArgStruct *arg) {
    StateStruct *state;

    state = (StateStruct *)arg;
    state->ptr_0x24->field_0x2 = 0x12c;
    fn_8002F9D8(arg->byte_0, arg->field_0x14, 0);
}
/* fzgx:end fn_1_ABC5C */

/* fzgx:begin fn_1_ABC98 */
extern s32 fn_8002A958(u8 byte_val);

typedef struct {
    u8 pad[0x2];
    u16 field_0x2;
    s32 field_0x4;
} ResultStruct;

typedef struct {
    u8 byte_0;
    u8 pad_0x1[0x23];
    ResultStruct *ptr_0x24;
} ArgStruct;

void fn_1_ABC98(ArgStruct *arg) {
    arg->ptr_0x24->field_0x4 = fn_8002A958(arg->byte_0);
    if (arg->ptr_0x24->field_0x4 != -1) {
        arg->ptr_0x24->field_0x2 = 0;
    }
}
/* fzgx:end fn_1_ABC98 */

/* fzgx:begin fn_1_ABCE8 */
extern u8 lbl_1_data_3C89C[68];
extern void fn_80083DB0(void *dest, void *src);
extern void fn_80083D40(void *arg, void *data);
extern void fn_8002EED8(u8 byte_val, void *arg1, void *arg2, void *arg3, u32 arg4);

typedef struct {
    u8 pad[0x2];
    u16 field_0x2;
    u8 pad_0x4[0x8];
    void *ptr_0xc;
} ResultStruct;

typedef struct {
    u8 byte_0;
    u8 pad_0x1[0xf];
    u8 data_0x10[0x14];
    ResultStruct *ptr_0x24;
} ArgStruct;

void fn_1_ABCE8(ArgStruct *arg) {
    u8 local[0x40];

    fn_80083DB0(local, (u8 *)arg->ptr_0x24 + 0x98);
    fn_80083D40(local, lbl_1_data_3C89C);
    arg->ptr_0x24->field_0x2 = 0x12c;
    fn_8002EED8(arg->byte_0, local, arg->ptr_0x24->ptr_0xc,
        arg->data_0x10, 0);
}
/* fzgx:end fn_1_ABCE8 */

/* fzgx:begin fn_1_ABD58 */
extern s32 fn_8002A958(u8 byte_val);

typedef struct {
    u8 pad[0x2];
    u16 field_0x2;
    s32 field_0x4;
} ResultStruct;

typedef struct {
    u8 byte_0;
    u8 pad_0x1[0x23];
    ResultStruct *ptr_0x24;
} ArgStruct;

void fn_1_ABD58(ArgStruct *arg) {
    arg->ptr_0x24->field_0x4 = fn_8002A958(arg->byte_0);
    if (arg->ptr_0x24->field_0x4 != -1) {
        arg->ptr_0x24->field_0x2 = 0;
    }
}
/* fzgx:end fn_1_ABD58 */

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

/* fzgx:begin fn_1_ABDB8 */
#include "rel/main_rel/globals.h"

extern s32 fn_8002FE54(u8 byte_val, void *arg1, void *arg2);

typedef struct {
    u8 pad_0x0[0x2];
    u16 unk_0x2;
    s32 unk_0x4;
    u8 pad_0x8[0x18];
    u8 data_0x20[1];
} ResultStruct;

typedef struct {
    u8 byte_0x0;
    u8 pad_0x1[0x13];
    void *ptr_0x14;
    u8 pad_0x18[0xc];
    ResultStruct *ptr_0x24;
} ArgStruct;

// Loads the result data and clears its metadata when the load succeeds.
void fn_1_ABDB8(ArgStruct *arg) {
    s32 loaded_result;

    loaded_result = fn_8002FE54(arg->byte_0x0, arg->ptr_0x14,
                                arg->ptr_0x24->data_0x20);
    arg->ptr_0x24->unk_0x4 = loaded_result;
    if (arg->ptr_0x24->unk_0x4 != -1) {
        arg->ptr_0x24->unk_0x2 = 0;
    }
}
/* fzgx:end fn_1_ABDB8 */

/* fzgx:begin fn_1_ABF44 */
typedef struct {
    u8 pad_00[0x2];
    u16 field_0x2;
    s32 field_0x4;
    u8 pad_08[0x84];
    void *field_0x8c;
    s32 field_0x90;
    s32 field_0x94;
    u8 field_0x98[0x10];
} Fn1ABF44State;

typedef struct {
    u8 id;
    u8 pad_01[0xF];
    u8 data_0x10[0x14];
    Fn1ABF44State *state;
    u8 pad_28[0x2];
    u8 flags_0x2a;
    u8 pad_2b[0x5];
    s32 value_0x30;
    u8 pad_34[0x4];
    f32 value_0x38;
} Fn1ABF44Object;

extern u8 lbl_1_data_3C7C4[216];
extern const f32 lbl_1_rodata_4CA8;

extern void fn_80083DB0(void *dst, void *src);
extern void fn_80083D40(void *dst, void *src);
extern s32 fn_8002EBD4(u8 id, void *work, void *data);
extern s32 fn_8002C0A0(u8 id);
extern void fn_8002F7D8(void *data, s32 size, s32 aligned, s32 value, s32 zero);

void fn_1_ABF44(Fn1ABF44Object *arg) {
    u8 work[0x40];
    s32 value;
    s32 size;
    s32 aligned;

    arg->state->field_0x2 = 300;
    fn_80083DB0(work, (u8 *)arg->state + 0x98);
    fn_80083D40(work, lbl_1_data_3C7C4);
    value = fn_8002EBD4(arg->id, work, arg->data_0x10);
    arg->state->field_0x4 = value;
    arg->value_0x30 = fn_8002C0A0(arg->id);
    arg->value_0x38 = lbl_1_rodata_4CA8;
    arg->flags_0x2a |= 2;
    size = arg->state->field_0x94 +
        *(s32 *)((u8 *)arg->state->field_0x8c + 0x44);
    aligned = (*(s32 *)((u8 *)arg->state->field_0x8c + 0x48) + 0x1fff) &
        ~0x1fff;
    fn_8002F7D8(arg->data_0x10, size, aligned,
                *(s32 *)((u8 *)arg->state->field_0x8c + 0x44), 0);
}
/* fzgx:end fn_1_ABF44 */

/* fzgx:begin fn_1_AC12C */
#include "rel/main_rel/globals.h"

typedef struct Fn1AC12CState {
    u8 pad_00[0x2];
    u16 value;
    u8 pad_04[0x90];
    void *data;
} Fn1AC12CState;

typedef struct Fn1AC12CTarget {
    u8 id;
    u8 pad_01[0xF];
    u8 buffer[0x14];
    Fn1AC12CState *state;
    u8 pad_28[0x8];
    void *result;
} Fn1AC12CTarget;

extern void *fn_8002C0A0(u8 id);
extern void fn_8002F7D8(void *dst, void *src, int size, int zero, int flags);

// Initializes the memory-card request and prepares its transfer buffer.
void fn_1_AC12C(Fn1AC12CTarget *target) {
    target->state->value = 300;
    target->result = fn_8002C0A0(target->id);
    fn_8002F7D8((u8 *)target + 0x10, target->state->data, 0x4000, 0, 0);
}
/* fzgx:end fn_1_AC12C */

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

/* fzgx:begin fn_1_AC2D8 */
typedef struct Fn1AC2D8State {
    u8 pad_00[0x2];
    u16 value;
    s32 status;
} Fn1AC2D8State;

typedef struct Fn1AC2D8Target {
    u8 id;
    u8 pad_01[0x23];
    Fn1AC2D8State *state;
} Fn1AC2D8Target;

extern s32 fn_8002A958(u8 id);

void fn_1_AC2D8(Fn1AC2D8Target *target) {
    target->state->status = fn_8002A958(target->id);
    if (target->state->status != -1) {
        target->state->value = 0;
    }
}
/* fzgx:end fn_1_AC2D8 */

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

/* fzgx:begin fn_1_AC38C */
typedef struct Fn1AC38Data {
    u8 pad_00[0x2];
    u16 value;
    int status;
} Fn1AC38Data;

typedef struct Fn1AC38Target {
    u8 id;
    u8 pad_01[0x23];
    Fn1AC38Data *data;
} Fn1AC38Target;

extern int fn_8002A958(u8 id);

void fn_1_AC38C(Fn1AC38Target *target) {
    target->data->status = fn_8002A958(target->id);
    if (target->data->status != -1) {
        target->data->value = 0;
    }
}
/* fzgx:end fn_1_AC38C */

/* fzgx:begin fn_1_AC3DC */
typedef struct Fn1AC3DCState {
    u8 pad_00[0x2];
    u16 value;
    u8 pad_04[0x94];
} Fn1AC3DCState;

typedef struct Fn1AC3DCTarget {
    u8 id;
    u8 pad_01[0x23];
    Fn1AC3DCState *state;
} Fn1AC3DCTarget;

extern u8 lbl_1_data_3C89C[68];
extern u8 lbl_1_data_3C7C4[216];

extern void fn_80083DB0(void *dst, void *src);
extern void fn_80083D40(void *dst, void *src);
extern void fn_8003013C(u8 id, void *data1, void *data2, int zero);

void fn_1_AC3DC(Fn1AC3DCTarget *target) {
    u8 data1[0x40];
    u8 data2[0x40];

    fn_80083DB0(data1, (u8 *)target->state + 0x98);
    fn_80083DB0(data2, (u8 *)target->state + 0x98);
    fn_80083D40(data1, lbl_1_data_3C89C);
    fn_80083D40(data2, lbl_1_data_3C7C4);
    target->state->value = 300;
    fn_8003013C(target->id, data1, data2, 0);
}
/* fzgx:end fn_1_AC3DC */

/* fzgx:begin fn_1_AC464 */
typedef struct Fn1AC464State {
    u16 pad_00;
    u16 value;
    s32 result;
} Fn1AC464State;

typedef struct Fn1AC464Target {
    u8 value;
    u8 pad_01[0x23];
    Fn1AC464State *state;
} Fn1AC464Target;

extern s32 fn_8002A958(u8 value);

void fn_1_AC464(Fn1AC464Target *target) {
    target->state->result = fn_8002A958(target->value);
    if (target->state->result != -1) {
        target->state->value = 0;
    }
}
/* fzgx:end fn_1_AC464 */

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

/* fzgx:begin fn_1_AC6A8 */
typedef struct {
    u8 unk0[8];
    u16 unk8;
    u16 unka;
    u8 unkC[0x1e];
    u8 unk2a;
} Fn1AC6A8Data;

void fn_1_AC6A8(Fn1AC6A8Data *data) {
    data->unk8 = 0x33;
    data->unka = 0xe;
    data->unk2a |= 4;
}
/* fzgx:end fn_1_AC6A8 */

/* fzgx:begin fn_1_AC6C8 */
// fn_1_AC6C8: empty in retail (single blr).
void fn_1_AC6C8(void) {
}
/* fzgx:end fn_1_AC6C8 */

/* fzgx:begin fn_1_AC6CC */
typedef struct MemCardState {
    u8 padding[0x2a];
    u8 flags;
} MemCardState;

void fn_1_AC6CC(MemCardState *self) {
    self->flags |= 4;
}
/* fzgx:end fn_1_AC6CC */

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

/* fzgx:begin fn_1_AC704 */
extern void fn_8002F428(void *arg0, void *arg1, void *arg2, int arg3, int arg4);

void fn_1_AC704(void *r3) {
    *(u16 *)((u8 *)*(void **)((u8 *)r3 + 0x24) + 0x2) = 0x12c;
    fn_8002F428((u8 *)r3 + 0x10,
                *(void **)((u8 *)*(void **)((u8 *)r3 + 0x24) + 0x94),
                *(void **)((u8 *)*(void **)((u8 *)r3 + 0x24) + 0xc),
                0,
                0);
}
/* fzgx:end fn_1_AC704 */

/* fzgx:begin fn_1_AC74C */
extern s32 fn_8002A958(u8 value);

void fn_1_AC74C(void *r3) {
    s32 result;

    result = fn_8002A958(*(u8 *)r3);
    *(s32 *)((u8 *)*(void **)((u8 *)r3 + 0x24) + 0x4) = result;
    if (*(s32 *)((u8 *)*(void **)((u8 *)r3 + 0x24) + 0x4) != -1) {
        *(u16 *)((u8 *)*(void **)((u8 *)r3 + 0x24) + 0x2) = 0;
    }
}
/* fzgx:end fn_1_AC74C */

/* fzgx:begin fn_1_AC904 */
// fn_1_AC904: empty in retail (single blr).
void fn_1_AC904(void) {
}
/* fzgx:end fn_1_AC904 */

/* fzgx:begin fn_1_AC908 */
typedef struct {
    u8 unk0[8];
    u16 field8;
    u16 fieldA;
    u8 unkC[0x1e];
    u8 flags;
} Fn1AC908State;

void fn_1_AC908(Fn1AC908State *state) {
    state->field8 = 0x3c;
    state->fieldA = 0xe;
    state->flags |= 4;
}
/* fzgx:end fn_1_AC908 */

/* fzgx:begin fn_1_AC928 */
// fn_1_AC928: empty in retail (single blr).
void fn_1_AC928(void) {
}
/* fzgx:end fn_1_AC928 */

/* fzgx:begin fn_1_AC960 */
extern const f64 lbl_1_rodata_4CC8;
extern s32 fn_8002FE54(u8, u8, void *);

struct MemcardData {
    u8 pad0[2];
    u16 status;
    s32 result;
};

struct MemcardObject {
    u8 id;
    u8 pad1[0x23];
    struct MemcardData *data;
    u8 pad28[2];
    u8 flags;
    u8 pad2b;
    u8 state;
    u8 pad2d[0xb];
    f32 progress;
};

void fn_1_AC960(struct MemcardObject *obj) {
    s32 result;

    result = fn_8002FE54(obj->id, obj->state,
                          (void *)((u8 *)obj->data + 0x20));
    obj->data->result = result;
    obj->progress = (f32)((f64)obj->state / lbl_1_rodata_4CC8);
    if (obj->state == 0x7e) {
        obj->flags = obj->flags & 0xfffffffd;
        obj->data->status = 0;
    }
}
/* fzgx:end fn_1_AC960 */

/* fzgx:begin fn_1_ACA00 */
typedef struct Fn1Aca00Target {
    u8 _pad00[0x24];
    struct Fn1Aca00Child *child;
    u8 _pad28[4];
    u8 flag_2c;
    u8 flag_2d;
} Fn1Aca00Target;

typedef struct Fn1Aca00Child {
    u8 _pad00[2];
    u16 value;
} Fn1Aca00Child;

void fn_1_ACA00(Fn1Aca00Target *self) {
    self->flag_2c = 0;
    self->flag_2d = 0;
    self->child->value = 300;
}
/* fzgx:end fn_1_ACA00 */

/* fzgx:begin fn_1_ACA1C */
typedef struct MemCardData {
    u8 pad0[4];
    s32 value;
    u8 pad8[0x18];
} MemCardData;

typedef struct MemCardState {
    u8 id;
    u8 pad1[0x23];
    MemCardData *data;
    u8 pad28[4];
    u8 slot;
} MemCardState;

extern s32 fn_8002FE54(u8, u8, void *);

void fn_1_ACA1C(MemCardState *state) {
    state->data->value = fn_8002FE54(state->id, state->slot,
        (u8 *)state->data + 0x20);
    if (state->slot == 0x7e) {
        *(u16 *)((u8 *)state->data + 2) = 0;
    }
}
/* fzgx:end fn_1_ACA1C */

/* fzgx:begin fn_1_ACD00 */
// fn_1_ACD00: empty in retail (single blr).
void fn_1_ACD00(void) {
}
/* fzgx:end fn_1_ACD00 */

/* fzgx:begin fn_1_ACD04 */
#include "rel/main_rel/globals.h"
#include "rel/main_rel/memcard.h"

typedef struct {
    u8 pad_0[0x8];
    u32 unk_8;
    u8 pad_c[0x4];
    s32 unk_10;
    u32 unk_14;
    u32 unk_18;
    u32 unk_1c;
    u8 pad_20[0x74];
    void *unk_94;
    u8 pad_98[0x44];
    void *unk_dc;
} Fn1Acd04Inner;

typedef struct {
    u8 unk_0;
    u8 pad_1[0x1];
    u8 unk_2;
    u8 pad_3[0x4];
    u8 unk_7;
    u16 unk_8;
    u16 unk_a;
    void *unk_c;
    u8 pad_10[0x14];
    Fn1Acd04Inner *unk_24;
    u8 pad_28[0x2];
    u8 unk_2a;
    u8 pad_2b[0x11];
    s32 unk_3c;
    s32 unk_40;
} Fn1Acd04Obj;

extern int fn_8002E0C4(u8 value);
extern void fn_1_46B4(void *arg0, void *arg1, void *arg2, int arg3);
extern void *lbl_801A6410;

// Clear pending card work and mark the operation ready for the next state.
void fn_1_ACD04(Fn1Acd04Obj *obj) {
    int value;

    if (obj->unk_c != 0) {
        do {
            value = fn_8002E0C4(obj->unk_0);
        } while (value == -1);

        fn_1_46B4(lbl_801A6410, obj->unk_c, &lbl_1_data_3C7B8, 0x3c8);
        obj->unk_7 = 0;
        obj->unk_c = 0;

        {
            Fn1Acd04Inner *inner = obj->unk_24;

            inner->unk_1c = 0;
            inner->unk_18 = 0;
        }
    }

    if ((obj->unk_24->unk_8 & 0x80) != 0) {
        obj->unk_3c = obj->unk_24->unk_10 / 8192;
        obj->unk_40 = obj->unk_24->unk_14;
    }

    if ((obj->unk_24->unk_8 & 0x20) == 0) {
        fn_1_46B4(lbl_801A6410, obj->unk_24->unk_94,
                  &lbl_1_data_3C7B8, 0x3d4);
    }

    if (obj->unk_2 == 4) {
        if (lbl_1_bss_716C0 != 0) {
            void *temp;
            Fn1Acd04Inner *inner;

            inner = obj->unk_24;
            temp = inner->unk_94;
            inner->unk_94 = inner->unk_dc;
            inner->unk_dc = temp;
            lbl_1_bss_716C0 = 0;
        }

        if (obj->unk_24->unk_dc != 0) {
            fn_1_46B4(lbl_801A6410, obj->unk_24->unk_dc,
                      &lbl_1_data_3C7B8, 0x3de);
            obj->unk_24->unk_dc = 0;
        }
    }

    obj->unk_8 = 0x3f;
    obj->unk_a = 0xe;
    obj->unk_2a |= 4;
    obj->unk_24->unk_8 |= 4;
}
/* fzgx:end fn_1_ACD04 */

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

/* fzgx:begin fn_1_ACEBC */
extern s32 fn_8002A958(u8);

typedef struct {
    u8 field_0;
    u8 pad_1[0x23];
    void* field_24;
} SomeObject;

void fn_1_ACEBC(SomeObject* obj) {
    s32 value;
    value = fn_8002A958(obj->field_0);
    *(s32*)((u8*)obj->field_24 + 0x4) = value;
    if (*(s32*)((u8*)obj->field_24 + 0x4) != -1) {
        *(u16*)((u8*)obj->field_24 + 0x2) = 0;
    }
}
/* fzgx:end fn_1_ACEBC */

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

/* fzgx:begin fn_1_ACF30 */
#include "rel/main_rel/globals.h"
#include "rel/main_rel/memcard.h"

typedef struct MemcardState MemcardState;

typedef struct {
    u8 id;
    u8 pad_1;
    u8 type;
    u8 pad_3[0x4];
    u8 ready;
    u16 field_8;
    u16 field_A;
    void* card;
    u8 pad_10[0x14];
    MemcardState* sub;
    u8 pad_28[0x2];
    u8 flags_2A;
    u8 pad_2B[0x11];
    s32 value_3C;
    s32 value_40;
} MemcardObject;

struct MemcardState {
    u8 pad_0[0x8];
    u32 flags_8;
    u8 pad_C[0x4];
    s32 value_10;
    s32 value_14;
    u32 field_18;
    u32 field_1C;
    u8 pad_20[0x74];
    void* value_94;
    u8 pad_98[0x44];
    void* value_DC;
};

extern void* lbl_801A6410;
extern s32 fn_8002E0C4(u8 value);
extern void fn_1_46B4(void* manager, void* value, Obj_1_data_3C7B8* data, s32 offset);

// Cleans up pending card data, updates state values, and marks the object ready.
void fn_1_ACF30(MemcardObject* obj) {
    if (obj->card != 0) {
        do {
            if (fn_8002E0C4(obj->id) != -1) {
                fn_1_46B4(lbl_801A6410, obj->card, &lbl_1_data_3C7B8, 0x3C8);
                obj->ready = 0;
                obj->card = 0;
                {
                    MemcardState* state = obj->sub;
                    state->field_1C = 0;
                    state->field_18 = 0;
                }
                break;
            }
        } while (1);
    }

    if ((obj->sub->flags_8 & 0x80) != 0) {
        obj->value_3C = obj->sub->value_10 / 0x2000;
        obj->value_40 = obj->sub->value_14;
    }
    if ((obj->sub->flags_8 & 0x20) == 0) {
        fn_1_46B4(lbl_801A6410, obj->sub->value_94, &lbl_1_data_3C7B8, 0x3D4);
    }
    if (obj->type == 4) {
        if (lbl_1_bss_716C0 != 0) {
            void* temp;
            MemcardState* state;
            state = obj->sub;
            temp = state->value_94;
            state->value_94 = state->value_DC;
            state->value_DC = temp;
            lbl_1_bss_716C0 = 0;
        }
        if (obj->sub->value_DC != 0) {
            fn_1_46B4(lbl_801A6410, obj->sub->value_DC, &lbl_1_data_3C7B8, 0x3DE);
            obj->sub->value_DC = 0;
        }
    }
    obj->field_8 = 0x41;
    obj->field_A = 0xE;
    obj->flags_2A |= 0x4;
}
/* fzgx:end fn_1_ACF30 */

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
#include "rel/main_rel/globals.h"

extern int fn_1_B7FDC(u8 value);

// Forwards the saved memory-card byte to the common handler.
int fn_1_AD140(const u8 *value) {
    return fn_1_B7FDC(*value);
}
/* fzgx:end fn_1_AD140 */

/* fzgx:begin fn_1_AD164 */
// fn_1_AD164: empty in retail (single blr).
void fn_1_AD164(void) {
}
/* fzgx:end fn_1_AD164 */

/* fzgx:begin fn_1_AD168 */
typedef struct Fn1AD168Obj {
    u8 field0;
    u8 _pad1[0x13];
    void *field14;
    u8 _pad18[0xc];
    u8 *field24;
} Fn1AD168Obj;

extern void fn_80030754(u8 value, void *data, int size, int mode);

void fn_1_AD168(Fn1AD168Obj *obj) {
    if (obj->field24[0] == 1) {
        fn_80030754(obj->field0, obj->field14, 0x1c, 0);
    }
}
/* fzgx:end fn_1_AD168 */

/* fzgx:begin fn_1_AD1AC */
#include "rel/main_rel/globals.h"

typedef struct Fn1AD1ACOutput {
    u8 unk_00[4];
    void *unk_04;
} Fn1AD1ACOutput;

typedef struct Fn1AD1ACObject {
    u8 unk_00;
    u8 unk_01[0x23];
    Fn1AD1ACOutput *unk_24;
} Fn1AD1ACObject;

extern void *fn_8002A958(u8 arg0);

// Generate the object's value and publish it through the output slot.
void fn_1_AD1AC(Fn1AD1ACObject *obj) {
    obj->unk_24->unk_04 = fn_8002A958(obj->unk_00);
}
/* fzgx:end fn_1_AD1AC */

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

/* fzgx:begin fn_1_AEC34 */
typedef struct Fn1AEC34A {
    u8 pad0[8];
    void *field8;
    void *fieldC;
} Fn1AEC34A;

typedef struct Fn1AEC34P {
    u8 pad0[8];
    u32 flags;
    u8 padC[0x88];
    void *field94;
    u8 pad98[0x4c];
    void *fieldE4;
} Fn1AEC34P;

typedef struct Fn1AEC34B {
    u8 pad0[3];
    u8 state;
    u8 pad4[0x20];
    Fn1AEC34P *field24;
} Fn1AEC34B;

extern u32 lbl_1_bss_77310[28];
extern u32 lbl_1_bss_7730C;
extern void fn_80083D6C(void *arg0, void *arg1, int arg2);
extern void fn_80008BA8(void *arg0, void *arg1, void *arg2);

void fn_1_AEC34(Fn1AEC34A *arg0, Fn1AEC34B *arg1) {
    if ((arg1->field24->flags & 0x20) == 0) {
        if (arg0->field8 != 0) {
            fn_80083D6C((u8 *)arg1->field24->field94 + 0x24,
                        arg0->field8, 0x20);
        }
        fn_80008BA8((u8 *)arg1->field24->field94 + 0x2060,
                    arg0->fieldC, arg1->field24->fieldE4);
    }
    arg1->state = 3;
    lbl_1_bss_77310[0] = 1;
    lbl_1_bss_7730C = 0;
}
/* fzgx:end fn_1_AEC34 */

/* fzgx:begin fn_1_B03A8 */
void fn_1_B03A8(void *arg0, void *arg1) {
    void *child = *(void **)((u8 *)arg1 + 0x24);
    if ((*(u32 *)((u8 *)child + 0x8) & 0x20) == 0) {
        *(u32 *)((u8 *)child + 0x90) = *(u32 *)((u8 *)arg0 + 0xc);
    }
    *(u8 *)((u8 *)arg1 + 0x3) = 0x3;
}
/* fzgx:end fn_1_B03A8 */

/* fzgx:begin fn_1_B1710 */
void fn_1_B1710(int unused, void *ptr) {
    *(u8 *)((u8 *)ptr + 3) = 0x3;
}
/* fzgx:end fn_1_B1710 */

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

/* fzgx:begin fn_1_B5F00 */
typedef struct {
    u8 pad0[0x54];
    f32 value;
} B5F00Source;

typedef struct {
    f32 value0;
    u8 pad4[8];
    f32 valueC;
} B5F00Dest;

void fn_1_B5F00(B5F00Source *source, B5F00Dest *dest) {
    f32 value = dest->value0;
    f32 increment = source->value;
    value += increment;
    dest->value0 = value;
    value = dest->valueC;
    value += increment;
    dest->valueC = value;
}
/* fzgx:end fn_1_B5F00 */

/* fzgx:begin fn_1_B7C00 */
#include "rel/main_rel/memcard.h"

extern u8 lbl_1_bss_71810[161];

u8 fn_1_B7C00(void) {
    u8 result = 0;

    if (lbl_1_bss_716C8.unk_2 != 0xff) {
        result |= 1;
    }
    if (lbl_1_bss_716C8.pad_A0[2] != 0xff) {
        result |= 2;
    }
    if (lbl_1_bss_71810[2] != 0xff) {
        result |= 4;
    }

    return result;
}
/* fzgx:end fn_1_B7C00 */

/* fzgx:begin fn_1_B7C5C */
extern u32 lbl_1_bss_718B4;

u32 fn_1_B7C5C(void) {
    return lbl_1_bss_718B4;
}
/* fzgx:end fn_1_B7C5C */

/* fzgx:begin fn_1_B7CD4 */
extern u8 lbl_1_bss_718B8;

u8 fn_1_B7CD4(void) {
    return lbl_1_bss_718B8;
}
/* fzgx:end fn_1_B7CD4 */

/* fzgx:begin fn_1_B7E14 */
#include "rel/main_rel/memcard.h"
#include "rel/main_rel/globals.h"

// Save the caller's memory-card state in the global work area.
void fn_80083D6C(void *dst, const void *src, int size);

void fn_1_B7E14(void *memory_card_state) {
    fn_80083D6C(lbl_1_bss_716C8.pad_54, memory_card_state, 0x20);
}
/* fzgx:end fn_1_B7E14 */

/* fzgx:begin fn_1_B7E48 */
#include "rel/main_rel/memcard.h"
#include "rel/main_rel/globals.h"

// Return whether either tracked object has its active flag set.
int fn_1_B7E48(void) {
    int result = 0;
    Obj_1_bss_716C8_At24 *object = lbl_1_bss_716C8.unk_24;

    if (object != 0 && (object->unk_8 & 0x200) != 0) {
        result = 1;
    }

    object = *(Obj_1_bss_716C8_At24 **)(lbl_1_bss_716C8.pad_A0 + 0x24);
    if (object != 0 && (object->unk_8 & 0x200) != 0) {
        result = 1;
    }

    return result;
}
/* fzgx:end fn_1_B7E48 */

/* fzgx:begin fn_1_B7E98 */
#include "rel/main_rel/memcard.h"
#include "rel/main_rel/globals.h"

// Return whether the indexed memory-card entry is not in the unavailable state.
int fn_1_B7E98(int index) {
    return (s8)lbl_1_bss_716C8.pad_A0[index * 0xa0 - 0x9f] != -3;
}
/* fzgx:end fn_1_B7E98 */

/* fzgx:begin fn_1_B7EF8 */
extern u8 lbl_1_bss_718B9[7];

// fn_1_B7EF8: Load and return first byte from lbl_1_bss_718B9
u8 fn_1_B7EF8(void) {
    return lbl_1_bss_718B9[0];
}
/* fzgx:end fn_1_B7EF8 */

/* fzgx:begin fn_1_B7F08 */
extern u32 lbl_1_bss_718B4;

u32 fn_1_B7F08(void) {
    return lbl_1_bss_718B4 != 0;
}
/* fzgx:end fn_1_B7F08 */

/* fzgx:begin fn_1_B7F24 */
#include "rel/main_rel/globals.h"

typedef struct {
    u8 pad_0[0x48];
    u32 unk_48;
    u32 unk_4C;
    u32 unk_50;
    u8 pad_54[0x94];
    u32 unk_E8;
    u32 unk_EC;
    u32 unk_F0;
} MemcardState;

extern MemcardState lbl_1_bss_716C8;
extern s8 lbl_1_bss_7730A;

// Reset the memcard state used by the next operation.
void fn_1_B7F24(void) {
    lbl_1_bss_716C8.unk_50 = 0;
    lbl_1_bss_716C8.unk_F0 = 0;
    lbl_1_bss_716C8.unk_4C = 0;
    lbl_1_bss_716C8.unk_48 = 0;
    lbl_1_bss_716C8.unk_EC = 0;
    lbl_1_bss_716C8.unk_E8 = 0;
    lbl_1_bss_7730A = -1;
}
/* fzgx:end fn_1_B7F24 */

/* fzgx:begin fn_1_B7FC8 */
#include "rel/main_rel/memcard.h"

u32 fn_1_B7FC8(void) {
    return lbl_1_bss_718C0.unk_0->unk_0;
}
/* fzgx:end fn_1_B7FC8 */

/* fzgx:begin fn_1_B7FDC */
extern int fn_8002A744(void);
extern void fn_8002A74C(u16 value);

// Toggles the low 16-bit status value and stores the result.
void fn_1_B7FDC(void) {
    u16 value = (u16)fn_8002A744();
    value = (u16)(value ^ 1);
    fn_8002A74C(value);
}
/* fzgx:end fn_1_B7FDC */

/* fzgx:begin fn_1_B800C */
#include "rel/main_rel/globals.h"
#include "rel/main_rel/memcard.h"

extern void *lbl_801A6410;
extern int fn_1_45D0(void *arg0, int arg1, Obj_1_data_3C7B8 *arg2, int arg3);
extern int fn_1_46B4(void *arg0, u32 arg1, Obj_1_data_3C7B8 *arg2, int arg3);
extern int fn_8002DFE0(int arg0, u32 arg1, int arg2);
extern int fn_8002D65C(int arg0);

// Loads a memory-card slot, retrying transient operations and cleaning up failures.
int fn_1_B800C(int index) {
    int result;
    Obj_1_bss_716C8 *slot;

    slot = (Obj_1_bss_716C8 *)((u8 *)&lbl_1_bss_716C8 + index * 0xa0);
    slot->unk_C = fn_1_45D0(lbl_801A6410, 0xA000, &lbl_1_data_3C7B8, 0x256D);
    for (;;) {
        result = fn_8002DFE0(index, slot->unk_C, 0);
        if (result == -1) {
            continue;
        }
        if (result != 0 && result != -6) {
            if (slot->unk_C != 0) {
                fn_1_46B4(lbl_801A6410, slot->unk_C, &lbl_1_data_3C7B8, 0x2573);
                slot->unk_C = 0;
            }
            return result;
        }
        do {
            result = fn_8002D65C(index);
        } while (result == -1);
        return result;
    }
}
/* fzgx:end fn_1_B800C */

/* fzgx:begin fn_1_B80F0 */
#include "rel/main_rel/globals.h"
#include "rel/main_rel/memcard.h"

extern int fn_8002E0C4(int);
extern void fn_1_46B4(void *arg0, u32 arg1, Obj_1_data_3C7B8 *arg2, u32 arg3);
extern void *lbl_801A6410;

// Wait for the card operation, then release its temporary result.
void fn_1_B80F0(int index) {
    Obj_1_bss_716C8 *entry =
        (Obj_1_bss_716C8 *)((u8 *)&lbl_1_bss_716C8 + index * 0xa0);

    while (fn_8002E0C4(index) == -1) {
    }

    if (entry->unk_C != 0) {
        fn_1_46B4(lbl_801A6410, entry->unk_C,
                  &lbl_1_data_3C7B8, 0x258c);
        entry->unk_C = 0;
    }
}
/* fzgx:end fn_1_B80F0 */

/* fzgx:begin fn_1_B9BE0 */
extern void fn_1_B9C38(s32);

void fn_1_B9BE0(void) {
    fn_1_B9C38(0);
    fn_1_B9C38(1);
}
/* fzgx:end fn_1_B9BE0 */

/* fzgx:begin fn_1_B9C0C */
extern void fn_1_B9C38(s32 value);

void fn_1_B9C0C(void) {
    fn_1_B9C38(0);
    fn_1_B9C38(1);
}
/* fzgx:end fn_1_B9C0C */

/* fzgx:begin fn_1_B9C38 */
#include "rel/main_rel/memcard.h"

extern u32 lbl_801A6410;
extern void fn_1_46B4(u32, u32, Obj_1_data_3C7B8 *, u32);

// Saves populated entries, then clears their status and payload fields.
void fn_1_B9C38(s32 index) {
    Obj_1_bss_77380 *entry;
    u8 *entry_bytes;
    s32 i;

    entry = (Obj_1_bss_77380 *)((u8 *)&lbl_1_bss_77380 + index * 0x1800);
    for (i = 0; i < 0x7f; i++) {
        entry_bytes = (u8 *)entry;
        if (entry->unk_2C != 0) {
            fn_1_46B4(lbl_801A6410, entry->unk_2C, &lbl_1_data_3C7B8, 0x2776);
        }
        entry_bytes[1] = 0;
        entry_bytes[2] = 0;
        entry->unk_24 = 0;
        entry->unk_28 = 0;
        entry->unk_2C = 0;
        entry = (Obj_1_bss_77380 *)(entry_bytes + 0x30);
    }
}
/* fzgx:end fn_1_B9C38 */

/* fzgx:begin fn_1_BC29C */
#include "rel/main_rel/memcard.h"

extern void fn_1_46B4(u32, u32, Obj_1_data_3C7B8 *, u32);
extern void fn_1_1596DC(u32);
extern void fn_1_484CC(u32);
extern u32 lbl_801A6410;

// Flush the pending memory-card operation and restore the card state when needed.
void fn_1_BC29C(void) {
    if (lbl_1_bss_7A380 != 0) {
        fn_1_46B4(lbl_801A6410, lbl_1_bss_7A380, &lbl_1_data_3C7B8, 0x2ad4);
        lbl_1_bss_7A380 = 0;
    }
    if (*(s16 *)&lbl_1_bss_960 != 2) {
        fn_1_1596DC(2);
        fn_1_484CC(2);
    }
}
/* fzgx:end fn_1_BC29C */

/* fzgx:begin fn_1_C132C */
typedef struct {
    u8 unk0[0x16];
    s16 unk16;
} Fn1C132CObject;

extern void fn_80008BEC(void *dst, s32 value, s32 size);
extern void fn_1_AA6D8(s32 arg0, u32 arg1, void *arg2);

void fn_1_C132C(Fn1C132CObject *object, u8 *data) {
    u8 local[0x24];

    fn_80008BEC(local, 0, 0x24);
    local[0] = data[1];
    *(u8 **)(local + 0x14) = data + 2;
    fn_1_AA6D8(5, (u8)object->unk16, local);
}
/* fzgx:end fn_1_C132C */

/* fzgx:begin fn_1_C17CC */
// fn_1_C17CC: empty in retail (single blr).
void fn_1_C17CC(void) {
}
/* fzgx:end fn_1_C17CC */

/* fzgx:begin fn_1_C34F0 */
#include "rel/main_rel/globals.h"
#include "rel/main_rel/memcard.h"

extern void fn_1_F755C(u8 value);
extern void fn_80008BA8(const void *dst, void *src, u32 size);
extern void fn_1_A6840(u8 value);
extern void fn_8000C49C(void *arg0, ...);
extern void fn_1_F79C8(void);

extern u8 lbl_1_data_2AC0[128];
extern u8 lbl_1_data_2B40[16];
extern u32 lbl_1_data_2B50;
extern u32 lbl_1_data_2B58;
extern u32 lbl_1_data_2B60;
extern u8 lbl_1_data_3D124[19];

// Initializes the memory-card state and restores the persistent card data.
void fn_1_C34F0(void) {
    Obj_1_bss_718C0_At0 *card_buffer;
    u8 *serialized_data;

    fn_1_F755C(lbl_1_bss_718E0.unk_2);
    fn_80008BA8(lbl_1_data_2AC0, (u8 *)&lbl_1_bss_718E0 + 0x8, 0x40);
    fn_80008BA8(lbl_1_data_2B40, (u8 *)&lbl_1_bss_718E0 + 0x48, 0x8);
    fn_80008BA8(&lbl_1_data_2B50, (u8 *)&lbl_1_bss_718E0 + 0x5542, 0x8);
    fn_80008BA8(&lbl_1_data_2B58, (u8 *)&lbl_1_bss_718E0 + 0x554A, 0x8);
    fn_80008BA8(lbl_1_bss_AA0, (u8 *)&lbl_1_bss_718E0 + 0x50, 0x150);
    fn_80008BA8(&lbl_1_data_2B60, (u8 *)&lbl_1_bss_718E0 + 0x5556, 0x4);

    fn_1_A6840(lbl_1_bss_718E0.unk_3 <= 1 ? lbl_1_bss_718E0.unk_3 : 1);

    card_buffer = lbl_1_bss_718C0.unk_0;
    serialized_data = (u8 *)card_buffer + 0x4;
    fn_80008BA8(&lbl_1_bss_8B3A0.unk_9F, serialized_data, 0x4);
    fn_80008BA8(&lbl_1_bss_8B3A0.unk_8C, serialized_data + 0x4, 0x6);
    fn_80008BA8((u8 *)&lbl_1_bss_8B3A0 + 0xC, serialized_data + 0xA, 0x80);

    if ((u32)(serialized_data + 0x8A - ((u8 *)lbl_1_bss_718C0.unk_0 + 0x4)) != 0x8A) {
        fn_8000C49C(&lbl_1_data_3C7B8, 0x3532, (const char *)lbl_1_data_3D124);
    }
    fn_1_F79C8();
}
/* fzgx:end fn_1_C34F0 */

/* fzgx:begin fn_1_C36EC */
#include "rel/main_rel/globals.h"
#include "rel/main_rel/memcard.h"

extern void fn_80008BA8(void *dst, const void *src, u32 size);
extern void fn_8000C49C(void *arg0, u32 arg1, ...);

// Serializes the current memory-card data into the active buffer.
void fn_1_C36EC(void) {
    u8 *serialized_data = &lbl_1_bss_718C0.unk_0->unk_4;

    fn_80008BA8(serialized_data, &lbl_1_bss_8B3A0.unk_9F, 4);
    fn_80008BA8(serialized_data + 4, &lbl_1_bss_8B3A0.unk_8C, 6);
    fn_80008BA8(serialized_data + 0xA, (u8 *)&lbl_1_bss_8B3A0 + 0xC, 0x80);

    if ((u32)(serialized_data + 0x8A -
              ((u8 *)lbl_1_bss_718C0.unk_0 + 4)) != 0x8A) {
        fn_8000C49C(&lbl_1_data_3C7B8, 0x3578, lbl_1_data_3D124);
    }
}
/* fzgx:end fn_1_C36EC */

/* fzgx:begin fn_1_C39FC */
extern u32 lbl_1_data_3D230;

u32 fn_1_C39FC(void) {
    return lbl_1_data_3D230;
}
/* fzgx:end fn_1_C39FC */
