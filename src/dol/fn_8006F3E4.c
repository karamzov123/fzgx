#include "types.h"

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec3;

typedef struct {
    Vec3 pos;
    Vec3 dir;
} VecPair;

extern void lbl_8006DAEC(void *, void *, void *);
extern const f32 lbl_801A73F0;
extern const f32 lbl_801A73F4;
extern const f32 lbl_801A73F8;
extern void lbl_8006E1C0(VecPair *, VecPair *);
extern s16 lbl_8006D24C(f32, f32);
extern f32 lbl_8006D0B4(f32);
extern void lbl_8006D8D8(s16);
extern void mathutil_mtxA_rotate_x(s16);
extern void fn_8006E2B0(VecPair *, VecPair *);
extern void lbl_8006DB30(void);

#pragma opt_propagation off
static inline f32 fn_8006F3E4_read_pointer(VecPair * owner) { return owner->dir.z; }
void fn_8006F3E4(s16 *arg0, s16 *arg1, s16 *arg2) {
    f32 sum;
    f32 mz;
    f32 my;
    f32 mx;
    VecPair vec;
    f32 fzgx_live;

    lbl_8006DAEC(arg0, arg1, arg2);

    vec.pos.x = lbl_801A73F0;
    vec.pos.y = lbl_801A73F4;
    vec.pos.z = lbl_801A73F0;

    mx = *(f32 *)(0xE0000000 + 0x08);
    my = *(f32 *)(0xE0000000 + 0x18);
    mz = *(f32 *)(0xE0000000 + 0x28);

    vec.dir.x = mx * lbl_801A73F8;
    vec.dir.y = my * lbl_801A73F8;
    vec.dir.z = mz * lbl_801A73F8;

    lbl_8006E1C0(&vec, &vec);

    sum = vec.dir.x * vec.dir.x;
    fzgx_live = fn_8006F3E4_read_pointer(&vec);
    *arg1 = lbl_8006D24C(vec.dir.y, lbl_8006D0B4(sum + fzgx_live * fzgx_live));
    *arg0 = lbl_8006D24C(vec.dir.x, (vec.dir.z)) + -32768;

    lbl_8006D8D8(*arg0);
    mathutil_mtxA_rotate_x(*arg1);

    fn_8006E2B0(&vec, &vec);
    *arg2 = -lbl_8006D24C(vec.pos.x, vec.pos.y);

    lbl_8006DB30();
}
#pragma opt_propagation reset

