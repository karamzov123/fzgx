#include "types.h"

typedef struct MovieModule {
    u8 _pad0[0x40];
    void *field_40;
    u8 _pad44[0x110];
    u8 field_154;
    u8 _pad155[0xff];
    u32 field_254;
    u8 _pad258[0xe8];
    u8 field_340;
} MovieModule;

extern void fn_12_8878(void *arg0, u32 arg1, void *arg2, void *arg3);

void fn_12_89D4(MovieModule *module) {
    u32 value = module->field_254;

    module->field_254 = value + 0x180;
    fn_12_8878(
        &module->field_340,
        value,
        &module->field_154,
        module->field_40
    );
}
