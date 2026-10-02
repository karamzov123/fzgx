#include "types.h"
#include "rel/main_rel/globals.h"
#include "rel/main_rel/car_test.h"

extern void fn_1_5634C(u32 arg0);
extern void fn_1_7ECB8(u32 arg0, u32 arg1, u32 arg2);
extern void *lbl_801A6410;
extern const f64 lbl_1_rodata_3588;
extern void fn_1_12AB38(const char *);
extern void *fn_1_45D0(void *, s32, const char *, s32);
extern void fn_80008BEC(void *, s32, s32);
extern void fn_1_80F80(void *, u8, void *);
extern void fn_1_1502BC(void *, void *, void *);

extern void fn_80083DB0(void* arg0, u32 arg1);
extern void fn_1_801F8(s16 arg0, void* arg1);
extern const f32 lbl_1_rodata_33AC;
extern void *fn_1_4630(void *arg0, u32 arg1, void *arg2, u32 arg3);
extern const f32 lbl_1_rodata_33B0[84];
extern void *memset(void *dst, int value, u32 size);
extern void fn_1_46B4(void *arg0, void *arg1, u8 *arg2, s32 arg3);
extern void fn_1_7F954(void *arg0, s32 arg1);
extern void fn_1_49614(void);
extern const f32 lbl_1_rodata_33A8;
extern void fn_80071ED4(s32 arg0, f32 arg1, f32 arg2);
extern void fn_80072014(void *arg0);
extern void fn_800720B0(s32 arg0);

/* fzgx:begin fn_1_7C13C */
#include "types.h"

typedef struct {
    u8 pad_32c[0x32c];
    void *field_32c;
    u8 pad_330[0x110];
} Fn17C13CEntry;

typedef struct {
    u8 pad_4[4];
    u16 field_4;
    u16 field_6;
    u8 pad_8[0x17c];
    f32 field_184;
} Fn17C13CObject;

void fn_1_7C13C(Fn17C13CEntry *arg0) {
    f32 value;
    Fn17C13CEntry *p;
    int i;

    value = 100.0f;
    p = arg0;
    for (i = 0; i < 0x29; i++) {
        p->field_32c = fn_1_4630((*(u8 * *)&lbl_801A6410), 0x620, (*(u8 (*)[128])&lbl_1_data_1EF5C), 0x58);
        ((Fn17C13CObject *)p->field_32c)->field_6 = (u16)i;
        ((Fn17C13CObject *)p->field_32c)->field_4 = (u16)i;
        ((Fn17C13CObject *)p->field_32c)->field_184 = value;
        p++;
    }
}
/* fzgx:end fn_1_7C13C */

/* fzgx:begin fn_1_7C1E8 */
void fn_1_7C1E8(void *out) {
    u8 *p;
    s8 i;

    p = (u8 *)out;
    for (i = 0; i < 2; i++) {
        *(u32 *)p = (u32)fn_1_4630(lbl_801A6410, 4, (*(u8 (*)[128])&lbl_1_data_1EF5C), 0x68);
        memset(*(void **)p, 0xff, 4);
        *(f32 *)(p + 4) = lbl_1_rodata_33B0[0];
        *(u16 *)(p + 8) = 0xE000;
        *(s16 *)(p + 10) = 0x2000;
        p += 0xc;
    }
}
/* fzgx:end fn_1_7C1E8 */

/* fzgx:begin fn_1_7D694 */
typedef struct {
    u8 _pad4[4];
    s16 value;
} Entry;

typedef struct {
    u8 _pad32c[0x32c];
    Entry *entry;
} Object;

typedef struct {
    u8 _pad7[7];
    s8 value;
} GlobalEntry;

// Compares the object's entry selector with the active global selector.
void fn_1_7D694(Object *obj) {
    Entry *entry = obj->entry;
    GlobalEntry *active = *(GlobalEntry **)&lbl_1_bss_6D7E8;

    if (entry->value == active->value) {
        return;
    }
}
/* fzgx:end fn_1_7D694 */

/* fzgx:begin fn_1_7E7A4 */
typedef struct Fn1_7E7A4Data {
    void *field_0;
    u8 pad_4[8];
    void *field_c;
    u8 pad_10[8];
    void *field_18;
    void *field_1c;
} Fn1_7E7A4Data;

void fn_1_7E7A4(Fn1_7E7A4Data *arg0) {
    s8 i;
    u32 offset;

    for (i = 0, offset = 0; i < 0x29; offset += 0x440, i++) {
        fn_1_46B4(
            lbl_801A6410,
            *(void **)(((((((0x32c) + ((u8 *)arg0->field_0)))) + ((offset))))),
            (*(u8 (*)[128])&lbl_1_data_1EF5C),
            0x313);
    }

    fn_1_7F954(arg0->field_0, 0x29);

    for (i = 0; i < 2; i++) {
        fn_1_46B4(
            lbl_801A6410,
            *(void **)((u8 *)arg0->field_1c + i * 0xc),
            (*(u8 (*)[128])&lbl_1_data_1EF5C),
            0x321);
    }

    fn_1_46B4(lbl_801A6410, arg0->field_18, (*(u8 (*)[128])&lbl_1_data_1EF5C), 0x324);
    fn_1_46B4(lbl_801A6410, arg0->field_c, (*(u8 (*)[128])&lbl_1_data_1EF5C), 0x325);
    fn_1_46B4(lbl_801A6410, arg0, (*(u8 (*)[128])&lbl_1_data_1EF5C), 0x326);
    fn_1_49614();
    fn_80074D68(lbl_1_rodata_33A8, lbl_1_rodata_33A8, lbl_1_rodata_33A8);
    fn_80074D78(1);
}
/* fzgx:end fn_1_7E7A4 */

/* fzgx:begin fn_1_7E9C4 noprologue */
#include "types.h"
#include "rel/main_rel/globals.h"
#include "rel/main_rel/car_test.h"

extern u32 fn_1_5910(void);
extern void fn_1_5634C(s32 arg0);
extern void fn_1_5635C(s32 arg0, f32 arg1, f32 arg2);
extern void fn_1_56374(u8 arg0, u8 arg1, u8 arg2);
extern void fn_1_7EB0C(void);

typedef struct {
    s8 unk_0;
    f32 unk_4;
    f32 unk_8;
    u8 unk_C;
    u8 unk_D;
    u8 unk_E;
    s8 unk_F;
    s32 unk_10;
    s32 unk_14;
} Entry7E9C4;

typedef struct {
    s8 unk_0;
    f32 unk_4;
    f32 unk_8;
    u8 unk_C;
    u8 unk_D;
    u8 unk_E;
    s8 unk_F;
} Cur7E9C4;

static inline s32 check7E9C4(u32 idx) {
    Entry7E9C4 *e = &((Entry7E9C4 *)&lbl_1_bss_6D620)[idx];
    s32 sel = 0;

    if (e->unk_F != 0 || e->unk_10 < 0x20) {
        sel = 1;
    }
    return sel;
}

void fn_1_7E9C4(void) {
    s32 sel;
    if (lbl_1_bss_3BE0 != 0 && (s8)lbl_1_bss_6D7F0 == 0) {
        u32 off;

        off = fn_1_5910() * 0x18;
        sel = 0;
        if (*((s8 *)&lbl_1_bss_6D620 + off + 0xF) != 0 ||
            *(s32 *)((s8 *)&lbl_1_bss_6D620 + off + 0x10) < 0x20) {
            sel = 1;
        }
        ((Cur7E9C4 *)&lbl_1_bss_6D7F4)->unk_F = sel != 0;
        ((Cur7E9C4 *)&lbl_1_bss_6D7F4)->unk_0 = ((u8 *)&lbl_1_bss_6D620)[off];
        ((Cur7E9C4 *)&lbl_1_bss_6D7F4)->unk_4 = *(f32 *)((u8 *)&lbl_1_bss_6D620 + off + 0x4);
        ((Cur7E9C4 *)&lbl_1_bss_6D7F4)->unk_8 = *(f32 *)((((off) + (0x8)) + ((u8 *)&lbl_1_bss_6D620)));
        ((Cur7E9C4 *)&lbl_1_bss_6D7F4)->unk_C = *((u8 *)&lbl_1_bss_6D620 + off + 0xC);
        ((Cur7E9C4 *)&lbl_1_bss_6D7F4)->unk_D = *((u8 *)&lbl_1_bss_6D620 + off + 0xD);
        ((Cur7E9C4 *)&lbl_1_bss_6D7F4)->unk_E = *((u8 *)&lbl_1_bss_6D620 + off + 0xE);
    }

    fn_1_5634C(((Cur7E9C4 *)&lbl_1_bss_6D7F4)->unk_F);
    if (((Cur7E9C4 *)&lbl_1_bss_6D7F4)->unk_F != 0) {
        fn_1_5635C(((Cur7E9C4 *)&lbl_1_bss_6D7F4)->unk_0,
                   ((Cur7E9C4 *)&lbl_1_bss_6D7F4)->unk_4,
                   ((Cur7E9C4 *)&lbl_1_bss_6D7F4)->unk_8);
        fn_1_56374(((Cur7E9C4 *)&lbl_1_bss_6D7F4)->unk_C,
                   ((Cur7E9C4 *)&lbl_1_bss_6D7F4)->unk_D,
                   ((Cur7E9C4 *)&lbl_1_bss_6D7F4)->unk_E);
    }
    fn_1_7EB0C();
}
/* fzgx:end fn_1_7E9C4 */

/* fzgx:begin fn_1_7EAE8 */
// fn_1_7EAE8: main_rel .text:0x0007EAE8 size 0x24
// Wrapper that calls fn_1_5634C with argument 0.

void fn_1_7EAE8(void) {
    fn_1_5634C(0);
}
/* fzgx:end fn_1_7EAE8 */

/* fzgx:begin fn_1_7EB0C */
typedef struct {
    s8 unk_0;
} SignedByteView;

typedef struct {
    u8 unk_0;
    u8 unk_1;
    u8 unk_2;
    u8 unk_3;
} LocalBytes;

void fn_1_7EB0C(void) {
    LocalBytes local0;
    LocalBytes local1;

    local0.unk_0 = lbl_1_bss_6D7F4.unk_C;
    local0.unk_1 = lbl_1_bss_6D7F4.unk_D;
    local0.unk_2 = lbl_1_bss_6D7F4.unk_E;
    if ((s8)lbl_1_bss_6D7F4.unk_F != 0) {
        fn_80071ED4(((SignedByteView *)&lbl_1_bss_6D7F4)->unk_0 + 0,
                    lbl_1_bss_6D7F4.unk_4,
                    lbl_1_bss_6D7F4.unk_8);
        local1 = local0;
        fn_80072014(&local1);
        fn_800720B0(1);
    } else {
        fn_800720B0(0);
    }
}
/* fzgx:end fn_1_7EB0C */

/* fzgx:begin fn_1_7F1E8 */
void fn_1_7F1E8(u32 arg0, u32 arg1) {
    fn_1_7ECB8(arg0, arg1, 0x80000000); // fzgx-allow: A1 retail sentinel
}
/* fzgx:end fn_1_7F1E8 */

/* fzgx:begin fn_1_7F20C */
void fn_1_7F20C(u32 arg0, u32 arg1) {
    fn_1_7ECB8(arg0, arg1, 0x40000000); // fzgx-allow: A1 retail sentinel
}
/* fzgx:end fn_1_7F20C */

/* fzgx:begin fn_1_7F230 */
void fn_1_7F230(u32 arg0, u32 arg1) {
    fn_1_7ECB8(arg0, arg1, 0);
}
/* fzgx:end fn_1_7F230 */

/* fzgx:begin fn_1_7F254 */
void *fn_1_7F254(void *arg0, void *arg1) {
    u8 kind = ((u8 *)arg0)[5];
    s16 type;
    void *result;
    char *data = (char *)&lbl_1_data_1F1D8;

    if (kind == 4) {
        type = 2;
    } else if (kind == 0x28 || kind >= 0x29) {
        type = 3;
    } else {
        type = 1;
    }

    if ((((u32 *)arg0)[0] & 0x40000000) == 0) {
        fn_1_12AB38(data + 0xe0c);
        result = fn_1_45D0(lbl_801A6410, type * 0x30, data + 0xe18, 0x1d5);
        fn_80008BEC(result, 0, type * 0x30);
        fn_1_80F80(result, ((u8 *)arg0)[0x81a0], arg1);
        fn_1_12AB38(data + 0xe20);
    } else {
        struct {
            u16 a;
            u16 b;
            u16 c;
        } info;
        *(u32 *)&info = ((const u32 *)&lbl_1_rodata_3588)[0];
        info.c = ((const u16 *)&lbl_1_rodata_3588)[2];
        info.a = ((u8 *)arg0)[0x81a4];
        info.b = ((u8 *)arg0)[0x81ac];
        info.c = ((u8 *)arg0)[0x81b4];
        result = fn_1_45D0(lbl_801A6410, 0x90, data + 0xe18, 0x1e2);
        fn_80008BEC(result, 0, 0x90);
        fn_1_1502BC(result, &info, arg1);
    }
    return result;
}
/* fzgx:end fn_1_7F254 */

/* fzgx:begin fn_1_7F518 */
// Initializes the car test entry and optionally performs its follow-up setup.
void* fn_1_7F518(s16 car_type, void* car, s32 initialize) {
    fn_80083DB0(car, ((u32 *)lbl_1_data_1F4FC)[car_type]);
    if (initialize == 1) {
        fn_1_801F8(car_type, car);
    }
    return car;
}
/* fzgx:end fn_1_7F518 */

/* fzgx:begin fn_1_83D28 noprologue */
#include "dolphin/types.h"
#include "rel/main_rel/car_test.h"

#pragma section code_type ".fzgxpool"
__declspec(section ".fzgxpool") static void fzgx_pool_prime1(void) {
    volatile f32 s; volatile f64 d;  /* fzgx-allow: S2 pool primer sinks */
    s = 3.0f;
}
static const u32 fzgx_pool_table2[2] = {0x40000000, 0x40000000};  /* fzgx-allow: A1 retail pool bytes */
__declspec(section ".fzgxpool") static void fzgx_pool_keep2(void) { const u32 *volatile cp; cp = fzgx_pool_table2; }  /* fzgx-allow: S2 pool primer sink */
__declspec(section ".fzgxpool") static void fzgx_pool_prime3(void) {
    volatile f32 s; volatile f64 d;  /* fzgx-allow: S2 pool primer sinks */
    s = 1.5f;
    s = 0.0f;
}
static const u32 fzgx_pool_table4[1] = {0x00000000};  /* fzgx-allow: A1 retail pool bytes */
__declspec(section ".fzgxpool") static void fzgx_pool_keep4(void) { const u32 *volatile cp; cp = fzgx_pool_table4; }  /* fzgx-allow: S2 pool primer sink */
__declspec(section ".fzgxpool") static void fzgx_pool_prime5(void) {
    volatile f32 s; volatile f64 d;  /* fzgx-allow: S2 pool primer sinks */
    d = 1.1920928955078125e-07;
    s = 1.4499999284744263f;
    s = -1.5881868392106856e-23f;
    s = 1.0f;
}
static const u32 fzgx_pool_table6[1] = {0x00000000};  /* fzgx-allow: A1 retail pool bytes */
__declspec(section ".fzgxpool") static void fzgx_pool_keep6(void) { const u32 *volatile cp; cp = fzgx_pool_table6; }  /* fzgx-allow: S2 pool primer sink */
__declspec(section ".fzgxpool") static void fzgx_pool_prime7(void) {
    volatile f32 s; volatile f64 d;  /* fzgx-allow: S2 pool primer sinks */
    d = 0.15000000074505807;
    s = 0.05000000074505806f;
    s = 30.0f;
    s = 0.00390625f;
    s = 182.04444885253906f;
    s = 180.0f;
    s = 44.0f;
    s = 89.0f;
    s = 360.0f;
    s = 60.0f;
}
static const u32 fzgx_pool_table8[1] = {0x00000000};  /* fzgx-allow: A1 retail pool bytes */
__declspec(section ".fzgxpool") static void fzgx_pool_keep8(void) { const u32 *volatile cp; cp = fzgx_pool_table8; }  /* fzgx-allow: S2 pool primer sink */
__declspec(section ".fzgxpool") static void fzgx_pool_prime9(void) {
    volatile f32 s; volatile f64 d;  /* fzgx-allow: S2 pool primer sinks */
    d = 0.12999999932944775;
    s = 0.029999999329447746f;
}
static const u32 fzgx_pool_table10[1] = {0x00000000};  /* fzgx-allow: A1 retail pool bytes */
__declspec(section ".fzgxpool") static void fzgx_pool_keep10(void) { const u32 *volatile cp; cp = fzgx_pool_table10; }  /* fzgx-allow: S2 pool primer sink */
__declspec(section ".fzgxpool") static void fzgx_pool_prime11(void) {
    volatile f32 s; volatile f64 d;  /* fzgx-allow: S2 pool primer sinks */
    d = 4503599627370496.0;
    d = 4503601774854144.0;
}
static const u32 fzgx_pool_table12[306] = {0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00330000, 0x00010001, 0x00020002, 0x00030003, 0x0004002F, 0x00050004, 0x00060005, 0x00070006, 0x00080007, 0x00090008, 0x000A002A, 0x000B0009, 0x000C002B, 0x000D000A, 0x000E002C, 0x000F000B, 0x0010002D, 0x0011000C, 0x0012002E, 0x0013000D, 0x0014000E, 0x003D000F, 0x003E0010, 0x003F0011, 0x00400012, 0x00410013, 0x00420014, 0x00430015, 0x00440016, 0x00450017, 0x00460018, 0x00470019, 0x0048001A, 0x0049001B, 0x004A001C, 0x004B001D, 0x004C0034, 0x004D0035, 0x004E0036, 0x004F0037, 0x001F001E, 0x0020001F, 0x00210020, 0x00220021, 0x00230022, 0x00240023, 0x00250024, 0x00260025, 0x00270026, 0x00280027, 0x00290028, 0x002A0029, 0x00500038, 0x00510030, 0x00520031, 0x00530032, 0x00540033, 0xFFFF0000};  /* fzgx-allow: A1 retail pool bytes */
__declspec(section ".fzgxpool") static void fzgx_pool_keep12(void) { const u32 *volatile cp; cp = fzgx_pool_table12; }  /* fzgx-allow: S2 pool primer sink */
__declspec(section ".fzgxpool") static void fzgx_pool_prime13(void) {
    volatile f32 s; volatile f64 d;  /* fzgx-allow: S2 pool primer sinks */
    s = 100.0f;
}
static const u32 fzgx_pool_table14[3] = {0x0032001E, 0x00140004, 0xFFFFFFFF};  /* fzgx-allow: A1 retail pool bytes */
__declspec(section ".fzgxpool") static void fzgx_pool_keep14(void) { const u32 *volatile cp; cp = fzgx_pool_table14; }  /* fzgx-allow: S2 pool primer sink */
__declspec(section ".fzgxpool") static void fzgx_pool_prime15(void) {
    volatile f32 s; volatile f64 d;  /* fzgx-allow: S2 pool primer sinks */
    d = 0.5;
    d = 0.20000000298023224;
    s = 0.10000000149011612f;
    s = 0.11999999731779099f;
    s = -0.10000000149011612f;
    s = -1.0f;
    s = 0.8999999761581421f;
    s = 0.49000000953674316f;
    d = 0.800000011920929;
    s = 0.4000000059604645f;
}
static const u32 fzgx_pool_table16[3] = {0x00000000, 0x3F000000, 0x3F000000};  /* fzgx-allow: A1 retail pool bytes */
__declspec(section ".fzgxpool") static void fzgx_pool_keep16(void) { const u32 *volatile cp; cp = fzgx_pool_table16; }  /* fzgx-allow: S2 pool primer sink */
__declspec(section ".fzgxpool") static void fzgx_pool_prime17(void) {
    volatile f32 s; volatile f64 d;  /* fzgx-allow: S2 pool primer sinks */
    s = 2.0f;
    s = -1.1920928955078125e-07f;
    s = 240.0f;
    s = 0.75f;
    s = 0.5f;
    s = 1000.0f;
}
#pragma section code_type ".text"

typedef struct {
    u8 pad[0x200];
    f32 value;
} ProjectionObject;
typedef struct {
    u8 pad[8];
    struct { f32 x, y, z; } vector;
    f32 radius;
} DistanceObject;
typedef struct {
    u8 pad[0x320];
    s16 index;
    u8 pad322[0xA];
    ProjectionObject *projection;
    struct {
        s16 threshold;
        u8 pad[2];
        DistanceObject **object;
        u8 pad8[4];
    } levels[6];
    u8 pad378[0x18];
    u32 flags;
    u8 pad394[0xC];
    struct {
        u8 pad[0x344];
        struct {
            u8 pad[8];
            DistanceObject **object;
        } *levels[6];
    } *other;
} DistanceState;
extern s8 fn_1_84124(void *, s8, s8, u8 *);
extern s32 fn_1_54E34(void *, f32);
extern f32 fn_1_A71AC(void);
extern s8 fn_1_84644(void *);
extern u8 lbl_1_bss_6D980[32];
extern u8 lbl_1_bss_6D9A0[188];
extern s32 camera_get_values(f32 *, f32 *);
extern s32 fn_1_A5DC4(void);
extern void lbl_8006DFE8(void *);
extern void lbl_8006E1B0(void *, void *);

#pragma opt_lifetimes off
s8 fn_1_83D28(DistanceState *state, f32 scale)
{
    s8 result = -1;
    s8 level;
    DistanceObject *object;
    s8 mode;
    u8 value;
    f32 camera_a;
    f32 camera_b;
    struct { f32 x, y, z; } vector;
    f32 diameter;
    f32 distance;
    f32 threshold;

    level = fn_1_84124(state, 1, 0, &value);
    if (state->flags & 0x04000000) {
        object = *state->other->levels[level]->object;
    } else {
        object = *state->levels[level].object;
    }
    if (object == 0) {
        return -1;
    }
    diameter = 2.0f * object->radius * scale;
    lbl_8006DFE8((u8 *)state->projection + 0x14C);
    if (fn_1_54E34(&object->vector, object->radius) == 0) {
        return 5;
    }
    lbl_8006E1B0((u8 *)object + 8, &vector);
    if (vector.z > -1.1920928955078125e-7f) {
        return level;
    }
    distance = -vector.z * fn_1_A71AC();
    mode = fn_1_84644(state->projection);
    lbl_1_bss_6D980[state->index] = 0;
    distance = 240.0f * diameter / distance;
    if (camera_get_values(&camera_a, &camera_b)) {
        distance *= camera_b;
    }
    if ((s8)fn_1_A5DC4()) {
        distance *= 0.75f;
    }
    {
    s8 fzgx_loop_level_2202;
for (fzgx_loop_level_2202 = 0; fzgx_loop_level_2202 < 6; fzgx_loop_level_2202++) {
        threshold = state->levels[fzgx_loop_level_2202 + 1].threshold;
        if (fzgx_loop_level_2202 == 3) {
            if (mode == 0) {
                threshold = 2.0f;
            } else if (distance < lbl_1_data_1FFE0.unk_0) {
                lbl_1_bss_6D980[state->index] = 1;
            }
        }
        if (distance > threshold) {
            if (fzgx_loop_level_2202 > 0) {
                f32 ratio = distance / (state->levels[fzgx_loop_level_2202].threshold + threshold);
                if (ratio > 0.5f) {
                    lbl_1_bss_6D9A0[state->index] = 0;
                } else {
                    lbl_1_bss_6D9A0[state->index] = 1;
                }
            } else {
                lbl_1_bss_6D9A0[state->index] = 1;
            }
            if (fzgx_loop_level_2202 == 4) {
                fzgx_loop_level_2202 = 5;
            }
            result = fzgx_loop_level_2202;
            break;
        }
        if (threshold < 0.0f) {
            result = fzgx_loop_level_2202;
            break;
        }
    }
    level = fzgx_loop_level_2202;
}
    if (result == 5 && state->projection != 0 && state->projection->value == 0.0f) {
        lbl_1_bss_6D980[state->index] = 0;
        result--;
    }
    return result;
}
#pragma opt_lifetimes reset
/* fzgx:end fn_1_83D28 */

/* fzgx:begin fn_1_8CA00 */
u32 *fn_1_8CA00(void) {
    return &lbl_1_data_1FFDC;
}
/* fzgx:end fn_1_8CA00 */
