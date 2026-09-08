#include "types.h"

typedef struct MovieModule {
    u32 unk_00;
    u8 pad_04[8];
    u32 unk_0c;
    u8 pad_10[0x80];
    u32 unk_90;
    u8 unk_94[1];
} MovieModule;

extern void *fn_12_3310C(void *, u32);
extern void fn_12_23F5C(void *, MovieModule *);
extern void fn_12_330E0(void *);

void fn_12_24570(MovieModule *self) {
    void *movie;

    movie = fn_12_3310C(self->unk_94, self->unk_90);
    if (movie != 0) {
        fn_12_23F5C(movie, self);
        fn_12_330E0(movie);
    }
}
