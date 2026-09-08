#include "types.h"

/* fzgx:begin fn_1_72648 */
extern u32 lbl_1_bss_6C8DC[2];
extern u32 lbl_801A6410;
extern u8 lbl_1_data_1DAC0[68];
extern void fn_1_46B4(u32 arg0, u32 arg1, u8 *arg2, s32 arg3);

void fn_1_72648(void) {
    if (lbl_1_bss_6C8DC[0] != 0) {
        fn_1_46B4(lbl_801A6410, lbl_1_bss_6C8DC[0], lbl_1_data_1DAC0, 0xf9);
        lbl_1_bss_6C8DC[0] = 0;
    }
}
/* fzgx:end fn_1_72648 */

/* fzgx:begin fn_1_7269C */
#include "rel/main_rel/screen_effect.h"

extern void fn_1_727BC(u32 arg0, u32 arg1, void *arg2);
extern void fn_1_76A94(void *arg0, u32 arg1);
extern void fn_1_76DBC(void);

typedef struct {
    u8 pad_0[0xa0];
    u32 unk_A0;
    u8 pad_A4[0x24];
    u32 unk_C8;
} ScreenEffectEntry;

void fn_1_7269C(u32 arg0, u32 arg1, void *arg2) {
    ScreenEffectEntry *object;

    object = (ScreenEffectEntry *)((u8 *)&lbl_1_bss_6C8EC + (arg0 & 0xff) * 0x10c);
    fn_1_727BC(arg0, arg1, arg2);
    ((ScreenEffectEntry *)((u8 *)object + (arg1 & 0xff) * 4))->unk_A0 = (u32)arg2;
    ((ScreenEffectEntry *)((u8 *)object + (arg1 & 0xff) * 4))->unk_C8 = 1;
    fn_1_76A94(object, arg1);
    fn_1_76DBC();
}
/* fzgx:end fn_1_7269C */

/* fzgx:begin fn_1_72714 */
extern void fn_1_727BC(void);
extern u8 lbl_1_bss_6C8EC[];

void *fn_1_72714(u8 arg0, u8 arg1) {
    u32 offset;
    u8 *result;

    fn_1_727BC();
    offset = arg0 * 0x10c;
    result = &lbl_1_bss_6C8EC[offset];
    return result + (arg1 << 5);
}
/* fzgx:end fn_1_72714 */

/* fzgx:begin fn_1_72768 */
typedef struct {
    u8 pad_0[0x104];
    u32 unk_104;
    u8 unk_108;
    u8 pad_109[3];
} ScreenEffect;

extern ScreenEffect lbl_1_bss_6C8EC[];
extern void fn_1_727BC(u8 arg0, u32 arg1);

void fn_1_72768(u8 arg0, u32 arg1) {
    ScreenEffect *entry;
    u32 index;

    fn_1_727BC(arg0, 0);
    index = arg0;
    entry = &lbl_1_bss_6C8EC[index];
    entry->unk_104 = arg1;
}
/* fzgx:end fn_1_72768 */

/* fzgx:begin fn_1_72848 */
extern u8 lbl_1_bss_6C8EC[2144];
extern void fn_1_76650(void *arg);
extern u8 lbl_1_bss_6D14C[92];

void fn_1_72848(void) {
    u8 i;

    for (i = 0; i < 8; i++) {
        fn_1_76650(&lbl_1_bss_6C8EC[i * 0x10c]);
    }
    lbl_1_bss_6D14C[0] = 0;
}
/* fzgx:end fn_1_72848 */

/* fzgx:begin fn_1_728B0 */
#include "rel/main_rel/screen_effect.h"

typedef struct {
    u8 pad_0[0xdc];
    s32 unk_DC;
    s32 unk_E0;
    u8 pad_E4[0x20];
    void (*unk_104)(void *);
} ScreenEffectEntry;

extern void fn_1_4E500(void);
extern void fn_1_4E638(int);
extern void fn_1_72980(ScreenEffectEntry *);
extern void *fn_1_729F8(ScreenEffectEntry *);
extern void fn_1_4E6F4(void);

void fn_1_728B0(void) {
    u8 i;
    u8 *base;
    ScreenEffectEntry *obj;
    void *result;

    if (lbl_1_bss_6D14C.unk_0 == 0) {
        fn_1_4E500();
        fn_1_4E638(0);
        base = (u8 *)&lbl_1_bss_6C8EC;
        for (i = 0; i < 8; i++) {
            obj = (ScreenEffectEntry *)(base + i * 0x10c);
            if (obj->unk_DC != 0) {
                if (obj->unk_DC == 0xe && obj->unk_E0 == 0) {
                    obj->unk_E0 = obj->unk_E0 + 1;
                } else {
                    fn_1_72980(obj);
                    result = fn_1_729F8(obj);
                    if (obj->unk_104 != 0) {
                        obj->unk_104(result);
                    }
                }
            }
        }
        fn_1_4E6F4();
    }
}
/* fzgx:end fn_1_728B0 */

/* fzgx:begin fn_1_72980 */
#include "rel/main_rel/screen_effect.h"

extern void fn_80073778(void *arg0, int arg1);

typedef struct {
    u8 pad_0[0xdc];
    u32 unk_dc;
} Fn_1_72980_Obj;

void fn_1_72980(Fn_1_72980_Obj *arg0) {
    int index;
    u8 *entry;
    int i;

    entry = (u8 *)arg0;
    i = 0;
    index = 0;
    while (i < ((u8 *)&lbl_1_data_1D9B8)[arg0->unk_dc * 6 + 5]) {
        fn_80073778(entry, index);
        index++;
        entry += 0x20;
        i++;
    }
}
/* fzgx:end fn_1_72980 */

/* fzgx:begin fn_1_76650 */
extern s32 fn_1_A5D9C(void);
extern const f32 lbl_1_rodata_31AC;

typedef struct ScreenEffect {
    u8 pad_00[0xa0];
    s32 field_a0;
    s32 field_a4;
    s32 field_a8;
    s32 field_ac;
    s32 field_b0;
    u8 pad_b4[0x14];
    s32 field_c8;
    s32 field_cc;
    s32 field_d0;
    s32 field_d4;
    s32 field_d8;
    s32 field_dc;
    s32 field_e0;
    f32 field_e4;
    s16 field_e8;
    s16 field_ea;
    s16 field_ec;
    s16 field_ee;
    s16 field_f0;
    s16 field_f2;
    s32 field_f4;
    s16 field_f8;
    s16 field_fa;
    s16 field_fc;
    s16 field_fe;
    s16 field_100;
    s16 field_102;
    s32 field_104;
    u8 field_108;
} ScreenEffect;

void fn_1_76650(ScreenEffect *effect) {
    s32 value;

    effect->field_f2 = 0;
    effect->field_f0 = 0;
    effect->field_ea = 0;
    effect->field_e8 = 0;
    effect->field_ec = 0x280;
    effect->field_ee = 0x1e0;
    effect->field_f4 = 0;
    effect->field_fa = 0;
    effect->field_f8 = 0;
    effect->field_100 = 0x280;
    effect->field_fc = 0x280;
    value = fn_1_A5D9C();
    effect->field_102 = value;
    effect->field_fe = value;
    effect->field_dc = 0;
    effect->field_e0 = 0;
    effect->field_e4 = lbl_1_rodata_31AC;
    effect->field_104 = 0;
    effect->field_108 = 0;
    effect->field_a0 = 0;
    effect->field_c8 = 0;
    effect->field_a4 = 0;
    effect->field_cc = 0;
    effect->field_a8 = 0;
    effect->field_d0 = 0;
    effect->field_ac = 0;
    effect->field_d4 = 0;
    effect->field_b0 = 0;
    effect->field_d8 = 0;
}
/* fzgx:end fn_1_76650 */

/* fzgx:begin fn_1_76BD0 */
#include "rel/main_rel/screen_effect.h"

void fn_1_76BD0(u8 index, u8 value) {
    Obj_1_bss_6C8EC *obj =
        (Obj_1_bss_6C8EC *)((u8 *)&lbl_1_bss_6C8EC + index * 0x10c);

    if ((s32)obj->unk_DC != 0) {
        obj->unk_108 = value;
    }
}
/* fzgx:end fn_1_76BD0 */

/* fzgx:begin fn_1_76BF8 */
#include "rel/main_rel/screen_effect.h"

extern void fn_1_76C60(void);

typedef struct {
    u8 pad_0[0xDC];
    s32 unk_DC;
    u8 pad_E0[0x2C];
} ScreenEffectEntry;

void fn_1_76BF8(void) {
    u8 i;
    ScreenEffectEntry *entry;

    entry = (ScreenEffectEntry *)&lbl_1_bss_6C8EC;
    for (i = 0; i < 8; i++) {
        entry = (ScreenEffectEntry *)((u8 *)&lbl_1_bss_6C8EC + i * 0x10C);
        if (entry->unk_DC != 0) {
            fn_1_76C60();
        }
    }
}
/* fzgx:end fn_1_76BF8 */

/* fzgx:begin fn_1_76C60 */
#include "rel/main_rel/screen_effect.h"

extern u32 fn_80008E84(u32 arg);
extern void fn_1_46B4(u32 arg0, u32 arg1, void *arg2, u32 arg3);
extern u16 fn_1_A5D9C(void);
extern void fn_1_76DBC(void);
extern u32 lbl_801A6410;

typedef struct {
    u8 pad_0[0xa0];
    u32 unk_a0[5];
    u32 unk_b4[5];
    s32 unk_c8[5];
    u32 unk_dc;
    u32 unk_e0;
    f32 unk_e4;
    u16 unk_e8;
    u16 unk_ea;
    u16 unk_ec;
    u16 unk_ee;
    u16 unk_f0;
    u16 unk_f2;
    u32 unk_f4;
    u16 unk_f8;
    u16 unk_fa;
    u16 unk_fc;
    u16 unk_fe;
    u16 unk_100;
    u16 unk_102;
    u32 unk_104;
    u8 unk_108;
} FnScreenEffect;

extern u32 lbl_1_bss_6C8E4;
extern u32 lbl_1_bss_6C8E8;
extern u8 lbl_1_data_1DAC0[68];
extern const f32 lbl_1_rodata_31AC;

void fn_1_76C60(FnScreenEffect *arg) {
    u8 i;
    u16 value;

    lbl_1_bss_6C8E8 = fn_80008E84(lbl_1_bss_6C8E4);
    i = 0;
    while (i < ((u8 *)&lbl_1_data_1D9B8)[arg->unk_dc * 6 + 5]) {
        if (arg->unk_c8[i] == 0) {
            fn_1_46B4(lbl_801A6410, arg->unk_a0[i], lbl_1_data_1DAC0, 0x9a1);
            arg->unk_a0[i] = 0;
        }
        i++;
    }
    fn_80008E84(lbl_1_bss_6C8E8);
    arg->unk_f2 = 0;
    arg->unk_f0 = 0;
    arg->unk_ea = 0;
    arg->unk_e8 = 0;
    arg->unk_ec = 0x280;
    arg->unk_ee = 0x1e0;
    arg->unk_f4 = 0;
    arg->unk_fa = 0;
    arg->unk_f8 = 0;
    arg->unk_100 = 0x280;
    arg->unk_fc = 0x280;
    value = fn_1_A5D9C();
    arg->unk_102 = value;
    arg->unk_fe = value;
    arg->unk_dc = 0;
    arg->unk_e0 = 0;
    arg->unk_e4 = lbl_1_rodata_31AC;
    arg->unk_104 = 0;
    arg->unk_108 = 0;
    arg->unk_a0[0] = 0;
    arg->unk_c8[0] = 0;
    arg->unk_a0[1] = 0;
    arg->unk_c8[1] = 0;
    arg->unk_a0[2] = 0;
    arg->unk_c8[2] = 0;
    arg->unk_a0[3] = 0;
    arg->unk_c8[3] = 0;
    arg->unk_a0[4] = 0;
    arg->unk_c8[4] = 0;
    fn_1_76DBC();
}
/* fzgx:end fn_1_76C60 */

/* fzgx:begin fn_1_76DBC */
// fn_1_76DBC: empty in retail (single blr).
void fn_1_76DBC(void) {
}
/* fzgx:end fn_1_76DBC */

/* fzgx:begin fn_1_77200 */
extern void fn_1_77238(void);
extern void fn_1_772E0(void);
extern void fn_1_77384(void *arg);

void fn_1_77200(void *arg) {
    fn_1_77238();
    fn_1_772E0();
    fn_1_77384(arg);
}
/* fzgx:end fn_1_77200 */

/* fzgx:begin fn_1_77238 */
extern void fn_80072864(int arg0);
extern void fn_800745A4(int arg0, int arg1, int arg2, int arg3, int arg4, int arg5);
extern void fn_80074660(int arg0);
extern void fn_80073678(int arg0);
extern void fn_80073898(int arg0);
extern void fn_80074788(int arg0);
extern void fn_800738E0(int arg0, int arg1, int arg2);
extern void fn_80073D60(int arg0, int arg1, int arg2, int arg3, int arg4);
extern void fn_800734A8(int arg0, int arg1, int arg2, int arg3);
extern void fn_80072EDC(int arg0, int arg1);

void fn_1_77238(void) {
    fn_80072864(2);
    fn_800745A4(0, 1, 4, 30, 0, 125);
    fn_80074660(1);
    fn_80073678(1);
    fn_80073898(1);
    fn_80074788(0);
    fn_800738E0(0, 0, 1);
    fn_80073D60(0, 0, 1, 0, 1);
    fn_800734A8(0, 0, 0, 255);
    fn_80072EDC(0, 3);
}
/* fzgx:end fn_1_77238 */

/* fzgx:begin fn_1_772E0 */
extern f32 lbl_1_rodata_31B0[10];
extern const f64 lbl_1_rodata_31A0;

extern void lbl_8006D784(f32 *arg);
extern void fn_80073A58(int arg0, f32 *arg1, int arg2);
extern void fn_800739E0(int arg0, int arg1, int arg2);
extern void fn_80038D34(f32 *arg0, int arg1, int arg2);

void fn_1_772E0(void) {
    f32 first[16];
    f32 second[8];
    f32 values[6];

    lbl_8006D784(&first[4]);
    values[0] = lbl_1_rodata_31B0[0] * first[4];
    values[1] = lbl_1_rodata_31B0[0] * first[5];
    values[2] = *(const f32 *)&lbl_1_rodata_31A0;
    values[3] = lbl_1_rodata_31B0[0] * first[8];
    values[4] = lbl_1_rodata_31B0[0] * first[9];
    values[5] = *(const f32 *)&lbl_1_rodata_31A0;
    fn_80073A58(1, values, 1);
    fn_800739E0(0, 0, 0);
    lbl_8006D784(second);
    fn_80038D34(second, 30, 1);
}
/* fzgx:end fn_1_772E0 */

/* fzgx:begin fn_1_78950 */
extern u8 lbl_1_bss_6D14C[92];
extern void fn_8006FDEC(void);
extern void fn_8006FEFC(void);
extern void fn_8006FD1C(void);

void fn_1_78950(void) {
    int i;

    lbl_1_bss_6D14C[0] = 1;
    for (i = 0; i < 2; i++) {
        fn_8006FDEC();
        fn_8006FEFC();
        fn_8006FD1C();
    }
}
/* fzgx:end fn_1_78950 */

/* fzgx:begin fn_1_7899C */
// fn_1_7899C: main_rel .text:0x0007899C size 0x10
// Zero the first byte of lbl_1_bss_6D14C

extern u8 lbl_1_bss_6D14C[92];

void fn_1_7899C(void) {
    lbl_1_bss_6D14C[0] = 0;
}
/* fzgx:end fn_1_7899C */

/* fzgx:begin fn_1_789AC */
#include "rel/main_rel/screen_effect.h"

extern void fn_1_79810(void);
extern void fn_1_79948(void);
extern void fn_1_791B0(void);
extern void fn_1_9A1E8(void);

void fn_1_789AC(void) {
    fn_1_79810();
    fn_1_79948();
    fn_1_791B0();
    fn_1_9A1E8();
    lbl_1_data_1DFA4.unk_4 = 0;
}
/* fzgx:end fn_1_789AC */

/* fzgx:begin fn_1_79100 */
extern void fn_1_9A864(void);
extern u8 fn_1_7B074(void);
extern u32 lbl_1_bss_6D770;
extern u32 lbl_1_bss_6D774;
extern void fn_8006FDEC(void);
extern void fn_80071718(void *arg);
extern void fn_800711A8(void *arg);
extern void fn_1_14CB4(void);
extern void fn_1_FA84(void);
extern s16 lbl_1_data_1DFA4[6];

void fn_1_79100(void) {
    fn_1_9A864();
    if (!fn_1_7B074()) {
        if (lbl_1_bss_6D770 != 0 || lbl_1_bss_6D774 != 0) {
            fn_8006FDEC();
        }
        if (lbl_1_bss_6D770 != 0) {
            fn_80071718((void *)lbl_1_bss_6D770);
            lbl_1_bss_6D770 = 0;
        }
        if (lbl_1_bss_6D774 != 0) {
            fn_800711A8((void *)lbl_1_bss_6D774);
            lbl_1_bss_6D774 = 0;
        }
    }
    fn_1_14CB4();
    fn_1_FA84();
    lbl_1_data_1DFA4[4] = -1;
}
/* fzgx:end fn_1_79100 */

/* fzgx:begin fn_1_7A648 */
extern const f32 lbl_1_rodata_32CC;
extern void fn_1_79C88(void *arg0, s32 arg1, f32 arg2, s32 arg3);

void fn_1_7A648(void *arg0) {
    fn_1_79C88(arg0, 0, lbl_1_rodata_32CC, 0);
}
/* fzgx:end fn_1_7A648 */

/* fzgx:begin fn_1_7A9B8 */
typedef struct {
    u8 unk00[0x18];
    s32 count;
    void *nodes;
} Manager;

typedef struct {
    u8 unk00[0xf4];
    void *field_f4;
    u8 unk_f8[0x4b0 - 0xf8];
} Node;

typedef struct {
    u8 unk00[0x2c];
    void *field_2c;
    u8 unk30[0x8c - 0x30];
    void *field_8c;
    u8 unk90[0xa0 - 0x90];
} Entry;

extern Manager *lbl_1_bss_3BE0;
extern Entry *lbl_1_bss_3BE4;

extern void lbl_8006DBAC(void *arg0);
extern void lbl_8006E1B0(void *arg0, void *arg1);

void fn_1_7A9B8(void) {
    Entry *table;
    Node *node;
    s32 i;

    i = 0;
    table = lbl_1_bss_3BE4;
    node = (Node *)lbl_1_bss_3BE0->nodes;
    while (i < lbl_1_bss_3BE0->count) {
        if (node->field_f4 != 0) {
            lbl_8006DBAC(&table->field_2c);
            lbl_8006E1B0(node->field_f4, &table->field_8c);
        }
        i++;
        table++;
        node++;
    }
}
/* fzgx:end fn_1_7A9B8 */

/* fzgx:begin fn_1_7B054 */
extern s16 lbl_1_data_1DFA4[6];

s16 fn_1_7B054(void) {
    return lbl_1_data_1DFA4[4];
}
/* fzgx:end fn_1_7B054 */

/* fzgx:begin fn_1_7B064 */
extern u32 lbl_1_bss_6D774;

u32 fn_1_7B064(void) {
    return lbl_1_bss_6D774;
}
/* fzgx:end fn_1_7B064 */

/* fzgx:begin fn_1_7B074 */
#include "rel/main_rel/screen_effect.h"

u8 fn_1_7B074(void) {
    return lbl_1_bss_6D600.unk_0;
}
/* fzgx:end fn_1_7B074 */

/* fzgx:begin fn_1_7B084 */
#include "rel/main_rel/screen_effect.h"

extern u32 lbl_1_data_1DCF0;

extern void fn_8008069C(char *dst, const char *format, ...);
extern void fn_1_465D0(char *text, int arg);
extern void fn_1_14D5C(void *object, int arg);
extern void fn_1_46EA8(int value);
extern void fn_1_12620(void *object);

void fn_1_7B084(void *object, int value) {
    char text_a[32];
    char text_b[32];
    const char *data = (const char *)&lbl_1_data_1DCF0;

    fn_8008069C(text_a, data + 0x7d0, data + 0x7e0, object);
    fn_8008069C(text_b, data + 0x7e8, data + 0x7e0, object);
    if (value == 0) {
        fn_1_465D0(text_a, 1);
        fn_1_465D0(text_b, 1);
        fn_1_14D5C(object, 0);
    } else {
        fn_1_465D0(text_a, 2);
        fn_1_465D0(text_b, 2);
        fn_1_14D5C(object, 1);
    }
    fn_1_46EA8(value);
    fn_1_12620(object);
    fn_8008069C(text_a, data + 0x7f8, object);
    fn_1_465D0(text_a, 1);
    fn_1_46EA8(0);
}
/* fzgx:end fn_1_7B084 */

/* fzgx:begin fn_1_7B184 */
#include "rel/main_rel/screen_effect.h"

extern void fn_8008069C(void *dst, const char *fmt, ...);
extern void fn_1_46DC4(void *obj);

void fn_1_7B184(void *arg) {
    char buf_48[0x20];
    char buf_28[0x20];
    char buf_08[0x20];
    u8 *base = (u8 *)&lbl_1_data_1DCF0;

    fn_8008069C(buf_48, (const char *)(base + 0x7d0), (const char *)(base + 0x7e0), arg);
    fn_8008069C(buf_28, (const char *)(base + 0x7e8), (const char *)(base + 0x7e0), arg);
    fn_8008069C(buf_08, (const char *)(base + 0x810), arg);

    fn_1_46DC4(buf_48);
    fn_1_46DC4(buf_28);
    fn_1_46DC4(buf_08);
}
/* fzgx:end fn_1_7B184 */

/* fzgx:begin fn_1_7B4C0 */
#include "rel/main_rel/screen_effect.h"

extern void fn_8006FDEC(void);
extern void fn_80071718(u32);
extern void fn_800711A8(u32);

void fn_1_7B4C0(void) {
    if (lbl_1_bss_6D770 != 0 || lbl_1_bss_6D774 != 0) {
        fn_8006FDEC();
    }

    if (lbl_1_bss_6D770 != 0) {
        fn_80071718(lbl_1_bss_6D770);
        lbl_1_bss_6D770 = 0;
    }

    if (lbl_1_bss_6D774 != 0) {
        fn_800711A8(lbl_1_bss_6D774);
        lbl_1_bss_6D774 = 0;
    }

    lbl_1_data_1DFA4.unk_8 = -1;
}
/* fzgx:end fn_1_7B4C0 */
