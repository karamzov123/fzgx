#include "types.h"

typedef struct MovieModule MovieModule;
typedef void (*MovieCallback)(MovieModule *, void *, void *);

struct MovieModule {
    u8 _pad[0x1c84];
    struct {
        u8 _pad0[4];
        MovieCallback callback;
    } *ops;
};

extern int fn_12_24990(MovieModule *);
extern void fn_12_24A88(int, u32);
extern int fn_12_2D73C(MovieModule *, int);

void fn_12_21B70(MovieModule *module, void *arg0, void *arg1) {
    if (fn_12_24990(module) != 0) {
        fn_12_24A88(0, 0xff0001a1);
    } else if (fn_12_2D73C(module, 6) != 0) {
        module->ops->callback(module, arg0, arg1);
    }
}
