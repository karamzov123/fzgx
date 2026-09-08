#include "types.h"

typedef struct MovieObject {
    u8 _pad[0x10];
    s32 value;
} MovieObject;

extern s32 fn_8004E580(MovieObject *self);
extern MovieObject *fn_12_38DBC(void);
extern s32 fn_12_3AB00(MovieObject *self);

s32 fn_12_3AC70(MovieObject *self) {
    s32 result = 0;

    if (fn_8004E580(self) == 1) {
        if (fn_12_38DBC()->value != 1) {
            result = fn_12_3AB00(self);
        }
    }

    return result;
}
