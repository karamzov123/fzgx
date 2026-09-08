#include "types.h"

typedef struct MovieModule MovieModule;
typedef void (*MovieCallback)(MovieModule *, void *);

struct MovieModule {
    u8 _pad[0x1c84];
    struct {
        u8 _pad0[0xc];
        MovieCallback callback;
    } *ops;
};

extern int fn_12_24990(MovieModule *);
extern void fn_12_24A88(int, u32);
extern int fn_12_2D73C(MovieModule *, int);

void fn_12_21A64(MovieModule *module, void *arg) {
    if (fn_12_24990(module) != 0) {
        fn_12_24A88(0, 0xff0001a3);
    } else if (fn_12_2D73C(module, 6) != 0) {
        module->ops->callback(module, arg);
    }
}
