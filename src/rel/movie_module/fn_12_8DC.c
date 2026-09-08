#include "types.h"

typedef struct MovieModule {
    u8 unk0[4];
    s32 value;
} MovieModule;

void fn_12_8DC(MovieModule* module, s32 value) {
    module->value = value;
}
