#include "types.h"

typedef struct MovieValues {
    u32 value_00;
    u32 value_04;
    u32 value_08;
    u32 value_0c;
    u32 value_10;
    u32 value_14;
    u32 value_18;
    u32 value_1c;
} MovieValues;

typedef struct MovieModule {
    u8 _pad00[0xa0];
    MovieValues values;
} MovieModule;

extern int fn_12_6C78(MovieModule *);
extern int fn_12_6D5C(int, int);

int fn_12_683C(MovieModule *module, MovieValues *result) {
    if (fn_12_6C78(module)) {
        return fn_12_6D5C(0, 0xff020203);
    }
    *result = module->values;
    return 0;
}
