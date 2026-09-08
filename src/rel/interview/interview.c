#include "types.h"

/* fzgx:begin fn_17_23C */
extern s32 lbl_17_bss_58[2];
extern s32 lbl_17_bss_24[4];
extern void fn_17_6C8(void);

void fn_17_23C(void) {
    if (lbl_17_bss_58[0] & 0x10000000) {
        if (lbl_17_bss_24[0] != 0) {
            lbl_17_bss_24[0]--;
        }
        if (lbl_17_bss_24[0] == 0) {
            lbl_17_bss_24[0] = 30;
            lbl_17_bss_58[0] &= ~0x10000000;
            fn_17_6C8();
        }
    }
}
/* fzgx:end fn_17_23C */

/* fzgx:begin fn_17_394 */
extern u8 lbl_17_data_63E30[32];
extern s16 lbl_1_bss_962;
extern u32 lbl_1_bss_71688;
extern u32 lbl_1_bss_7168C;

typedef void (*InterviewFunc)(void);

typedef struct InterviewEntry {
    u32 unk_0;
    InterviewFunc func;
    u32 value_8;
    u32 value_c;
} InterviewEntry;

void fn_17_394(void) {
    InterviewEntry *entry;
    s16 index;

    entry = (InterviewEntry *)lbl_17_data_63E30;
    index = lbl_1_bss_962;
    entry += index - 0xc5;
    lbl_1_bss_71688 = entry->value_8;
    lbl_1_bss_7168C = entry->value_c;
    entry->func();
}
/* fzgx:end fn_17_394 */

/* fzgx:begin fn_17_1794 */
// fn_17_1794: empty in retail (single blr).
void fn_17_1794(void) {
}
/* fzgx:end fn_17_1794 */

/* fzgx:begin fn_17_2D24 */
extern u32 lbl_17_bss_14;
extern u32 lbl_17_bss_18[2];
extern void fn_17_3270(u32, u32);

void fn_17_2D24(void) {
    fn_17_3270(lbl_17_bss_14, lbl_17_bss_18[0]);
}
/* fzgx:end fn_17_2D24 */

/* fzgx:begin fn_17_3270 */
typedef struct InterviewState {
    u32 first;
    u32 second;
} InterviewState;

extern InterviewState *lbl_1_bss_6EAD0;
extern u32 lbl_801A6410;

extern void fn_8004C698(u32 value);
extern void fn_80051E30(u32 value);
extern void fn_1_A0AA4(void);
extern void fn_17_416C(void);
extern void fn_17_4858(void);
extern void fn_17_49BC(void);
extern void fn_17_4FE0(void);
extern u32 lbl_17_bss_C;
extern u8 lbl_17_data_63E50[592];
extern void fn_1_46B4(u32, u32, void *, u32);
extern void fn_1_435C(void *);
extern void fn_1_41A8(void);
extern void fn_1_410A0(void);
extern void fn_17_602C(void);

void fn_17_3270(void *arg0, void *arg1) {
    fn_8004C698(lbl_1_bss_6EAD0->first);
    fn_8004C698(lbl_1_bss_6EAD0->second);
    fn_80051E30(lbl_1_bss_6EAD0->second);
    fn_1_A0AA4();
    fn_17_416C();
    fn_17_4858();
    fn_17_49BC();
    fn_17_4FE0();

    if (lbl_17_bss_C != 0) {
        fn_1_46B4(lbl_801A6410, lbl_17_bss_C, lbl_17_data_63E50, 0x81d);
        lbl_17_bss_C = 0;
    }

    fn_1_435C(arg0);
    fn_1_41A8();
    fn_1_435C(arg1);
    fn_1_41A8();
    fn_1_410A0();
    fn_17_602C();
}
/* fzgx:end fn_17_3270 */

/* fzgx:begin fn_17_4214 */
extern u32 lbl_17_rodata_1B8;

struct fn_17_4214_Arg0 {
    u8 pad_0[0x88];
    u32 unk_88;
    u8 pad_8C[0x14];
    u16 unk_A0;
};
struct fn_17_4214_Copy12 { u32 a[3]; };

void fn_17_4214(struct fn_17_4214_Arg0 *arg0) {
    if ((s32)arg0->unk_88 != 4) { return; }
    *(struct fn_17_4214_Copy12 *)((u8 *)(u32)arg0 + 168) = *(struct fn_17_4214_Copy12 *)&lbl_17_rodata_1B8;
    arg0->unk_A0 = 12288;
}
/* fzgx:end fn_17_4214 */

/* fzgx:begin fn_17_4540 */
typedef struct {
    u8 pad[0x88];
    // Hardware-style shared state must be read exactly as published.
    volatile int value;
} InterviewState;

typedef struct {
    u8 pad[0xc];
    // Hardware-style shared state must be read exactly as published.
    volatile s16 value;
} InterviewData;

extern InterviewState *lbl_17_bss_D8;
extern InterviewData lbl_17_bss_60;

void fn_17_4540(void) {
    InterviewState *state = lbl_17_bss_D8;
    InterviewData *data = &lbl_17_bss_60;

    if (state->value != 7)
        return;
    if (data->value == 8)
        return;
}
/* fzgx:end fn_17_4540 */

/* fzgx:begin fn_17_4858 */
extern u32 lbl_17_bss_58[2];
extern u32 lbl_17_bss_198[95];
extern u8 lbl_17_data_63E50[592];
extern u32 lbl_801A6410;

extern void fn_1_8F494(void *arg0);
extern void fn_1_46B4(u32 arg0, u32 arg1, void *arg2, s32 arg3);

void fn_17_4858(void) {
    if ((lbl_17_bss_58[0] & 0x00400000) != 0 &&
        lbl_17_bss_198[0] != 0) {
        fn_1_8F494((void *)lbl_17_bss_198[0]);
        fn_1_46B4(lbl_801A6410, lbl_17_bss_198[0], lbl_17_data_63E50, 0xae9);
        lbl_17_bss_198[0] = 0;
    }
}
/* fzgx:end fn_17_4858 */

/* fzgx:begin fn_17_49B8 */
// fn_17_49B8: empty in retail (single blr).
void fn_17_49B8(void) {
}
/* fzgx:end fn_17_49B8 */

/* fzgx:begin fn_17_49BC */
extern void fn_1_79100(void);

void fn_17_49BC(void) {
    fn_1_79100();
}
/* fzgx:end fn_17_49BC */

/* fzgx:begin fn_17_4E50 */
// fn_17_4E50: empty in retail (single blr).
void fn_17_4E50(void) {
}
/* fzgx:end fn_17_4E50 */

/* fzgx:begin fn_17_5FA8 */
typedef struct InterviewState {
    u8 pad_00[0x84];
    s16 field_84;
    u8 pad_86[0x12];
    s16 field_98;
} InterviewState;

typedef struct InterviewGlobals {
    s32 field_00;
    u8 pad_04[0x10];
    InterviewState *field_14;
} InterviewGlobals;

extern InterviewGlobals lbl_17_bss_60;
extern u32 lbl_17_bss_58[2];
extern void fn_17_60B4(void);
extern void fn_1_154F1C(void);
extern void fn_17_8C7C(s32 arg0);

void fn_17_5FA8(void) {
    InterviewGlobals *globals = &lbl_17_bss_60;
    InterviewState *state = globals->field_14;

    fn_17_60B4();
    if (globals->field_00 >= state->field_84 + 0x14a &&
        (lbl_17_bss_58[0] & 0x00800000) != 0 &&
        globals->field_00 < state->field_98) {
        fn_1_154F1C();
        fn_17_8C7C(0x140);
    }
}
/* fzgx:end fn_17_5FA8 */

/* fzgx:begin fn_17_602C */
extern u8 lbl_17_bss_60[118];
extern u8 lbl_17_data_63E50[592];
extern u32 lbl_801A6410;
extern void fn_1_46B4(u32, u32, void *, u32);

void fn_17_602C(void) {
    u32 value;

    value = *(u32 *)(lbl_17_bss_60 + 0x10);
    if (value != 0) {
        fn_1_46B4(lbl_801A6410, value, lbl_17_data_63E50, 0xed7);
    }
}
/* fzgx:end fn_17_602C */

/* fzgx:begin fn_17_6078 */
extern u8 lbl_17_bss_D8[34];
extern void fn_17_65F4(void);

typedef struct {
    u8 pad[0x88];
    int state;
} InterviewState;

void fn_17_6078(void) {
    InterviewState *state = *(InterviewState **)lbl_17_bss_D8;

    switch (state->state) {
    case 0x1c:
        fn_17_65F4();
        break;
    default:
        break;
    }
}
/* fzgx:end fn_17_6078 */

/* fzgx:begin fn_17_60B4 */
typedef struct InterviewState {
    s32 value;
    u8 unk04[8];
    s16 status;
    u8 unk0E[6];
    void *data;
} InterviewState;

typedef struct InterviewData {
    u8 unk00[0x84];
    s16 lower_bound;
    u8 unk86[0x12];
    s16 upper_bound;
} InterviewData;

typedef struct InterviewManager {
    u8 unk00[0x18];
    u32 flags;
} InterviewManager;

extern InterviewState lbl_17_bss_60;
extern InterviewManager *lbl_17_bss_D8;
extern void fn_17_613C(void);

void fn_17_60B4(void) {
    InterviewState *state = &lbl_17_bss_60;
    InterviewData *data = state->data;

    if (state->status == 9 &&
        state->value >= data->lower_bound &&
        state->value < data->upper_bound) {
        lbl_17_bss_D8->flags |= 0x08000000;
        fn_17_613C();
    } else {
        lbl_17_bss_D8->flags &= ~0x08000000;
    }
}
/* fzgx:end fn_17_60B4 */

/* fzgx:begin fn_17_7020 */
typedef struct InterviewObject {
    u8 pad0[0x14];
    struct InterviewText *text;
} InterviewObject;

typedef struct InterviewText {
    u8 pad0[0x9A];
    u8 text[1];
} InterviewText;

extern u8 lbl_17_data_64458[4];
extern u8 lbl_17_bss_D8[34];

extern void fn_1_97204(void *arg0, u8 *arg1);

void fn_17_7020(InterviewObject *object) {
    InterviewText *text;
    u8 *value;

    text = object->text;
    if (text->text[0] == 0) {
        value = lbl_17_data_64458;
    } else {
        u8 *cursor;

        cursor = text->text;
        while (*cursor != 0x3C && *cursor != 0) {
            cursor++;
        }
        *cursor = 0;
        value = text->text;
    }
    fn_1_97204(*(void **)(lbl_17_bss_D8 + 4), value);
}
/* fzgx:end fn_17_7020 */

/* fzgx:begin fn_17_71C0 */
typedef struct InterviewState {
    u8 pad[4];
    u32 flags;
} InterviewState;

void fn_17_71C0(InterviewState *self) {
    self->flags &= ~2;
    self->flags &= 1;
}
/* fzgx:end fn_17_71C0 */

/* fzgx:begin fn_17_72BC */
// fn_17_72BC: empty in retail (single blr).
void fn_17_72BC(void) {
}
/* fzgx:end fn_17_72BC */

/* fzgx:begin fn_17_74C0 */
extern u8 lbl_17_bss_D8[34];
extern void fn_17_7728(void *);

typedef struct InterviewChild {
    u8 pad_00[0x42];
    s16 value_42;
    s16 value_44;
} InterviewChild;

typedef struct InterviewState {
    u8 pad_00[0x88];
    s32 value_88;
} InterviewState;

typedef struct Interview {
    u8 pad_00[0x0c];
    s16 value_0c;
    u8 pad_0e[0x4a];
    s16 value_58;
} Interview;

void fn_17_74C0(Interview *interview) {
    InterviewState *state = *(InterviewState **)lbl_17_bss_D8;
    InterviewChild *child = (InterviewChild *)((u8 *)interview + 0x18);

    interview->value_58 = 0;
    child->value_42 = interview->value_0c;
    child->value_44 = state->value_88;
    fn_17_7728(child);
}
/* fzgx:end fn_17_74C0 */

/* fzgx:begin fn_17_87C0 */
extern f32 lbl_17_rodata_0[];
extern u32 lbl_1_rodata_26F8[];

extern u8 fn_1_B7C00(void);
extern void fn_1_4F734(void*);

typedef struct {
    u32 word04;
    f32 float08;
    f32 float0c;
    f32 float10;
    u32 word14[7];
    f32 float30;
    u32 word34;
    u32 word38[9];
} InterviewParams;

void fn_17_87C0(f32 value) {
    f32* rodata = lbl_17_rodata_0;
    InterviewParams params;

    if (!fn_1_B7C00()) {
        params = *(InterviewParams*)lbl_1_rodata_26F8;
        params.word04 = 0x00008c03;
        params.float08 = rodata[168];
        params.float0c = rodata[169];
        params.float10 = rodata[108];
        params.float30 = value;
        params.word34 = 5;
        fn_1_4F734(&params.word04);
    }
}
/* fzgx:end fn_17_87C0 */
