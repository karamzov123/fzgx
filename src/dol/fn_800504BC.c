#include "types.h"

extern u32 memset(u32, u32, u32);

void fn_800504BC(u32 arg0) {
    if (arg0 != 0) {
    memset(arg0, 0, 44);
    }
}
