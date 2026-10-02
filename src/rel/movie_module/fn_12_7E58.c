#include "types.h"

struct fn_12_7E58_b {
    u32 unk_0;
    u32 unk_4;
};

struct fn_12_7E58_c {
    u32 unk_0;
    u32 unk_4;
    u32 unk_8;
    s16 unk_C;
    s16 unk_E;
};

struct fn_12_7E58_a {
    f64 v[16];
    f64 w[32];
};

static inline f64 fn_12_7E58_array_read(f64 *array, s32 index) { return array[index]; }
#pragma opt_dead_assignments off
#pragma opt_common_subs off
void fn_12_7E58(struct fn_12_7E58_a *arg0, struct fn_12_7E58_b *arg1,
                struct fn_12_7E58_c *arg2) {
    u32 st0;
    u32 st1;
    u8 *d0;
    u8 *q0;
    u8 *q1;
    f64 *w;
    f64 *p;
    f64 *s;

    st0 = arg2->unk_C & ~7;

    d0 = (u8 *)(arg2->unk_0 + arg1->unk_0);
    *(f64 *)d0 = fn_12_7E58_array_read(arg0->v, 0);
    d0 += st0;
    *(f64 *)d0 = fn_12_7E58_array_read(arg0->v, 1);
    d0 += st0;
    *(f64 *)d0 = fn_12_7E58_array_read(arg0->v, 2);
    d0 += st0;
    *(f64 *)d0 = fn_12_7E58_array_read(arg0->v, 3);
    d0 += st0;
    *(f64 *)d0 = fn_12_7E58_array_read(arg0->v, 4);
    d0 += st0;
    *(f64 *)d0 = fn_12_7E58_array_read(arg0->v, 5);
    d0 += st0;
    *(f64 *)d0 = fn_12_7E58_array_read(arg0->v, 6);
    d0 += st0;
    *(f64 *)d0 = fn_12_7E58_array_read(arg0->v, 7);

    d0 = (u8 *)(arg2->unk_4 + arg1->unk_0);
    *(f64 *)d0 = fn_12_7E58_array_read(arg0->v, 8);
    d0 += st0;
    *(f64 *)d0 = fn_12_7E58_array_read(arg0->v, 9);
    d0 += st0;
    *(f64 *)d0 = fn_12_7E58_array_read(arg0->v, 10);
    d0 += st0;
    *(f64 *)d0 = fn_12_7E58_array_read(arg0->v, 11);
    d0 += st0;
    *(f64 *)d0 = fn_12_7E58_array_read(arg0->v, 12);
    d0 += st0;
    *(f64 *)d0 = fn_12_7E58_array_read(arg0->v, 13);
    d0 += st0;
    *(f64 *)d0 = fn_12_7E58_array_read(arg0->v, 14);
    d0 += st0;
    *(f64 *)d0 = fn_12_7E58_array_read(arg0->v, 15);

    st1 = arg2->unk_E & ~7;

    q0 = (u8 *)(arg2->unk_8 + arg1->unk_4);
    q1 = q0 + 8;

    w = arg0->w;
    p = w + 3;
    s = w + 11;

    *(f64 *)q0 = fn_12_7E58_array_read(w, 0);
    *(f64 *)q1 = fn_12_7E58_array_read(w, 8);
    q0 += st1;
    q1 += st1;
    *(f64 *)q0 = fn_12_7E58_array_read(w, 1);
    *(f64 *)q1 = fn_12_7E58_array_read(w, 9);
    q0 += st1;
    q1 += st1;
    *(f64 *)q0 = fn_12_7E58_array_read(w, 2);
    q0 += st1;
    *(f64 *)q1 = fn_12_7E58_array_read(w, 10);
    q1 += st1;

    *(f64 *)q0 = fn_12_7E58_array_read(p, 0);
    *(f64 *)q1 = fn_12_7E58_array_read(s, 0);
    q0 += st1;
    q1 += st1;
    *(f64 *)q0 = fn_12_7E58_array_read(p, 1);
    *(f64 *)q1 = fn_12_7E58_array_read(s, 1);
    q0 += st1;
    q1 += st1;
    *(f64 *)q0 = fn_12_7E58_array_read(p, 2);
    *(f64 *)q1 = fn_12_7E58_array_read(s, 2);
    q0 += st1;
    q1 += st1;
    *(f64 *)q0 = fn_12_7E58_array_read(p, 3);
    *(f64 *)q1 = fn_12_7E58_array_read(s, 3);
    q0 += st1;
    q1 += st1;
    *(f64 *)q0 = fn_12_7E58_array_read(p, 4);
    *(f64 *)q1 = fn_12_7E58_array_read(s, 4);
    q0 += st1;
    q1 += st1;

    *(f64 *)q0 = fn_12_7E58_array_read(p, 13);
    *(f64 *)q1 = fn_12_7E58_array_read(s, 13);
    q0 += st1;
    q1 += st1;
    *(f64 *)q0 = fn_12_7E58_array_read(p, 14);
    *(f64 *)q1 = fn_12_7E58_array_read(s, 14);
    q0 += st1;
    q1 += st1;
    *(f64 *)q0 = fn_12_7E58_array_read(p, 15);
    *(f64 *)q1 = fn_12_7E58_array_read(s, 15);
    q0 += st1;
    q1 += st1;
    *(f64 *)q0 = fn_12_7E58_array_read(p, 16);
    *(f64 *)q1 = fn_12_7E58_array_read(s, 16);
    q0 += st1;
    q1 += st1;
    *(f64 *)q0 = fn_12_7E58_array_read(p, 17);
    *(f64 *)q1 = fn_12_7E58_array_read(s, 17);
    q0 += st1;
    q1 += st1;
    *(f64 *)q0 = fn_12_7E58_array_read(p, 18);
    *(f64 *)q1 = fn_12_7E58_array_read(s, 18);
    q0 += st1;
    q1 += st1;
    *(f64 *)q0 = fn_12_7E58_array_read(p, 19);
    *(f64 *)q1 = fn_12_7E58_array_read(s, 19);
    q0 += st1;
    q1 += st1;
    *(f64 *)q0 = fn_12_7E58_array_read(p, 20);
    *(f64 *)q1 = fn_12_7E58_array_read(s, 20);
}
#pragma opt_common_subs reset

#pragma opt_dead_assignments reset

