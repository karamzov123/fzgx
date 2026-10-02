#include "types.h"

typedef struct MovieData {
    void *movie;
    u8 pad_4[0x20];
    s32 unk_24;
    s32 unk_28;
} MovieData;

typedef struct MovieModule {
    u8 pad_0[0x48];
    s32 unk_48;
    u8 pad_4C[0xF60 - 0x4C];
    u8 unk_F60[0x1B74 - 0xF60];
    MovieData *movie_data;
} MovieModule;

typedef struct Pair64 {
    s64 a;
    s64 b;
} Pair64;

extern s32 fn_12_2E44C(MovieModule *, s32 *, s32 *);
extern u32 lbl_12_bss_6988;
extern s64 fn_12_335B8(void);
extern s64 fn_12_335A4(void);
extern void fn_12_2FE60(void *, Pair64 *, Pair64 *, Pair64 *);
extern s32 fn_8004C658(void *);
extern void fn_8004C164(void *, s32 *, s32 *);

s32 fn_12_205BC(MovieModule *arg0, s32 *arg1, s32 *arg2) {
    Pair64 out;
    Pair64 scale;
    Pair64 in;
    s32 y;
    s32 x;
    s32 ra;
    s32 rb;
    MovieData *md;
    void *movie;
    void *obj;

    obj = arg0->unk_F60;
    md = arg0->movie_data;
    movie = md->movie;

    if (fn_12_2E44C(arg0, arg1, arg2) == 0) {
        return 0;
    }
    if (arg0->unk_48 == 4) {
        lbl_12_bss_6988 = fn_8004C658(movie);
        fn_8004C164(movie, &x, &y);
        in.a = x;
        in.b = y;
        scale.a = fn_12_335B8();
        scale.b = fn_12_335A4();
        fn_12_2FE60(obj, &in, &scale, &out);
        ra = (s32)out.a;
        rb = (s32)out.b;
        if (md->unk_24 < ra) {
            md->unk_24 = ra;
            md->unk_28 = rb;
        }
    }
    *arg1 = md->unk_24;
    *arg2 = md->unk_28;
    return 0;
}
