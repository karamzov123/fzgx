#include "types.h"

typedef struct MovieModule {
    u8 _pad0[0x318];
    u32 field_318;
    u32 field_31c;
    u32 field_320;
} MovieModule;

void fn_12_9730(MovieModule *module) {
    module->field_318 = 0x400;
    module->field_320 = 0x400;
    module->field_31c = 0x400;
}
