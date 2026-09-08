#include "types.h"

typedef struct MovieModule {
    u8 pad_0[0x9d8];
    s32 state;
    u8 pad_9dc[0x74];
    s32 movie;
} MovieModule;

extern void fn_12_2DBD4(void *, void *, void *, s32 *, s32);

s32 fn_12_2DEB0(MovieModule *module, void *arg1, void *arg2, s32 *arg3, s32 arg4) {
    s32 result;

    if (module->state != 0) {
        return 1;
    }
    fn_12_2DBD4(module, arg1, arg2, &result, module->movie);
    return result;
}
