#include "types.h"

extern int fn_12_24990(void);
extern void fn_12_24A88(int, void *);
extern int fn_12_2D73C(void *, int);

typedef int (*MovieCallback)(void *);

typedef struct MovieModule {
    u8 pad[0x1c84];
    struct {
        u8 pad[0x10];
        MovieCallback callback;
    } *vtable;
} MovieModule;

int fn_12_219E8(MovieModule *self) {
    if (fn_12_24990() != 0) {
        fn_12_24A88(0, (void *)((0xff00 << 16) | 0x1a4));
        return 0;
    }
    if (fn_12_2D73C(self, 6) == 0) {
        return 0;
    }
    return self->vtable->callback(self);
}
