#include "types.h"
#include "rel/main_rel/globals.h"
#include "rel/main_rel/cloth.h"

extern void fn_1_105744(void);
extern void fn_1_106DB4(void);
extern void fn_1_9D0EC(u32, u32, void*, void*);
extern void fn_1_103AA8(void);
extern u32 lbl_1_bss_85288[2];
extern void fn_1_103AD4(void);
extern void fn_1_105768(void);
extern void fn_1_9E5B8(void *);
extern struct fn_1_10240C_lbl_801A6410 lbl_801A6410;
extern u32 fn_1_45D0(u32, u32, void *, u32);
extern u32 fn_1_46B4(u32, u32, void *, u32);
extern u32 fn_80008BA8(u32, u32, u32);

/* fzgx:begin fn_1_101AE8 noprologue */
#include "types.h"

struct fn_1_101AE8_lbl_1_bss_85250 {
    u8 pad_0[0xC];
    u32 unk_C;
    u32 unk_10;
    u8 pad_14[0x4];
    u32 unk_18;
    u8 pad_1C[0x4];
    u32 unk_20;
};
struct fn_1_101AE8_lbl_801A6410 {
    u32 unk_0;
};

extern struct fn_1_101AE8_lbl_1_bss_85250 lbl_1_bss_85250;
extern struct fn_1_101AE8_lbl_801A6410 lbl_801A6410;
extern u32 fn_1_435C(u32);
extern u32 fn_1_46B4(u32, u32, u32, u32);
extern u32 lbl_1_data_3F17C;
extern void fn_1_41A8(void);
extern void fn_800711A8(void *);
extern void fn_80071718(void *);

void fn_1_101AE8(u32 arg0, u32 arg1, u32 arg2, u32 arg3) {
    struct fn_1_101AE8_lbl_1_bss_85250 *p_lbl_1_bss_85250;
    u32 v0;
    u32 v1;
    u32 v2;
    u32 v3;
    u32 v4;
    u32 v5;
    u32 v6;
    u32 v7;
    u32 t6, t7;
    p_lbl_1_bss_85250 = (struct fn_1_101AE8_lbl_1_bss_85250 *)&lbl_1_bss_85250;
    fn_1_435C(arg0);
    fn_1_41A8();
    fn_1_435C(arg1);
    fn_1_41A8();
    v0 = p_lbl_1_bss_85250->unk_C;
    if (v0 != 0) {
    fn_80071718((void *)v0);
    p_lbl_1_bss_85250->unk_C = 0;
    }
    v1 = p_lbl_1_bss_85250->unk_10;
    if (v1 != 0) {
    fn_800711A8((void *)v1);
    p_lbl_1_bss_85250->unk_10 = 0;
    }
    v2 = p_lbl_1_bss_85250->unk_18;
    v5 = arg3;
    if (v2 != 0) {
    v5 = (u32)&lbl_801A6410;
    v3 = (u32)&lbl_1_data_3F17C;
    v4 = *(u32 *)((u8 *)v5 + 0);
    v5 = 257;
    t6 = fn_1_46B4(v4, v2, (u32)v3, v5);
    v4 = t6;
    p_lbl_1_bss_85250->unk_18 = 0;
    }
    v6 = p_lbl_1_bss_85250->unk_20;
    v7 = v4;
    if (v6 != 0) {
    v7 = lbl_801A6410.unk_0;
    t7 = fn_1_46B4(v7, v6, (u32)&lbl_1_data_3F17C, 258);
    v7 = t7;
    p_lbl_1_bss_85250->unk_20 = 0;
    }
}
/* fzgx:end fn_1_101AE8 */

/* fzgx:begin fn_1_10240C */
struct fn_1_10240C_lbl_801A6410 {
    u32 unk_0;
};

void fn_1_10240C(u32 arg0, u32 arg1, s32 arg2) {
    u32 t0;
    t0 = fn_1_45D0(lbl_801A6410.unk_0, arg2, &lbl_1_data_3F17C, 561);
    fn_80008BA8(t0, arg0, arg2);
    fn_80008BA8(arg0, arg1, arg2);
    fn_80008BA8(arg1, t0, arg2);
    fn_1_46B4(lbl_801A6410.unk_0, t0, &lbl_1_data_3F17C, 565);
}
/* fzgx:end fn_1_10240C */

/* fzgx:begin fn_1_1024C4 */
typedef struct Point1024C4 {
    u8 pad[0x10];
    f32 v[3];
} Point1024C4;

f32 fn_1_1024C4(const Point1024C4 *a, const Point1024C4 *b) {
    f32 dz = a->v[2] - b->v[2];
    f32 dy = a->v[1] - b->v[1];
    f32 dx = a->v[0] - b->v[0];
    f32 result = dx * dx;
    result += dy * dy;
    result += dz * dz;
    return result;
}
/* fzgx:end fn_1_1024C4 */

/* fzgx:begin fn_1_1024F8 */
struct Obj1024F8 {
    u8 pad_0[0x4];
    u32 unk_4;
    u8 pad_8[0x18];
    u32 unk_20;
};

struct Slot8 {
    struct Obj1024F8 *ptr;
    u32 pad_4;
};

typedef struct {
    u32 a;
    u32 b;
    u32 c;
} W3_1024F8;

typedef struct {
    W3_1024F8 v;
    u8 pad[0x34];
} Dst1024F8;

typedef struct {
    u8 unk_0;
    u8 unk_1;
    u8 unk_2;
    u16 unk_4[6];
    W3_1024F8 pos;
    u32 unk_1C;
    u32 unk_20;
    u32 unk_24;
    W3_1024F8 vel;
    u8 pad_34[0x10];
} Node1024F8;

typedef struct {
    u8 pad_0[0x10];
    f32 x;
    f32 y;
    f32 z;
} NodeF1024F8;

extern f64 lbl_1_rodata_7898;
extern u32 lbl_801A66A0;
extern f32 lbl_8006D188(u32);
extern s32 fn_80077B14(struct Obj1024F8 *);

#define ARR ((Node1024F8 *)p->unk_18)
#define N (&ARR[i])
#define B ((u8 *)N)
#define NF ((NodeF1024F8 *)B)

void fn_1_1024F8(void) {
    Obj_1_bss_85250 *p = (Obj_1_bss_85250 *)&lbl_1_bss_85250;
    struct Slot8 *tab;
    struct Obj1024F8 *obj;
    Dst1024F8 *out;
    u32 i;
    u32 j;
    f64 t;

    tab = (struct Slot8 *)p->unk_10->unk_8;
    obj = tab[p->unk_4].ptr;
    if (obj == 0) {
        return;
    }
    out = (Dst1024F8 *)fn_80077B14(obj);
    if (obj->unk_4 & 0x10) {
        for (i = 0; i < p->unk_1C; i++) {
            Node1024F8 *n = (Node1024F8 *)B;
            n->pos = n->vel;
            for (j = 0; j < N->unk_2; j++) {
                out[N->unk_4[j]].v = N->pos;
            }
        }
    }
    for (i = 0; i < p->unk_1C; i++) {
        if (N->unk_1 == 1 && (N->unk_0 & 1) == 0) {
            t = lbl_1_rodata_7898 * lbl_8006D188((s16)((lbl_801A66A0 + i * i) << 8));
            NF->x += t;
            t = lbl_1_rodata_7898 * lbl_8006D188((s16)(((lbl_801A66A0 + i * i + i) << 7) + 0x4000));
            NF->y += t;
            t = lbl_1_rodata_7898 * lbl_8006D188((s16)((lbl_801A66A0 + i * i * i) << 9));
            NF->z += t;
            for (j = 0; j < N->unk_2; j++) {
                out[N->unk_4[j]].v = N->pos;
            }
        }
    }
}
/* fzgx:end fn_1_1024F8 */

/* fzgx:begin fn_1_102FD0 */
// Initialize the cloth subsystem and reset its shared state.
void fn_1_102FD0(void) {
    fn_1_105744();
    fn_1_9D0EC(lbl_1_bss_3BE0->unk_54, lbl_1_bss_3BE0->unk_48,
               lbl_1_data_3F244, fn_1_106DB4);
    fn_1_103AA8();
    lbl_1_bss_85288[0] = -1;
    lbl_1_bss_85288[1] = -1;
}
/* fzgx:end fn_1_102FD0 */

/* fzgx:begin fn_1_10302C */
// fn_1_10302C: empty in retail (single blr).
void fn_1_10302C(void) {
}
/* fzgx:end fn_1_10302C */

/* fzgx:begin fn_1_103030 */
void fn_1_103030(void) {
    fn_1_103AD4();
    fn_1_105768();
}
/* fzgx:end fn_1_103030 */

/* fzgx:begin fn_1_103054 */
// fn_1_103054: empty in retail (single blr).
void fn_1_103054(void) {
}
/* fzgx:end fn_1_103054 */

/* fzgx:begin fn_1_103058 */
// Runs the cloth update unless the global state is one of the inactive modes.
void fn_1_103058(void) {
    s16 state = *(s16 *)(void *)&lbl_1_bss_960;

    if (state != 10 && state != 14) {
        fn_1_10576C();
    }
}
/* fzgx:end fn_1_103058 */

/* fzgx:begin fn_1_103090 */
typedef struct ClothState {
    u8 _pad68[0x68];
    void *field_68;
    u8 _pad6c[8];
    void *field_74;
    void *field_78;
    u8 field_7c;
} ClothState;

void fn_1_103090(ClothState *self) {
    self->field_74 = self->field_68;
    self->field_78 = &self->field_7c;
}
/* fzgx:end fn_1_103090 */

/* fzgx:begin fn_1_1030A4 */
typedef struct Cloth {
    u8 pad_00[0x68];
    s32 field_68;
    u8 field_6c[4];
    u8 field_70[1];
} Cloth;

void fn_1_1030A4(Cloth *self) {
    if (self->field_68 != 0) {
        fn_1_9E5B8(&self->field_70);
    }
}
/* fzgx:end fn_1_1030A4 */

/* fzgx:begin fn_1_106CAC */
extern s32 fn_1_151AF0(void *arg0, int arg1, void *arg2, void *arg3, void *arg4, void *arg5);

s32 fn_1_106CAC(void) {
    Obj_1_bss_85280 *base = &lbl_1_bss_85280;

    if (*(s32 *)((u8 *)base + 0x248) == 2) {
        return 0;
    }

    fn_1_151AF0((void *)base->unk_4, 0, (void *)((u32 *)&lbl_1_data_3F284)[*(u8 *)((u8 *)base + 0x908)], 0, 0, (void *)base->unk_0);
    *(u8 *)((u8 *)base + 0x908) = 1 - *(u8 *)((u8 *)base + 0x908);
    *(s32 *)((u8 *)base + 0x248) = 1;
    return 0;
}
/* fzgx:end fn_1_106CAC */
