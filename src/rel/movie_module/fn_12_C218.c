#include "types.h"

typedef struct MovieObject MovieObject;
typedef struct MovieArgs {
    u32 field_0000;
    u32 field_0004;
} MovieArgs;

typedef struct MovieVTable {
    u8 pad_0000[0x0c];
    void (*method_000c)(MovieObject *);
    u8 pad_0010[0x14];
    s32 (*method_0024)(MovieObject *, s32);
} MovieVTable;

struct MovieObject {
    MovieVTable *vtable;
};

extern MovieObject *fn_80057B9C(u32 arg0, u32 arg1);
extern s32 fn_12_C2C8(void *arg0, MovieObject *movie);

s32 fn_12_C218(void *arg0, MovieArgs *args, s32 *result) {
    s32 status;
    s32 elapsed;
    MovieObject *movie;

    movie = fn_80057B9C(args->field_0000, args->field_0004);
    if (movie == 0) {
        return -1;
    }
    status = fn_12_C2C8(arg0, movie);
    elapsed = movie->vtable->method_0024(movie, 1);
    *result = args->field_0004 - elapsed;
    movie->vtable->method_000c(movie);
    return status;
}
