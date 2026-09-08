#include "types.h"

typedef struct MovieModule {
    u8 _pad120[0x120];
    s32 field_120;
    u8 _pad124[0x30];
    s32 field_154;
    u8 _pad158[0x4c];
    s32 field_1a4;
} MovieModule;

void fn_12_8AEC(MovieModule *module) {
    s32 value = 4;

    if (module->field_1a4 == 0) {
        value = -1;
    }
    module->field_154 = value;
    module->field_120 = value;
}
