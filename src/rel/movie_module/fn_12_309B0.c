#include "types.h"

typedef struct MovieModuleData {
    u8 pad_0[0x0c];
    u32 field_0c;
} MovieModuleData;

void fn_12_309B0(MovieModuleData *data, u32 value) {
    data->field_0c = value;
}
