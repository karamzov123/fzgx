#include "types.h"

typedef struct MovieObject {
    u8 pad[0x54];
    u32 value;
} MovieObject;

extern MovieObject *fn_12_38DBC(void);

s32 fn_12_3A92C(void) {
    MovieObject *object = fn_12_38DBC();
    return object->value != 1;
}
