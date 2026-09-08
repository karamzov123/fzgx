#include "types.h"

extern u32 autoInvalidation_801A647C;

u32 fn_8001989C(u32 arg0) {
    u32 v0;
    v0 = autoInvalidation_801A647C;
    autoInvalidation_801A647C = arg0;
    return v0;
}
