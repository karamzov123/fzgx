#include "types.h"

struct MovieModule {
    char pad_44[0x44];
    int field_44;
    int field_48;
    int field_4c;
};

extern int fn_12_24990(struct MovieModule *module);
extern int fn_12_24A88(int arg0, u32 arg1);
extern int fn_12_2D73C(struct MovieModule *module, int arg1);
extern int fn_12_2AD78(struct MovieModule *module);

int fn_12_2B634(struct MovieModule *module) {
    int result;

    if (fn_12_24990(module)) {
        return fn_12_24A88(0, 0xff000132);
    } else {
        if (fn_12_2D73C(module, 0x2f) == 1) {
            result = fn_12_2AD78(module);
        } else {
            result = 0;
            module->field_4c = 4;
        }
        module->field_44 = 1;
        return result;
    }
}
